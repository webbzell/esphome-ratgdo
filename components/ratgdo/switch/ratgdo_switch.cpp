#include "ratgdo_switch.h"
#include "../ratgdo_state.h"
#include "esphome/core/log.h"

namespace esphome::ratgdo {

static const char* const TAG = "ratgdo.switch";

void RATGDOSwitch::dump_config()
{
    LOG_SWITCH("", "RATGDO Switch", this);
    switch (this->switch_type_) {
    case SwitchType::RATGDO_LEARN:
        ESP_LOGCONFIG(TAG, "  Type: Learn");
        break;
    case SwitchType::RATGDO_LED:
        ESP_LOGCONFIG(TAG, "  Type: LED");
        break;
    case SwitchType::RATGDO_REVERSE_ENCODER:
        ESP_LOGCONFIG(TAG, "  Type: Reverse Encoder");
        break;
    case SwitchType::RATGDO_AUTO_CLOSE:
        ESP_LOGCONFIG(TAG, "  Type: Auto Close");
        break;
    default:
        break;
    }
}

void RATGDOSwitch::setup()
{
    switch (this->switch_type_) {
    case SwitchType::RATGDO_LEARN:
        this->parent_->subscribe_learn_state([this](LearnState state) {
            this->publish_state(state == LearnState::ACTIVE);
        });
        break;
    case SwitchType::RATGDO_LED:
        this->pin_->setup();
#ifdef RATGDO_USE_VEHICLE_SENSORS
        this->parent_->subscribe_vehicle_arriving_state([this](VehicleArrivingState state) {
            this->write_state(state == VehicleArrivingState::YES);
        });
#endif
        break;
#ifdef RATGDO_USE_ENCODER
    case SwitchType::RATGDO_REVERSE_ENCODER:
        this->pref_ = global_preferences->make_preference<bool>(fnv1_hash("ratgdo_reverse_encoder"));
        {
            bool stored = false;
            if (!this->pref_.load(&stored)) {
                ESP_LOGW(TAG, "Failed to load reverse_encoder preference. Defaulting to false.");
            }
            this->parent_->set_reverse_encoder(stored);
            this->publish_state(stored);
        }
        break;
#endif
    case SwitchType::RATGDO_AUTO_CLOSE:
        this->publish_state(false); // switch starts in off position
        this->parent_->subscribe_ttc_state([this](TtcState state) {
            this->publish_state(ttc_is_counting(state));
        });
        break;
    default:
        break;
    }
}

void RATGDOSwitch::write_state(bool state)
{
    switch (this->switch_type_) {
    case SwitchType::RATGDO_LEARN:
        if (state) {
            this->parent_->activate_learn();
        } else {
            this->parent_->inactivate_learn();
        }
        break;
    case SwitchType::RATGDO_LED:
        this->pin_->digital_write(state);
        this->publish_state(state);
        break;
#ifdef RATGDO_USE_ENCODER
    case SwitchType::RATGDO_REVERSE_ENCODER:
        if (!this->pref_.save(&state)) {
            ESP_LOGW(TAG, "Failed to save reverse_encoder preference.");
            return;
        }
        this->parent_->set_reverse_encoder(state);
        this->parent_->recalculate_encoder_state();
        this->publish_state(state);
        break;
#endif
    case SwitchType::RATGDO_AUTO_CLOSE: { // Brace block needed to scope ts; open is forced here by clang-format
        auto& ts = this->parent_->ttc_state;
        if (!state) { // User wants to turn switch off (HOLD)
            if (ttc_is_counting(*ts)) {
                this->parent_->ttc_toggle_hold();
            }
            // No else needed here because switch is already off and we're not counting.
        } else { // User wants to turn switch on (RELEASE)
            if (ttc_is_holding(*ts)) {
                this->parent_->ttc_toggle_hold();
            }
            if (ttc_is_off(*ts)) {
                // If TTC tracking is still off now, we need to correct the UI.
                // There are several reasons this can happen, but usually it is
                // because we were not holding when the UI was switched to ON.
                // Regardless of why, the UI (e.g. ESPHome web UI) has already
                // optimistically shown the switch as "on," so we need to fix that.
                // But just calling publish_state(false) here won't work
                // because internally the switch is still "false" and
                // the notification will be suppressed.
                // So the solution is to double publish, first to true (on) to match
                // the UI, then to false (off) to put it back.
                this->publish_state(true);
                this->publish_state(false);
            }
        }
    } break;
    default:
        break;
    }
}

} // namespace esphome::ratgdo
