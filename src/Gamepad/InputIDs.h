#pragma once
#include <Arduino.h>

namespace GSB {
    enum class ButtonID : uint8_t {
        DPAD_1,
        DPAD_2,
        DPAD_3,
        DPAD_4,
        MAIN_1,
        MAIN_2,
        MAIN_3,
        MAIN_4,
        MAIN_5,
        MAIN_6,
        MAIN_7,
        MAIN_8,
        MAIN_9,
        MAIN_10,
        MAIN_11,
        MAIN_12,
        MAIN_13,
        MAIN_14,
        MAIN_15,
        MAIN_16,
        MISC_1,
        MISC_2,
        MISC_3,
        MISC_4,
        COUNT
    };

    enum class TriggerID : uint8_t {
        TRIGGER_1,
        TRIGGER_2,
        COUNT
    };

    enum class JoystickID : uint8_t {
        JOYSTICK_1,
        JOYSTICK_2,
        COUNT
    };

    enum class BatteryID : uint8_t {
        BATTERY_1,
        COUNT
    };

    enum class SensorID : uint8_t {
        SENSOR_1,
        SENSOR_2,
        COUNT
    };

    // Small helpers for sizing/indices
    constexpr uint8_t ButtonCount() {
        return static_cast<uint8_t>(ButtonID::COUNT);
    }

    constexpr uint8_t ButtonIndex(ButtonID id) {
        return (static_cast<uint8_t>(id) < ButtonCount()) ? static_cast<uint8_t>(id) : ButtonCount();
    }

    inline bool IsValid(ButtonID id) {
        return id < ButtonID::COUNT;
    }

    constexpr uint8_t TriggerCount() {
        return static_cast<uint8_t>(TriggerID::COUNT);
    }

    constexpr uint8_t TriggerIndex(TriggerID id) {
        return (static_cast<uint8_t>(id) < TriggerCount()) ? static_cast<uint8_t>(id) : TriggerCount();
    }

    inline bool IsValid(TriggerID id) {
        return id < TriggerID::COUNT;
    }

    constexpr uint8_t JoystickCount() {
        return static_cast<uint8_t>(JoystickID::COUNT);
    }

    constexpr uint8_t JoystickIndex(JoystickID id) {
        return (static_cast<uint8_t>(id) < JoystickCount()) ? static_cast<uint8_t>(id) : JoystickCount();
    }

    inline bool IsValid(JoystickID id) {
        return id < JoystickID::COUNT;
    }

    constexpr uint8_t BatteryCount() {
        return static_cast<uint8_t>(BatteryID::COUNT);
    }

    constexpr uint8_t BatteryIndex(BatteryID id) {
        return (static_cast<uint8_t>(id) < BatteryCount()) ? static_cast<uint8_t>(id) : BatteryCount();
    }

    inline bool IsValid(BatteryID id) {
        return id < BatteryID::COUNT;
    }

    constexpr uint8_t SensorCount() {
        return static_cast<uint8_t>(SensorID::COUNT);
    }

    constexpr uint8_t SensorIndex(SensorID id) {
        return (static_cast<uint8_t>(id) < SensorCount()) ? static_cast<uint8_t>(id) : SensorCount();
    }

    inline bool IsValid(SensorID id) {
        return id < SensorID::COUNT;
    }

    struct DPadButtons {
        static constexpr ButtonID buttonIDs[] = {
            ButtonID::DPAD_1,
            ButtonID::DPAD_2,
            ButtonID::DPAD_3,
            ButtonID::DPAD_4,
        };
        
        static constexpr uint8_t Count() {
            return sizeof(buttonIDs) / sizeof(buttonIDs[0]);
        }
    };

    struct MainButtons {
        static constexpr ButtonID buttonIDs[] = {
            ButtonID::MAIN_1,
            ButtonID::MAIN_2,
            ButtonID::MAIN_3,
            ButtonID::MAIN_4,
            ButtonID::MAIN_5,
            ButtonID::MAIN_6,
            ButtonID::MAIN_7,
            ButtonID::MAIN_8,
            ButtonID::MAIN_9,
            ButtonID::MAIN_10,
            ButtonID::MAIN_11,
            ButtonID::MAIN_12,
            ButtonID::MAIN_13,
            ButtonID::MAIN_14,
            ButtonID::MAIN_15,
            ButtonID::MAIN_16,
        };

        static constexpr uint8_t Count() {
            return sizeof(buttonIDs) / sizeof(buttonIDs[0]);
        }
    };

    struct MiscButtons {
        static constexpr ButtonID buttonIDs[] = {
            ButtonID::MISC_1,
            ButtonID::MISC_2,
            ButtonID::MISC_3,
            ButtonID::MISC_4,
        };

        static constexpr uint8_t Count() {
            return sizeof(buttonIDs) / sizeof(buttonIDs[0]);
        }
    };
} // namespace GSB