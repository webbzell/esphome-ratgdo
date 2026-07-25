#pragma once

#include "../ratgdo.h"
#include "esphome/components/text_sensor/text_sensor.h"
#include "esphome/core/component.h"

namespace esphome::ratgdo {

enum RATGDOTextSensorType : uint8_t {
    RATGDO_BUILD_INFO,
};

class RATGDOTextSensor : public text_sensor::TextSensor, public RATGDOClient, public Component {
public:
    void setup() override;
    void dump_config() override;
    void set_ratgdo_text_sensor_type(RATGDOTextSensorType type) { this->text_sensor_type_ = type; }

protected:
    RATGDOTextSensorType text_sensor_type_;
};

} // namespace esphome::ratgdo
