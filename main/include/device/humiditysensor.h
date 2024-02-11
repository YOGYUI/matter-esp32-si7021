#pragma once
#ifndef _HUMIDITY_SENSOR_H_
#define _HUMIDITY_SENSOR_H_

#include "device.h"

#ifdef __cplusplus
extern "C" {
#endif

class CHumiditySensor : public CDevice
{
public:
    CHumiditySensor();

    bool matter_add_endpoint() override;
    bool matter_init_endpoint() override;
    void matter_on_change_attribute_value(
        esp_matter::attribute::callback_type_t type,
        uint32_t cluster_id,
        uint32_t attribute_id,
        esp_matter_attr_val_t *value
    ) override;
    void matter_update_all_attribute_values() override;
};

#ifdef __cplusplus
};
#endif



#endif