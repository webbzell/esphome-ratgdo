#include "ratgdo_text_sensor.h"
#include "esphome/core/log.h"

namespace esphome::ratgdo {

static const char* const TAG = "ratgdo.text_sensor";

void RATGDOTextSensor::setup()
{
    switch (this->text_sensor_type_) {
    case RATGDOTextSensorType::RATGDO_GIT_VERSION:
        this->publish_state(RATGDO_BUILD_GIT_HASH);
        break;
    case RATGDOTextSensorType::RATGDO_FIRMWARE_YAML:
        this->publish_state(RATGDO_BUILD_YAML);
        break;
    default:
        break;
    }
}

void RATGDOTextSensor::dump_config()
{
    LOG_TEXT_SENSOR("", "RATGDO Text Sensor", this);
    switch (this->text_sensor_type_) {
    case RATGDOTextSensorType::RATGDO_GIT_VERSION:
        ESP_LOGCONFIG(TAG, "  Type: Git Version");
        break;
    case RATGDOTextSensorType::RATGDO_FIRMWARE_YAML:
        ESP_LOGCONFIG(TAG, "  Type: Firmware YAML");
        break;
    default:
        break;
    }
}

} // namespace esphome::ratgdo
