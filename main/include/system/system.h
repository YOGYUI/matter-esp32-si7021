#pragma once
#ifndef _SYSTEM_H_
#define _SYSTEM_H_

#include <esp_matter.h>
#include <esp_matter_core.h>
#include <iot_button.h>
#include "I2CMaster.h"

#ifdef __cplusplus
extern "C" {
#endif

class CSystem
{
public:
    CSystem();
    virtual ~CSystem();
    static CSystem* Instance();

public:
    bool initialize();
    void release();

private:
    static CSystem* _instance;
    CI2CMaster *m_i2c_master;
};

inline CSystem* GetSystem() {
    return CSystem::Instance();
}

#ifdef __cplusplus
};
#endif
#endif