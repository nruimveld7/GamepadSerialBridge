#pragma once
#include <Arduino.h>
#include "internal/Utilities.h"
#include "Gamepad/InputIDs.h"

namespace GSB {
    namespace internal {
        struct Status {
            public:
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
                uint8_t battery1;
                int16_t sensor1X;
                int16_t sensor1Y;
                int16_t sensor1Z;
                int16_t sensor2X;
                int16_t sensor2Y;
                int16_t sensor2Z;

                static constexpr size_t MaximumLength() noexcept {
                    return m_size;
                }

                size_t Serialize(uint8_t* out, size_t outCapacity) const noexcept {
                    if (out == nullptr) {
                        return 0;
                    }
                    if (outCapacity < m_size) {
                        return 0;
                    }
                    out[0]  = gamepadIndex;
                    out[1]  = dpadMask;
                    WriteLE16(out + 2, mainButtonsMask);
                    WriteLE16(out + 4, joystick1X);
                    WriteLE16(out + 6, joystick1Y);
                    WriteLE16(out + 8, joystick2X);
                    WriteLE16(out + 10, joystick2Y);
                    WriteLE16(out + 12, trigger1);
                    WriteLE16(out + 14, trigger2);
                    out[16] = miscButtonsMask;
                    out[17] = battery1;
                    WriteLE16(out + 18, sensor1X);
                    WriteLE16(out + 20, sensor1Y);
                    WriteLE16(out + 22, sensor1Z);
                    WriteLE16(out + 24, sensor2X);
                    WriteLE16(out + 26, sensor2Y);
                    WriteLE16(out + 28, sensor2Z);
                    return m_size;
                }

                static bool Deserialize(const uint8_t* in, size_t length, Status& out) noexcept {
                    if (in == nullptr) {
                        return false;
                    }
                    if (length != m_size) {
                        return false;
                    }
                    Status status{};
                    status.gamepadIndex = in[0];
                    status.dpadMask = in[1];
                    status.mainButtonsMask = ReadLE16(in + 2);
                    status.joystick1X = static_cast<int16_t>(ReadLE16(in + 4));
                    status.joystick1Y = static_cast<int16_t>(ReadLE16(in + 6));
                    status.joystick2X = static_cast<int16_t>(ReadLE16(in + 8));
                    status.joystick2Y = static_cast<int16_t>(ReadLE16(in + 10));
                    status.trigger1 = static_cast<int16_t>(ReadLE16(in + 12));
                    status.trigger2 = static_cast<int16_t>(ReadLE16(in + 14));
                    status.miscButtonsMask = in[16];
                    status.battery1 = in[17];
                    status.sensor1X = static_cast<int16_t>(ReadLE16(in + 18));
                    status.sensor1Y = static_cast<int16_t>(ReadLE16(in + 20));
                    status.sensor1Z = static_cast<int16_t>(ReadLE16(in + 22));
                    status.sensor2X = static_cast<int16_t>(ReadLE16(in + 24));
                    status.sensor2Y = static_cast<int16_t>(ReadLE16(in + 26));
                    status.sensor2Z = static_cast<int16_t>(ReadLE16(in + 28));
                    out = status;
                    return true;
                }

                void Update(ButtonID buttonID, bool pressed) noexcept {
                    uint8_t index = 0;
                    if (DPadButtons::FindButton(buttonID, index)) {
                        SetBit(dpadMask, index, pressed);
                        return;
                    }
                    if (MainButtons::FindButton(buttonID, index)) {
                        SetBit(mainButtonsMask, index, pressed);
                        return;
                    }
                    if (MiscButtons::FindButton(buttonID, index)) {
                        SetBit(miscButtonsMask, index, pressed);
                        return;
                    }
                    // Unknown button -> no-op
                }

                void Update(JoystickID joystickID, int16_t valueX, int16_t valueY) noexcept {
                    switch (joystickID) {
                        case JoystickID::JOYSTICK_1:
                            joystick1X = valueX;
                            joystick1Y = valueY;
                            break;
                        case JoystickID::JOYSTICK_2:
                            joystick2X = valueX;
                            joystick2Y = valueY;
                            break;
                        default:
                            // Unknown joystick -> no-op
                            break;
                    }
                }

                void Update(TriggerID triggerID, int16_t value) noexcept {
                    switch (triggerID) {
                        case TriggerID::TRIGGER_1:
                            trigger1 = value;
                            break;
                        case TriggerID::TRIGGER_2:
                            trigger2 = value;
                            break;
                        default:
                            // Unknown trigger -> no-op
                            break;
                    }
                }

                void Update(BatteryID batteryID, uint8_t value) noexcept {
                    switch (batteryID) {
                        case BatteryID::BATTERY_1:
                            battery1 = value;
                            break;
                        default:
                            // Unknown battery -> no-op
                            break;
                    }
                }

                void Update(SensorID sensorID, int16_t valueX, int16_t valueY, int16_t valueZ) noexcept {
                    switch (sensorID) {
                        case SensorID::SENSOR_1:
                            sensor1X = valueX;
                            sensor1Y = valueY;
                            sensor1Z = valueZ;
                            break;
                        case SensorID::SENSOR_2:
                            sensor2X = valueX;
                            sensor2Y = valueY;
                            sensor2Z = valueZ;
                            break;
                        default:
                            // Unknown sensor -> no-op
                            break;
                    }
                }
            
            private:
                static inline void SetBit(uint8_t& mask, uint8_t bit, bool on) noexcept {
                    const uint8_t modifier = static_cast<uint8_t>(1u << bit);
                    mask = on ? static_cast<uint8_t>(mask | modifier) : static_cast<uint8_t>(mask & static_cast<uint8_t>(~modifier));
                }
                static inline void SetBit(uint16_t& mask, uint8_t bit, bool on) noexcept {
                    const uint16_t modifier = static_cast<uint16_t>(1u << bit);
                    mask = on ? static_cast<uint16_t>(mask | modifier) : static_cast<uint16_t>(mask & static_cast<uint16_t>(~modifier));
                }
               static constexpr size_t m_size = 30;
        };
        static_assert(sizeof(Status) == Status::m_size, "Status must be 30 bytes");
    } // namepase internal
    static_assert(DPadButtons::Count() <= 8, "DPad must fit in 8 bits");
    static_assert(MainButtons::Count() <= 16, "Main must fit in 16 bits");
    static_assert(MiscButtons::Count() <= 8, "Misc must fit in 8 bits");
    static_assert(static_cast<uint8_t>(ButtonID::COUNT) == GSB::DPadButtons::Count() + GSB::MainButtons::Count() + GSB::MiscButtons::Count(), "ButtonID::COUNT must equal sum of subset counts");
} // namespace GSB