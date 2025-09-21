#pragma once

#include <Arduino.h>
#include "internal/LinkBase.h"

namespace GSB {
  class ApplicationLink : public internal::LinkBase {
    public:
      ApplicationLink(const LinkConfig& linkConfig) noexcept;
      ~ApplicationLink();

      // Pass-through callback setters (applied to all controllers)
      void SetButtonOnPress(void (*fxPtr)(uint8_t gamepadIndex, ButtonID buttonID));
      void SetButtonOnRelease(void (*fxPtr)(uint8_t gamepadIndex, ButtonID buttonID));
      void SetTriggerOnChange(void (*fxPtr)(uint8_t gamepadIndex, TriggerID triggerID, int16_t value));
      void SetJoystickOnChange(void (*fxPtr)(uint8_t gamepadIndex, JoystickID joystickID, int16_t xValue, int16_t yValue));
      void SetBatteryOnChange(void (*fxPtr)(uint8_t gamepadIndex, BatteryID batteryID, int8_t value));
      void SetSensorOnChange(void (*fxPtr)(uint8_t gamepadIndex, SensorID sensorID, int16_t xValue, int16_t yValue, int16_t zValue));

      // Per-controller tolerance configuration
      void SetTriggerTolerance(uint8_t gamepadIndex, TriggerID triggerID, uint16_t tolerance);
      void SetJoystickTolerance(uint8_t gamepadIndex, JoystickID joystickID, uint16_t xTolerance, uint16_t yTolerance);
      void SetJoystickToleranceX(uint8_t gamepadIndex, JoystickID joystickID, uint16_t tolerance);
      void SetJoystickToleranceY(uint8_t gamepadIndex, JoystickID joystickID, uint16_t tolerance);
      void SetBatteryTolerance(uint8_t gamepadIndex, BatteryID batteryID, uint8_t tolerance);
      void SetSensorTolerance(uint8_t gamepadIndex, SensorID sensorID, uint16_t xTolerance, uint16_t yTolerance, uint16_t zTolerance);
      void SetSensorToleranceX(uint8_t gamepadIndex, SensorID sensorID, uint16_t tolerance);
      void SetSensorToleranceY(uint8_t gamepadIndex, SensorID sensorID, uint16_t tolerance);
      void SetSensorToleranceZ(uint8_t gamepadIndex, SensorID sensorID, uint16_t tolerance);

      // Per-controller commands
      bool Rumble(uint8_t gamepadIndex, uint8_t force, uint8_t duration) noexcept;
      bool SetColorLED(uint8_t gamepadIndex, uint8_t r, uint8_t g, uint8_t b);
      bool SetPlayerLEDs(uint8_t gamepadIndex, uint8_t mask) noexcept;
      bool Disconnect(uint8_t gamepadIndex) noexcept;
    private:
      // Process status messages
      void ParseSerial(const uint8_t* data, size_t length) noexcept override;
      void ApplyStatus(const internal::Status& status);
      void HandleDPad(Gamepad& gamepad, uint8_t bitfield);
      void HandleMainButtons(Gamepad& gamepad, uint16_t bitfield);
      void HandleJoystick(Gamepad& gamepad, JoystickID joystickID, int16_t xValue, int16_t yValue);
      void HandleTrigger(Gamepad& gamepad, TriggerID triggerID, int16_t value);
      void HandleMiscButtons(Gamepad& gamepad, uint8_t bitfield);
      void HandleBattery(Gamepad& gamepad, BatteryID batteryID, uint8_t value);
      void HandleSensor(Gamepad& gamepad, SensorID sensorID, int16_t xValue, int16_t yValue, int16_t zValue);

      // Send command messages
      bool SendCommand(const internal::Command& command) noexcept;
  };
} // namespace GSB