#include "ratgdo_text_sensor.h"
#include "esphome/core/helpers.h"
#include "esphome/core/log.h"

namespace esphome::ratgdo {

static const char* const TAG = "ratgdo.text_sensor";

void RATGDOTextSensor::setup()
{
    switch (this->text_sensor_type_) {
    case RATGDOTextSensorType::RATGDO_BUILD_INFO:
        this->publish_state(str_sprintf("git=%s yaml=%s",
            RATGDO_BUILD_GIT_HASH, RATGDO_BUILD_YAML));
        break;
    default:
        break;
    }
}

void RATGDOTextSensor::dump_config()
{
    LOG_TEXT_SENSOR("", "RATGDO Text Sensor", this);
    switch (this->text_sensor_type_) {
    case RATGDOTextSensorType::RATGDO_BUILD_INFO:
        ESP_LOGCONFIG(TAG, "  Type: Firmware Build Source");
        break;
    default:
        break;
    }
}

} // namespace esphome::ratgdo
