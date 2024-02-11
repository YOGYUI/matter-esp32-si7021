#pragma once
#ifndef _SI7021_H_
#define _SI7021_H_

#include "I2CMaster.h"

#ifdef __cplusplus
extern "C" {
#endif

class CSi7021Ctrl
{
public:
    CSi7021Ctrl();
    virtual ~CSi7021Ctrl();
    static CSi7021Ctrl* Instance();

public:
    bool initialize(CI2CMaster *i2c_master);
    bool release();

    bool reset();
    bool read_temperature(float *temperature);
    bool read_humidity(float *humidity);

private:
    static CSi7021Ctrl *_instance;
    CI2CMaster *m_i2c_master;

    bool read_revision(uint8_t *revision);
    bool read_serial_number(uint32_t *sna, uint32_t *snb);
};

inline CSi7021Ctrl* GetSi7021Ctrl() {
    return CSi7021Ctrl::Instance();
}

#ifdef __cplusplus
}
#endif
#endif