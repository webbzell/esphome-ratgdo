#pragma once

#include "../ratgdo.h"
#include "../ratgdo_state.h"
#include "esphome/components/text_sensor/text_sensor.h"
#include "esphome/core/component.h"

namespace esphome::ratgdo {

class RATGDOTextSensor : public text_sensor::TextSensor, public RATGDOClient, public Component {
public:
    void dump_config() override;
    void setup() override;
};

} // namespace esphome::ratgdo
