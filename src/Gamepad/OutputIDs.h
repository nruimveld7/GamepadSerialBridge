#pragma once
#include <Arduino.h>

namespace GSB {
    enum class RumbleID : uint8_t {
        RUMBLE_1,
        COUNT
    }

    enum class PlayerLedID : uint8_t {
        PLAYER_LED_1,
        PLAYER_LED_2,
        PLAYER_LED_3,
        PLAYER_LED_4,
        COUNT
    }

    enum class ColorLedID : uint8_t {
        COLOR_LED_1,
        COUNT
    }

    enum class DisconnectID : uint8_t {

    }
} // namespace GSB