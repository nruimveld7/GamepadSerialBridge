#pragma once

#include <Arduino.h>

namespace GSB {
    class Rumble {
        public:
            Rumble();
            bool Set(uint8_t force, uint8_t duration);
            uint8_t GetForce();
            uint8_t GetDuration();
        
        private:
            bool m_triggered{false};
            uint8_t m_force{0};
            uint8_t m_duration{0};
    };

    class Led {
        public:
            Led();
            bool Set(bool illuminated);
            bool Toggle();
            bool GetIlluminated();
        
            private:
                bool m_illuminated{false};
    };

    class PlayerLed : public Led {
        public:
            PlayerLed(uint8_t index);
            bool SetPlayer(uint8_t player);
        
        private:
            uint8_t m_index{0};
    }

    class ColorLed : public Led {
        public:
            namespace internal {
                struct Color {
                    uint8_t red{0};
                    uint8_t green{0};
                    uint8_t blue{0};
                };
            } // namepsace internal
            ColorLed();
            bool SetColor(uint8_t red, uint8_t green, uint8_t blue);
            bool SetColor(internal::Color color);
            internal::Color GetColor();

        private:
            internal::Color m_color;
    };

    

    class Disconnect {
        public:
            Disconnect();
            void SetDisconnect();
            bool Triggered();
        
        private:
            bool m_triggered{false};
    };
} // namespace GSB