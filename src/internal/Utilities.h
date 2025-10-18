#pragma once
#include <Arduino.h>

namespace GSB {
    namespace internal {
        inline void CopyBytes(uint8_t* destination, const uint8_t* source, size_t length) noexcept {
            for (size_t i = 0; i < length; ++i) {
                destination[i] = source[i];
            }
        }

        inline uint16_t ReadLE16(const uint8_t* buffer) noexcept {
            return static_cast<uint16_t>(buffer[0]) | (static_cast<uint16_t>(buffer[1]) << 8);
        }

        inline void WriteLE16(uint8_t* buffer, uint16_t value) noexcept {
            buffer[0] = static_cast<uint8_t>(value & 0xFF);
            buffer[1] = static_cast<uint8_t>((value >> 8) & 0xFF);
        }

        constexpr inline bool Uint8ToBool(uint8_t value) noexcept {
            return (value & 0x01u) != 0;
        }
    } // namespace internal
} // namespace GSB