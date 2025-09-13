#include "Gamepad.h"
#include "GamepadInputIDs.h"  // for ButtonIndex/TriggerIndex/...

namespace Gamepad {

// ---------- ctor / callback setters ----------

Gamepad::Gamepad()
  : m_buttonOnPress(nullptr),
    m_buttonOnRelease(nullptr),
    m_triggerOnChange(nullptr),
    m_joystickOnChange(nullptr),
    m_sensorOnChange(nullptr),
    m_index(-1) {

}

Gamepad::Gamepad(int index)
  : m_buttonOnPress(nullptr),
    m_buttonOnRelease(nullptr),
    m_triggerOnChange(nullptr),
    m_joystickOnChange(nullptr),
    m_sensorOnChange(nullptr),
    m_index(index) {

}

void Gamepad::SetBtnOnPress(void (*fxPtr)(int gamepadIndex, ButtonID buttonID)) {
  m_buttonOnPress = fxPtr;
}

void Gamepad::SetBtnOnRelease(void (*fxPtr)(int gamepadIndex, ButtonID buttonID)) {
  m_buttonOnRelease = fxPtr;
}

void Gamepad::SetTriggerOnChange(void (*fxPtr)(int gamepadIndex, TriggerID triggerID, int value)) {
  m_triggerOnChange = fxPtr;
}

void Gamepad::SetJoystickOnChange(void (*fxPtr)(int gamepadIndex, JoystickID joystickID, int xValue, int yValue)) {
  m_joystickOnChange = fxPtr;
}

void Gamepad::SetSensorOnChange(void (*fxPtr)(int gamepadIndex, SensorID sensorID, int xValue, int yValue, int zValue)) {
  m_sensorOnChange = fxPtr;
}

// ---------- state update methods (only fire on real changes) ----------

void Gamepad::SetButton(ButtonID buttonID, bool pressed) {
  if (!IsValid(buttonID)) {
    return;
  }
  Button& button = GetButton(buttonID);
  if (button.SetPressed(pressed)) {
    if (pressed) {
      if (m_buttonOnPress) {
        m_buttonOnPress(m_index, buttonID);
      }
    } else {
      if (m_buttonOnRelease) {
        m_buttonOnRelease(m_index, buttonID);
      }
    }
  }
}

void Gamepad::SetTrigger(TriggerID triggerID, int value) {
  if (!IsValid(triggerID)) {
    return;
  }
  Trigger& trigger = GetTrigger(triggerID);
  if (trigger.SetValue(value)) {
    if (m_triggerOnChange) {
      m_triggerOnChange(m_index, triggerID, trigger.GetValue());
    }
  }
}

void Gamepad::SetJoystick(JoystickID joystickID, int xValue, int yValue) {
  if (!IsValid(joystickID)) {
    return;
  }
  Joystick& joystick = GetJoystick(joystickID);
  bool validX = joystick.SetValueX(xValue);
  bool validY = joystick.SetValueY(yValue);
  if (validX || validY) {
    if (m_joystickOnChange) {
      m_joystickOnChange(m_index, joystickID, joystick.GetValueX(), joystick.GetValueY());
    }
  }
}

void Gamepad::SetJoystickX(JoystickID joystickID, int value) {
  if (!IsValid(joystickID)) {
    return;
  }
  Joystick& joystick = GetJoystick(joystickID);
  if (joystick.SetValueX(value)) {
    if (m_joystickOnChange) {
      m_joystickOnChange(m_index, joystickID, joystick.GetValueX(), joystick.GetValueY());
    }
  }
}

void Gamepad::SetJoystickY(JoystickID joystickID, int value) {
  if (!IsValid(joystickID)) {
    return;
  }
  Joystick& joystick = GetJoystick(joystickID);
  if (joystick.SetValueY(value)) {
    if (m_joystickOnChange) {
      m_joystickOnChange(m_index, joystickID, joystick.GetValueX(), joystick.GetValueY());
    }
  }
}

void Gamepad::SetSensor(SensorID sensorID, int xValue, int yValue, int zValue) {
  if (!IsValid(sensorID)) {
    return;
  }
  Sensor& sensor = GetSensor(sensorID);
  bool validX = sensor.SetValueX(xValue);
  bool validY = sensor.SetValueY(yValue);
  bool validZ = sensor.SetValueZ(zValue);
  if (validX || validY || validZ) {
    if (m_sensorOnChange) {
      m_sensorOnChange(m_index, sensorID, sensor.GetValueX(), sensor.GetValueY(), sensor.GetValueZ());
    }
  }
}

void Gamepad::SetSensorX(SensorID sensorID, int value) {
  if (!IsValid(sensorID)) {
    return;
  }
  Sensor& sensor = GetSensor(sensorID);
  if (sensor.SetValueX(value)) {
    if (m_sensorOnChange) {
      m_sensorOnChange(m_index, sensorID, sensor.GetValueX(), sensor.GetValueY(), sensor.GetValueZ());
    }
  }
}

void Gamepad::SetSensorY(SensorID sensorID, int value) {
  if (!IsValid(sensorID)) {
    return;
  }
  Sensor& sensor = GetSensor(sensorID);
  if (sensor.SetValueY(value)) {
    if (m_sensorOnChange) {
      m_sensorOnChange(m_index, sensorID, sensor.GetValueX(), sensor.GetValueY(), sensor.GetValueZ());
    }
  }
}

void Gamepad::SetSensorZ(SensorID sensorID, int value) {
  if (!IsValid(sensorID)) {
    return;
  }
  Sensor& sensor = GetSensor(sensorID);
  if (sensor.SetValueZ(value)) {
    if (m_sensorOnChange) {
      m_sensorOnChange(m_index, sensorID, sensor.GetValueX(), sensor.GetValueY(), sensor.GetValueZ());
    }
  }
}

// ---------- tolerance update methods ----------

void Gamepad::SetTriggerTolerance(TriggerID triggerID, int tolerance) {
  if (!IsValid(triggerID)) {
    return;
  }
  Trigger& trigger = GetTrigger(triggerID);
  trigger.SetTolerance(tolerance);
}

void Gamepad::SetJoystickTolerance(JoystickID joystickID, int xTolerance, int yTolerance) {
  if (!IsValid(joystickID)) {
    return;
  }
  SetJoystickToleranceX(joystickID, xTolerance);
  SetJoystickToleranceY(joystickID, yTolerance);
}

void Gamepad::SetJoystickToleranceX(JoystickID joystickID, int tolerance) {
  if (!IsValid(joystickID)) {
    return;
  }
  Joystick& joystick = GetJoystick(joystickID);
  joystick.SetToleranceX(tolerance);
}

void Gamepad::SetJoystickToleranceY(JoystickID joystickID, int tolerance) {
  if (!IsValid(joystickID)) {
    return;
  }
  Joystick& joystick = GetJoystick(joystickID);
  joystick.SetToleranceY(tolerance);
}

void Gamepad::SetSensorTolerance(SensorID sensorID, int xTolerance, int yTolerance, int zTolerance) {
  if (!IsValid(sensorID)) {
    return;
  }
  SetSensorToleranceX(sensorID, xTolerance);
  SetSensorToleranceY(sensorID, yTolerance);
  SetSensorToleranceZ(sensorID, zTolerance);
}

void Gamepad::SetSensorToleranceX(SensorID sensorID, int tolerance) {
  if (!IsValid(sensorID)) {
    return;
  }
  Sensor& sensor = GetSensor(sensorID);
  sensor.SetToleranceX(tolerance);
}

void Gamepad::SetSensorToleranceY(SensorID sensorID, int tolerance) {
  if (!IsValid(sensorID)) {
    return;
  }
  Sensor& sensor = GetSensor(sensorID);
  sensor.SetToleranceY(tolerance);
}

void Gamepad::SetSensorToleranceZ(SensorID sensorID, int tolerance) {
  if (!IsValid(sensorID)) {
    return;
  }
  Sensor& sensor = GetSensor(sensorID);
  sensor.SetToleranceZ(tolerance);
}

// ---------- private accessors (use per-enum Index helpers) ----------

Button& Gamepad::GetButton(ButtonID buttonID) {
  return m_buttons[ButtonIndex(buttonID)];
}

const Button& Gamepad::GetButton(ButtonID buttonID) const {
  return m_buttons[ButtonIndex(buttonID)];
}

Trigger& Gamepad::GetTrigger(TriggerID triggerID) {
  return m_triggers[TriggerIndex(triggerID)];
}

const Trigger& Gamepad::GetTrigger(TriggerID triggerID) const {
  return m_triggers[TriggerIndex(triggerID)];
}

Joystick& Gamepad::GetJoystick(JoystickID joystickID) {
  return m_joysticks[JoystickIndex(joystickID)];
}

const Joystick& Gamepad::GetJoystick(JoystickID joystickID) const {
  return m_joysticks[JoystickIndex(joystickID)];
}

Sensor& Gamepad::GetSensor(SensorID sensorID) {
  return m_sensors[SensorIndex(sensorID)];
}

const Sensor& Gamepad::GetSensor(SensorID sensorID) const {
  return m_sensors[SensorIndex(sensorID)];
}

} // namespace Gamepad
