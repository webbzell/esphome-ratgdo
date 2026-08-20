#include "ratgdo_text_sensor.h"
#include "esphome/core/log.h"
#include <cstdio>

namespace esphome::ratgdo {

static const char* const TAG = "ratgdo.text_sensor";

void RATGDOTextSensor::setup()
{
    // subscribe() only fires on future changes, not the current value -
    // publish it once eagerly here so the entity isn't blank until the
    // next version query response (same pattern as the auto_close switch).
    auto publish_gdo_version = [this](uint16_t version) {
        if (version == GDO_VERSION_UNKNOWN) {
            this->publish_state("UNKNOWN");
            return;
        }
        char buf[8]; // "255.255" (7 chars) + null
        snprintf(buf, sizeof(buf), "%d.%d", (version >> 8) & 0xff, version & 0xff);
        this->publish_state(buf);
    };
    publish_gdo_version(*this->parent_->gdo_version);
    this->parent_->subscribe_gdo_version(publish_gdo_version);
}

void RATGDOTextSensor::dump_config()
{
    LOG_TEXT_SENSOR("", "RATGDO Text Sensor", this);
    ESP_LOGCONFIG(TAG, "  Type: GDO Version");
}

} // namespace esphome::ratgdo
