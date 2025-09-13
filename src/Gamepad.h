#pragma once
#include <Arduino.h>
#include "GamepadInputs.h"
#include "GamepadInputIDs.h"

namespace Gamepad {

class Gamepad {
  public:
    Gamepad();
    Gamepad(int index);

    void SetBtnOnPress(void (*fxPtr)(int gamepadIndex, ButtonID buttonID));
    void SetBtnOnRelease(void (*fxPtr)(int gamepadIndex, ButtonID buttonID));
    void SetTriggerOnChange(void (*fxPtr)(int gamepadIndex, TriggerID triggerID, int));
    void SetJoystickOnChange(void (*fxPtr)(int gamepadIndex, JoystickID joystickID, int, int));
    void SetSensorOnChange(void (*fxPtr)(int gamepadIndex, SensorID sensorID, int, int, int));

    void SetButton(ButtonID buttonID, bool pressed);
    void SetTrigger(TriggerID triggerID, int value);
    void SetJoystick(JoystickID joystickID, int xValue, int yValue);
    void SetJoystickX(JoystickID joystickID, int value);
    void SetJoystickY(JoystickID joystickID, int value);
    void SetSensor(SensorID sensorID, int xValue, int yValue, int zValue);
    void SetSensorX(SensorID sensorID, int value);
    void SetSensorY(SensorID sensorID, int value);
    void SetSensorZ(SensorID sensorID, int value);

    void SetTriggerTolerance(TriggerID triggerID, int tolerance);
    void SetJoystickTolerance(JoystickID joystickID, int xTolerance, int yTolerance);
    void SetJoystickToleranceX(JoystickID joystickID, int tolerance);
    void SetJoystickToleranceY(JoystickID joystickID, int tolerance);
    void SetSensorTolerance(SensorID sensorID, int xTolerance, int yTolerance, int zTolerance);
    void SetSensorToleranceX(SensorID sensorID, int tolerance);
    void SetSensorToleranceY(SensorID sensorID, int tolerance);
    void SetSensorToleranceZ(SensorID sensorID, int tolerance);

  private:
    Button& GetButton(ButtonID buttonID);
    const Button& GetButton(ButtonID buttonID) const;
    Trigger& GetTrigger(TriggerID triggerID);
    const Trigger& GetTrigger(TriggerID triggerID) const;
    Joystick& GetJoystick(JoystickID joystickID);
    const Joystick& GetJoystick(JoystickID joystickID) const;
    Sensor& GetSensor(SensorID sensorID);
    const Sensor& GetSensor(SensorID sensorID) const;

    void (*m_buttonOnPress)(int gamepadIndex, ButtonID buttonID);
    void (*m_buttonOnRelease)(int gamepadIndex, ButtonID buttonID);
    void (*m_triggerOnChange)(int gamepadIndex, TriggerID triggerID, int value);
    void (*m_joystickOnChange)(int gamepadIndex, JoystickID joystickID, int xValue, int yValue);
    void (*m_sensorOnChange)(int gamepadIndex, SensorID sensorID, int xValue, int yValue, int zValue);

    int m_index;

    Button m_buttons[ButtonCount() + 1];
    Trigger m_triggers[TriggerCount() + 1];
    Joystick m_joysticks[JoystickCount() + 1];
    Sensor m_sensors[SensorCount() + 1];
};

} // namespace Gamepad