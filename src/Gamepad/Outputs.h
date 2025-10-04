#pragma once

#include <Arduino.h>
#include "OutputIDs.h"

namespace GSB {
    class Rumble {
        public:
            Rumble() = default;
            void Set(uint8_t force, uint8_t duration);
            uint8_t GetForce() const;
            uint8_t GetDuration() const;
        
        private:
            uint8_t m_force{0};
            uint8_t m_duration{0};
    };

    class Led {
        public:
            Led() = default;
            bool Set(bool illuminated);
            void Toggle();
            bool GetIlluminated() const;
        
        protected:
            bool m_illuminated{false};
    };

    class PlayerLed : public Led {
        public:
            PlayerLed() = default;
            PlayerLed(PlayerLedID id);
            void SetID(PlayerLedID id);
            bool SetPlayer(uint8_t playerBitmask);
            PlayerLedID GetID() const;
        
        private:
            PlayerLedID m_id{PlayerLedID::PLAYER_LED_1};
    };

    struct Color {
        uint8_t red{0};
        uint8_t green{0};
        uint8_t blue{0};
    };

    class ColorLed : public Led {
        public:
            ColorLed() = default;
            bool SetColor(uint8_t red, uint8_t green, uint8_t blue);
            bool SetColor(Color color);
            Color GetColor() const;

        private:
            Color m_color{};
    };
} // namespace GSB