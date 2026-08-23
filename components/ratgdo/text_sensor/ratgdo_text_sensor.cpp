#include "ratgdo_text_sensor.h"
#include "esphome/core/log.h"
#include "esphome/core/progmem.h"

namespace esphome::ratgdo {

static const char* const TAG = "ratgdo.text_sensor";

void RATGDOTextSensor::setup()
{
    switch (this->ratgdo_text_sensor_type_) {
    case RATGDOTextSensorType::RATGDO_TTC_STATE: {
        // subscribe() only fires on future changes, not the current value -
        // publish it once eagerly here so the entity isn't blank until the
        // next TTC transition (same pattern as the auto_close switch).
        auto publish_ttc_state = [this](TtcState state) {
            // TtcState_to_string() may return a PROGMEM-backed LogString on
            // ESP8266 (see macros.h) - copy it out via the PROGMEM-safe
            // helper rather than constructing a std::string directly from
            // the pointer, which would read flash unsafely on that platform.
            char buf[24]; // long enough for "INITIALIZING_DISABLED" (21 chars + null)
            ESPHOME_strncpy_P(buf, LOG_STR_ARG(TtcState_to_string(state)), sizeof(buf) - 1);
            buf[sizeof(buf) - 1] = '\0';
            this->publish_state(buf);
        };
        publish_ttc_state(*this->parent_->ttc_state);
        this->parent_->subscribe_ttc_state(publish_ttc_state);
        break;
    }
    default:
        break;
    }
}

void RATGDOTextSensor::dump_config()
{
    LOG_TEXT_SENSOR("", "RATGDO Text Sensor", this);
    switch (this->ratgdo_text_sensor_type_) {
    case RATGDOTextSensorType::RATGDO_TTC_STATE:
        ESP_LOGCONFIG(TAG, "  Type: TTC State");
        break;
    default:
        break;
    }
}

} // namespace esphome::ratgdo
