#pragma once
#include <Arduino.h>

namespace GSB {
    namespace internal {
        struct Status {
            uint8_t gamepadIndex;
            uint8_t dpadMask;
            uint16_t mainButtonsMask;
            int16_t joystick1X;
            int16_t joystick1Y;
            int16_t joystick2X;
            int16_t joystick2Y;
            int16_t trigger1;
            int16_t trigger2;
            uint8_t miscButtonsMask;
            uint8_t battery;
            int16_t sensor1X;
            int16_t sensor1Y;
            int16_t sensor1Z;
            int16_t sensor2X;
            int16_t sensor2Y;
            int16_t sensor2Z;
        };
        static_assert(sizeof(Status) == 30, "Status must be 30 bytes");
    } // namepase internal
} // namespace GSB