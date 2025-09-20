#pragma once

#include <Arduino.h>
#include "internal/LinkBase.h"

namespace GSB {
  class GamepadLink : public internal::LinkBase {
    public:
      GamepadLink(LinkConfig linkConfig);
      ~GamepadLink();

      // Pass-through state setters (applied to specific controller)
      void SetButton(int gamepadIndex, ButtonID buttonID, bool pressed);
      void SetTrigger(int gamepadIndex, TriggerID triggerID, int value);
      void SetJoystick(int gamepadIndex, JoystickID joystickID, int xValue, int yValue);
      void SetJoystickX(int gamepadIndex, JoystickID joystickID, int value);
      void SetJoystickY(int gamepadIndex, JoystickID joystickID, int value);
      void SetSensor(int gamepadIndex, SensorID sensorID, int xValue, int yValue, int zValue);
      void SetSensorX(int gamepadIndex, SensorID sensorID, int value);
      void SetSensorY(int gamepadIndex, SensorID sensorID, int value);
      void SetSensorZ(int gamepadIndex, SensorID sensorID, int value);

    private:

  };
}