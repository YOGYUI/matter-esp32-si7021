#include "system.h"
#include <nvs_flash.h>
#include "logger.h"
#include "definition.h"
#include "si7021.h"

CSystem* CSystem::_instance = nullptr;

CSystem::CSystem() 
{
    m_i2c_master = nullptr;
}

CSystem::~CSystem()
{
    if (_instance) {
        delete _instance;
        _instance = nullptr;
    }
}

CSystem* CSystem::Instance()
{
    if (!_instance) {
        _instance = new CSystem();
    }

    return _instance;
}

bool CSystem::initialize()
{
    GetLogger(eLogType::Info)->Log("Start Initializing System");
 
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    if (ret != ESP_OK) {
        GetLogger(eLogType::Error)->Log("Failed to initialize nsv flash (%d)", ret);
        return false;
    }

    m_i2c_master = GetI2CMaster();
    m_i2c_master->initialize(I2C_PORT_NUM, GPIO_PIN_I2C_SCL, GPIO_PIN_I2C_SDA, I2C_MASTER_FREQ);

    GetSi7021Ctrl()->initialize(m_i2c_master);
    return true;
}

void CSystem::release()
{
    
}