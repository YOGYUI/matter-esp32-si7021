#include "si7021.h"
#include "logger.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define SI7021_DEFAULT_ADDRESS      0x40
#define SI7021_MEASRH_HOLD_CMD      0xE5    /**< Measure Relative Humidity, Hold Master Mode */
#define SI7021_MEASRH_NOHOLD_CMD    0xF5    /**< Measure Relative Humidity, No Hold Master Mode */
#define SI7021_MEASTEMP_HOLD_CMD    0xE3    /**< Measure Temperature, Hold Master Mode */
#define SI7021_MEASTEMP_NOHOLD_CMD  0xF3    /**< Measure Temperature, No Hold Master Mode */
#define SI7021_READPREVTEMP_CMD     0xE0    /**< Read Temperature Value from Previous RH Measurement */
#define SI7021_RESET_CMD            0xFE    /**< Reset Command */
#define SI7021_WRITERHT_REG_CMD     0xE6    /**< Write RH/T User Register 1 */
#define SI7021_READRHT_REG_CMD      0xE7    /**< Read RH/T User Register 1 */
#define SI7021_WRITEHEATER_REG_CMD  0x51    /**< Write Heater Control Register */
#define SI7021_READHEATER_REG_CMD   0x11    /**< Read Heater Control Register */
#define SI7021_REG_HTRE_BIT         0x02    /**< Control Register Heater Bit */
#define SI7021_ID1_CMD              0xFA0F  /**< Read Electronic ID 1st Byte */
#define SI7021_ID2_CMD              0xFCC9  /**< Read Electronic ID 2nd Byte */
#define SI7021_FIRMVERS_CMD         0x84B8  /**< Read Firmware Revision */
#define SI7021_REV_1                0xFF    /**< Sensor revision 1 */
#define SI7021_REV_2                0x20    /**< Sensor revision 2 */

CSi7021Ctrl* CSi7021Ctrl::_instance = nullptr;

CSi7021Ctrl::CSi7021Ctrl()
{
    m_i2c_master = nullptr;
}

CSi7021Ctrl::~CSi7021Ctrl()
{

}

CSi7021Ctrl* CSi7021Ctrl::Instance()
{
    if (!_instance) {
        _instance = new CSi7021Ctrl();
    }

    return _instance;
}

bool CSi7021Ctrl::initialize(CI2CMaster *i2c_master)
{
    m_i2c_master = i2c_master;

    reset();
    vTaskDelay(50 / portTICK_PERIOD_MS);

    uint8_t revision = 0;
    read_revision(&revision);
    GetLogger(eLogType::Info)->Log("Revision: %u", revision);
    uint32_t serial_num_a = 0, serial_num_b = 0;
    read_serial_number(&serial_num_a, &serial_num_b);
    GetLogger(eLogType::Info)->Log("SNA: 0x%08X, SNB: 0x%08X", serial_num_a, serial_num_b);

    float temperature = 0.f, humidity = 0.f;
    read_temperature(&temperature);
    read_humidity(&humidity);
    GetLogger(eLogType::Info)->Log("Temperature: %g, Humidity: %g", temperature, humidity);

    GetLogger(eLogType::Info)->Log("Initialized");
    return true;
}

bool CSi7021Ctrl::release()
{
    return true;
}

bool CSi7021Ctrl::reset()
{
    if (!m_i2c_master) {
        GetLogger(eLogType::Error)->Log("I2C Controller is null");
        return false;
    }

    uint8_t data[1] = {SI7021_RESET_CMD};
    return m_i2c_master->write_bytes(SI7021_DEFAULT_ADDRESS, data, 1);
}

bool CSi7021Ctrl::read_temperature(float *temperature)
{
    if (!m_i2c_master) {
        GetLogger(eLogType::Error)->Log("I2C Controller is null");
        return false;
    }

    uint8_t data_write[1] = {SI7021_MEASTEMP_NOHOLD_CMD};
    m_i2c_master->write_bytes(SI7021_DEFAULT_ADDRESS, data_write, 1);
    vTaskDelay(20 / portTICK_PERIOD_MS);

    uint8_t data_read[3] = {0,};
    if (!m_i2c_master->read_bytes(SI7021_DEFAULT_ADDRESS, data_read, 3)) {
        return false;
    }
    uint16_t temp = data_read[0] << 8 | data_read[1];
    float conv = (float)temp * 175.72 / 65536. - 46.85;
    *temperature = conv;

    return true;
}

bool CSi7021Ctrl::read_humidity(float *humidity)
{
    if (!m_i2c_master) {
        GetLogger(eLogType::Error)->Log("I2C Controller is null");
        return false;
    }
    
    uint8_t data_write[1] = {SI7021_MEASRH_NOHOLD_CMD};
    m_i2c_master->write_bytes(SI7021_DEFAULT_ADDRESS, data_write, 1);
    vTaskDelay(20 / portTICK_PERIOD_MS);

    uint8_t data_read[3] = {0,};
    if (!m_i2c_master->read_bytes(SI7021_DEFAULT_ADDRESS, data_read, 3)) {
        return false;
    }
    uint16_t temp = data_read[0] << 8 | data_read[1];
    float conv = (float)temp * 125. / 65536. - 6.;
    *humidity = conv > 100.f? 100.f : conv;

    return true;
}

bool CSi7021Ctrl::read_revision(uint8_t *revision)
{
    if (!m_i2c_master) {
        GetLogger(eLogType::Error)->Log("I2C Controller is null");
        return false;
    }

    uint8_t data_write[2] = {(uint8_t)(SI7021_FIRMVERS_CMD >> 8), (uint8_t)(SI7021_FIRMVERS_CMD & 0xFF)};
    uint8_t data_read[1] = {0};
    if (!m_i2c_master->write_and_read_bytes(SI7021_DEFAULT_ADDRESS, data_write, 2, data_read, 1))
        return false;
    *revision = data_read[0];
    return true;
}

bool CSi7021Ctrl::read_serial_number(uint32_t *sna, uint32_t *snb)
{
    if (!m_i2c_master) {
        GetLogger(eLogType::Error)->Log("I2C Controller is null");
        return false;
    }

    uint8_t data_write[2] = {0, };
    uint8_t data_read[8] = {0, };

    data_write[0] = (uint8_t)(SI7021_ID1_CMD >> 8);
    data_write[1] = (uint8_t)(SI7021_ID1_CMD & 0xFF);
    if (!m_i2c_master->write_and_read_bytes(SI7021_DEFAULT_ADDRESS, data_write, 2, data_read, 8))
        return false;
    *sna = uint32_t(data_read[0]) << 24 | uint32_t(data_read[2]) << 16 | uint32_t(data_read[4]) << 8 | uint32_t(data_read[6]);

    data_write[0] = (uint8_t)(SI7021_ID2_CMD >> 8);
    data_write[1] = (uint8_t)(SI7021_ID2_CMD & 0xFF);
    if (!m_i2c_master->write_and_read_bytes(SI7021_DEFAULT_ADDRESS, data_write, 2, data_read, 6))
        return false;
    *snb = uint32_t(data_read[0]) << 24 | uint32_t(data_read[1]) << 16 | uint32_t(data_read[4]) << 8 | uint32_t(data_read[5]);

    return true;    
}