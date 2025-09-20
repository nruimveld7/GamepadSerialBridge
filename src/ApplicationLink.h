#pragma once

#include <Arduino.h>
#include "internal/LinkBase.h"

namespace GSB {
  class ApplicationLink : public internal::LinkBase {
    public:
      ApplicationLink(Config config);
      ~ApplicationLink();

      // Pass-through callback setters (applied to all controllers)
      void SetButtonOnPress(void (*fxPtr)(int gamepadIndex, ButtonID buttonID));
      void SetButtonOnRelease(void (*fxPtr)(int gamepadIndex, ButtonID buttonID));
      void SetTriggerOnChange(void (*fxPtr)(int gamepadIndex, TriggerID triggerID, int value));
      void SetJoystickOnChange(void (*fxPtr)(int gamepadIndex, JoystickID joystickID, int xValue, int yValue));
      void SetSensorOnChange(void (*fxPtr)(int gamepadIndex, SensorID sensorID, int xValue, int yValue, int zValue));

      // Per-controller tolerance configuration
      void SetTriggerTolerance(int gamepadIndex, TriggerID triggerID, int tolerance);
      void SetJoystickTolerance(int gamepadIndex, JoystickID joystickID, int xTolerance, int yTolerance);
      void SetJoystickToleranceX(int gamepadIndex, JoystickID joystickID, int tolerance);
      void SetJoystickToleranceY(int gamepadIndex, JoystickID joystickID, int tolerance);
      void SetSensorTolerance(int gamepadIndex, SensorID sensorID, int xTolerance, int yTolerance, int zTolerance);
      void SetSensorToleranceX(int gamepadIndex, SensorID sensorID, int tolerance);
      void SetSensorToleranceY(int gamepadIndex, SensorID sensorID, int tolerance);
      void SetSensorToleranceZ(int gamepadIndex, SensorID sensorID, int tolerance);

    private:
      void ParseBinary(const uint8_t* data, size_t length);
      void ParseASCII(const char* data, size_t length);
      void ApplyFrame(const FrameData& frame);
      void HandleDPad(Gamepad& gamepad, uint8_t bitfield);
      void HandleMainButtons(Gamepad& gamepad, uint16_t bitfield);
      void HandleJoystick(Gamepad& gamepad, JoystickID joystickID, int xValue, int yValue);
      void HandleTrigger(Gamepad& gamepad, TriggerID triggerID, int value);
      void HandleMiscButtons(Gamepad& gamepad, uint8_t bitfield);
      void HandleSensor(Gamepad& gamepad, SensorID sensorID, int xValue, int yValue, int zValue);
  };
}