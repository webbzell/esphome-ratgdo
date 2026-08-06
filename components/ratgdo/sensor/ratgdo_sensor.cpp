#include "ratgdo_sensor.h"
#include "../ratgdo_state.h"
#include "esphome/core/controller_registry.h"
#include "esphome/core/log.h"

namespace esphome::ratgdo {

static const char* const TAG = "ratgdo.sensor";

void RATGDOSensor::setup()
{
    switch (this->ratgdo_sensor_type_) {
    case RATGDOSensorType::RATGDO_OPENINGS:
        this->parent_->subscribe_openings([this](uint16_t value) {
            this->publish_state(value);
        });
        break;
    case RATGDOSensorType::RATGDO_PAIRED_DEVICES_TOTAL:
        this->parent_->subscribe_paired_devices_total([this](uint8_t value) {
            this->publish_state(value);
        });
        break;
    case RATGDOSensorType::RATGDO_PAIRED_REMOTES:
        this->parent_->subscribe_paired_remotes([this](uint8_t value) {
            this->publish_state(value);
        });
        break;
    case RATGDOSensorType::RATGDO_PAIRED_KEYPADS:
        this->parent_->subscribe_paired_keypads([this](uint8_t value) {
            this->publish_state(value);
        });
        break;
    case RATGDOSensorType::RATGDO_PAIRED_WALL_CONTROLS:
        this->parent_->subscribe_paired_wall_controls([this](uint8_t value) {
            this->publish_state(value);
        });
        break;
    case RATGDOSensorType::RATGDO_PAIRED_ACCESSORIES:
        this->parent_->subscribe_paired_accessories([this](uint8_t value) {
            this->publish_state(value);
        });
        break;
    case RATGDOSensorType::RATGDO_ENCODER:
#ifdef RATGDO_USE_ENCODER
        this->parent_->set_encoder_sensor(this);
#endif
        break;
    case RATGDOSensorType::RATGDO_TTC_COUNTDOWN:
        this->parent_->subscribe_ttc_countdown([this](uint16_t seconds) {
            if (ttc_is_counting(*this->parent_->ttc_state)) {
                this->publish_state(seconds);
            } else {
                this->publish_unavailable();
            }
        });
        break;
    case RATGDOSensorType::RATGDO_TTC_LIMIT:
        this->parent_->subscribe_ttc_limit([this](uint16_t seconds) {
            this->publish_state(seconds);
        });
        break;
    default:
        break;
    }
}

void RATGDOSensor::dump_config()
{
    LOG_SENSOR("", "RATGDO Sensor", this);
    switch (this->ratgdo_sensor_type_) {
    case RATGDOSensorType::RATGDO_OPENINGS:
        ESP_LOGCONFIG(TAG, "  Type: Openings");
        break;
    case RATGDOSensorType::RATGDO_PAIRED_DEVICES_TOTAL:
        ESP_LOGCONFIG(TAG, "  Type: Paired Devices");
        break;
    case RATGDOSensorType::RATGDO_PAIRED_REMOTES:
        ESP_LOGCONFIG(TAG, "  Type: Paired Remotes");
        break;
    case RATGDOSensorType::RATGDO_PAIRED_KEYPADS:
        ESP_LOGCONFIG(TAG, "  Type: Paired Keypads");
        break;
    case RATGDOSensorType::RATGDO_PAIRED_WALL_CONTROLS:
        ESP_LOGCONFIG(TAG, "  Type: Paired Wall Controls");
        break;
    case RATGDOSensorType::RATGDO_PAIRED_ACCESSORIES:
        ESP_LOGCONFIG(TAG, "  Type: Paired Accessories");
        break;
    case RATGDOSensorType::RATGDO_ENCODER:
        ESP_LOGCONFIG(TAG, "  Type: Encoder");
        break;
    case RATGDOSensorType::RATGDO_TTC_COUNTDOWN:
        ESP_LOGCONFIG(TAG, "  Type: TTC Countdown");
        break;
    case RATGDOSensorType::RATGDO_TTC_LIMIT:
        ESP_LOGCONFIG(TAG, "  Type: TTC Limit");
        break;
    default:
        break;
    }
}

// Special method to mark this sensor "unavailable" so that it displays as
// - "NA" in the ESPHome Web UI
// - "Unknown" in Home Assistant
//
// publish_state() can't be used for this because internally it calls
// set_has_state(true).
//
// Mimics internal_send_state_to_frontend(), but sets has_state false first.
// Order matters here because notify_sensor_update() needs that set first.
void RATGDOSensor::publish_unavailable()
{
    this->state = NAN;
    this->set_has_state(false);
    this->callback_.call(NAN);
    ControllerRegistry::notify_sensor_update(this);
}

} // namespace esphome::ratgdo
