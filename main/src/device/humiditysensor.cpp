#include "humiditysensor.h"
#include "system.h"
#include "logger.h"
#include "si7021.h"

CHumiditySensor::CHumiditySensor()
{
    
}

bool CHumiditySensor::matter_add_endpoint()
{

    return CDevice::matter_add_endpoint();
}

bool CHumiditySensor::matter_init_endpoint()
{
    matter_update_all_attribute_values();

    return true;
}

void CHumiditySensor::matter_on_change_attribute_value(esp_matter::attribute::callback_type_t type, uint32_t cluster_id, uint32_t attribute_id, esp_matter_attr_val_t *value)
{

}

void CHumiditySensor::matter_update_all_attribute_values()
{
    
}