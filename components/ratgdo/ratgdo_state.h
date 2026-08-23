/************************************
 * Rage
 * Against
 * The
 * Garage
 * Door
 * Opener
 *
 * Copyright (C) 2022  Paul Wieland
 *
 * GNU GENERAL PUBLIC LICENSE
 ************************************/

#pragma once
#include "esphome/core/defines.h"
#include "macros.h"
#include <cstdint>

namespace esphome::ratgdo {

ENUM(DoorState, uint8_t,
    (UNKNOWN, 0),
    (OPEN, 1),
    (CLOSED, 2),
    (STOPPED, 3),
    (OPENING, 4),
    (CLOSING, 5))

ENUM(DoorActionDelayed, uint8_t,
    (NO, 0),
    (YES, 1))

/// Enum for all states a the light can be in.
ENUM(LightState, uint8_t,
    (OFF, 0),
    (ON, 1),
    (UNKNOWN, 2))
LightState light_state_toggle(LightState state);

/// Enum for all states a the lock can be in.
ENUM(LockState, uint8_t,
    (UNLOCKED, 0),
    (LOCKED, 1),
    (UNKNOWN, 2))
LockState lock_state_toggle(LockState state);

/// MotionState for all states a the motion can be in.
ENUM(MotionState, uint8_t,
    (CLEAR, 0),
    (DETECTED, 1),
    (UNKNOWN, 2))

/// Enum for all states a the obstruction can be in.
ENUM(ObstructionState, uint8_t,
    (OBSTRUCTED, 0),
    (CLEAR, 1),
    (UNKNOWN, 2))

/// Enum for all states a the motor can be in.
ENUM(MotorState, uint8_t,
    (OFF, 0),
    (ON, 1),
    (UNKNOWN, 2))

ENUM(ManuallyOperatedState, uint8_t,
    (NO, 0),
    (YES, 1))

/// Enum for all states the button can be in.
ENUM(ButtonState, uint8_t,
    (PRESSED, 0),
    (RELEASED, 1),
    (UNKNOWN, 2))

ENUM_SPARSE(BatteryState, uint8_t,
    (UNKNOWN, 0),
    (CHARGING, 0x6),
    (FULL, 0x8))

/// Enum for learn states.
ENUM(LearnState, uint8_t,
    (INACTIVE, 0),
    (ACTIVE, 1),
    (UNKNOWN, 2))
LearnState learn_state_toggle(LearnState state);

ENUM(PairedDevice, uint8_t,
    (ALL, 0),
    (REMOTE, 1),
    (KEYPAD, 2),
    (WALL_CONTROL, 3),
    (ACCESSORY, 4),
    (UNKNOWN, 0xff))

// actions
ENUM(LightAction, uint8_t,
    (OFF, 0),
    (ON, 1),
    (TOGGLE, 2),
    (UNKNOWN, 3))

ENUM(LockAction, uint8_t,
    (UNLOCK, 0),
    (LOCK, 1),
    (TOGGLE, 2),
    (UNKNOWN, 3))

ENUM(DoorAction, uint8_t,
    (CLOSE, 0),
    (OPEN, 1),
    (TOGGLE, 2),
    (STOP, 3),
    (UNKNOWN, 4))

#ifdef RATGDO_USE_VEHICLE_SENSORS
ENUM(VehicleDetectedState, uint8_t,
    (NO, 0),
    (YES, 1))

ENUM(VehicleArrivingState, uint8_t,
    (NO, 0),
    (YES, 1))

ENUM(VehicleLeavingState, uint8_t,
    (NO, 0),
    (YES, 1))
#endif

struct Openings {
    uint16_t count;
    uint8_t flag;
};

struct PairedDeviceCount {
    PairedDevice kind;
    uint8_t count;
};

struct TtcLimit {
    uint16_t seconds;
};

struct TtcCountdown {
    uint16_t seconds;
};

// Raw byte1 from a TTC_ACTION message, not yet mapped to a meaning.
struct TtcAction {
    uint8_t value;
};

// Raw byte1 from a TTC_STATE message, not yet mapped to TtcState.
struct TtcStateMsg {
    uint8_t value;
};

ENUM(TtcState, uint8_t,
    (UNKNOWN, 0),
    (ENABLED_READY, 1),
    (ENABLED_COUNTING, 2),
    (ENABLED_HOLDING, 3),
    (DISABLED, 4),
    (INITIALIZING_ENABLED, 5),
    (INITIALIZING_DISABLED, 6),
    (CLOSING_ALERT, 7)) // countdown ended; light-flash/beeper warning before the door actually closes

// True when TTC's state hasn't been learned yet this power cycle.
inline constexpr bool ttc_is_unknown(TtcState state)
{
    return state == TtcState::UNKNOWN;
}

// True when TTC is enabled but not currently active: the door isn't open,
// no broadcast has been seen yet this cycle, or the watchdog assumed comms
// failure and gave up.
inline constexpr bool ttc_is_ready(TtcState state)
{
    return state == TtcState::ENABLED_READY;
}

// True when counting down
inline constexpr bool ttc_is_counting(TtcState state)
{
    return state == TtcState::ENABLED_COUNTING;
}

// True when the door is holding open (and the countdown can be restarted).
inline constexpr bool ttc_is_holding(TtcState state)
{
    return state == TtcState::ENABLED_HOLDING;
}

// True when TTC is in one of the ENABLED_* states (ready, counting, or
// holding) - i.e. the GDO has confirmed a real, actionable state.
inline constexpr bool ttc_is_enabled(TtcState state)
{
    return ttc_is_ready(state) || ttc_is_counting(state) || ttc_is_holding(state);
}

// True when the GDO hasn't confirmed a real TTC state yet - a fault/pending
// signal (see INITIALIZING_ENABLED/INITIALIZING_DISABLED in secplus2.h), not a state to act
// on. TTC_ACTION commands sent while in this state are silently dropped by
// the GDO, so nothing here should attempt to change TTC state - only ever
// reflect/reject.
inline constexpr bool ttc_is_initializing(TtcState state)
{
    return state == TtcState::INITIALIZING_ENABLED || state == TtcState::INITIALIZING_DISABLED;
}

} // namespace esphome::ratgdo
