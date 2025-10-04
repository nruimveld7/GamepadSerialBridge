#pragma once
#include <Arduino.h>

namespace GSB {
    enum class RumbleID : uint8_t {
        RUMBLE_1,
        COUNT
    };

    enum class PlayerLedID : uint8_t {
        PLAYER_LED_1,
        PLAYER_LED_2,
        PLAYER_LED_3,
        PLAYER_LED_4,
        COUNT
    };

    enum class ColorLedID : uint8_t {
        COLOR_LED_1,
        COUNT
    };

    // Small helpers for sizing/indices
    constexpr uint8_t RumbleCount() {
        return static_cast<uint8_t>(RumbleID::COUNT);
    }

    constexpr uint8_t RumbleIndex(RumbleID id) {
        return (static_cast<uint8_t>(id) < RumbleCount()) ? static_cast<uint8_t>(id) : RumbleCount();
    }

    inline bool IsValid(RumbleID id) {
        return id < RumbleID::COUNT;
    }

    constexpr uint8_t PlayerLedCount() {
        return static_cast<uint8_t>(PlayerLedID::COUNT);
    }

    constexpr uint8_t PlayerLedIndex(PlayerLedID id) {
        return (static_cast<uint8_t>(id) < PlayerLedCount()) ? static_cast<uint8_t>(id) : PlayerLedCount();
    }

    inline bool IsValid(PlayerLedID id) {
        return id < PlayerLedID::COUNT;
    }

    constexpr uint8_t ColorLedCount() {
        return static_cast<uint8_t>(ColorLedID::COUNT);
    }

    constexpr uint8_t ColorLedIndex(ColorLedID id) {
        return (static_cast<uint8_t>(id) < ColorLedCount()) ? static_cast<uint8_t>(id) : ColorLedCount();
    }

    inline bool IsValid(ColorLedID id) {
        return id < ColorLedID::COUNT;
    }
} // namespace GSB