#include "Gamepad.h"

namespace GSB {
  // ---------- ctor / callback setters ----------
  Gamepad::Gamepad()
    : m_buttonOnPress(nullptr),
      m_buttonOnRelease(nullptr),
      m_triggerOnChange(nullptr),
      m_joystickOnChange(nullptr),
      m_batteryOnChange(nullptr),
      m_sensorOnChange(nullptr),
      m_index(0) {

  }

  Gamepad::Gamepad(uint8_t index)
    : m_buttonOnPress(nullptr),
      m_buttonOnRelease(nullptr),
      m_triggerOnChange(nullptr),
      m_joystickOnChange(nullptr),
      m_batteryOnChange(nullptr),
      m_sensorOnChange(nullptr),
      m_index(index) {

  }

  // Inputs
  void Gamepad::SetBtnOnPress(void (*fxPtr)(uint8_t gamepadIndex, ButtonID buttonID)) {
    m_buttonOnPress = fxPtr;
  }

  void Gamepad::SetBtnOnRelease(void (*fxPtr)(uint8_t gamepadIndex, ButtonID buttonID)) {
    m_buttonOnRelease = fxPtr;
  }

  void Gamepad::SetTriggerOnChange(void (*fxPtr)(uint8_t gamepadIndex, TriggerID triggerID, int16_t value)) {
    m_triggerOnChange = fxPtr;
  }

  void Gamepad::SetJoystickOnChange(void (*fxPtr)(uint8_t gamepadIndex, JoystickID joystickID, int16_t xValue, int16_t yValue)) {
    m_joystickOnChange = fxPtr;
  }

  void Gamepad::SetBatteryOnChange(void (*fxPtr)(uint8_t gamepadIndex, BatteryID batteryID, uint8_t value)) {
    m_batteryOnChange = fxPtr;
  }

  void Gamepad::SetSensorOnChange(void (*fxPtr)(uint8_t gamepadIndex, SensorID sensorID, int16_t xValue, int16_t yValue, int16_t zValue)) {
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

  void Gamepad::SetTrigger(TriggerID triggerID, int16_t value) {
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

  void Gamepad::SetJoystick(JoystickID joystickID, int16_t xValue, int16_t yValue) {
    if (!IsValid(joystickID)) {
      return;
    }
    Joystick& joystick = GetJoystick(joystickID);
    if (joystick.SetValueX(xValue) || joystick.SetValueY(yValue)) {
      if (m_joystickOnChange) {
        m_joystickOnChange(m_index, joystickID, joystick.GetValueX(), joystick.GetValueY());
      }
    }
  }

  void Gamepad::SetJoystickX(JoystickID joystickID, int16_t value) {
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

  void Gamepad::SetJoystickY(JoystickID joystickID, int16_t value) {
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

  void Gamepad::SetBattery(BatteryID batteryID, uint8_t value) {
    if (!IsValid(batteryID)) {
      return;
    }
    Battery& battery = GetBattery(batteryID);
    if (battery.SetValue(value)) {
      if (m_batteryOnChange) {
        m_batteryOnChange(m_index, batteryID, battery.GetValue());
      }
    }
  }

  void Gamepad::SetSensor(SensorID sensorID, int16_t xValue, int16_t yValue, int16_t zValue) {
    if (!IsValid(sensorID)) {
      return;
    }
    Sensor& sensor = GetSensor(sensorID);
    if (sensor.SetValueX(xValue) || sensor.SetValueY(yValue) || sensor.SetValueZ(zValue)) {
      if (m_sensorOnChange) {
        m_sensorOnChange(m_index, sensorID, sensor.GetValueX(), sensor.GetValueY(), sensor.GetValueZ());
      }
    }
  }

  void Gamepad::SetSensorX(SensorID sensorID, int16_t value) {
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

  void Gamepad::SetSensorY(SensorID sensorID, int16_t value) {
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

  void Gamepad::SetSensorZ(SensorID sensorID, int16_t value) {
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
  void Gamepad::SetTriggerTolerance(TriggerID triggerID, uint16_t tolerance) {
    if (!IsValid(triggerID)) {
      return;
    }
    Trigger& trigger = GetTrigger(triggerID);
    trigger.SetTolerance(tolerance);
  }

  void Gamepad::SetJoystickTolerance(JoystickID joystickID, uint16_t xTolerance, uint16_t yTolerance) {
    if (!IsValid(joystickID)) {
      return;
    }
    SetJoystickToleranceX(joystickID, xTolerance);
    SetJoystickToleranceY(joystickID, yTolerance);
  }

  void Gamepad::SetJoystickToleranceX(JoystickID joystickID, uint16_t tolerance) {
    if (!IsValid(joystickID)) {
      return;
    }
    Joystick& joystick = GetJoystick(joystickID);
    joystick.SetToleranceX(tolerance);
  }

  void Gamepad::SetJoystickToleranceY(JoystickID joystickID, uint16_t tolerance) {
    if (!IsValid(joystickID)) {
      return;
    }
    Joystick& joystick = GetJoystick(joystickID);
    joystick.SetToleranceY(tolerance);
  }

  void Gamepad::SetBatteryTolerance(BatteryID batteryID, uint8_t tolerance) {
    if (!IsValid(batteryID)) {
      return;
    }
    Battery& battery = GetBattery(batteryID);
    battery.SetTolerance(tolerance);
  }

  void Gamepad::SetSensorTolerance(SensorID sensorID, uint16_t xTolerance, uint16_t yTolerance, uint16_t zTolerance) {
    if (!IsValid(sensorID)) {
      return;
    }
    SetSensorToleranceX(sensorID, xTolerance);
    SetSensorToleranceY(sensorID, yTolerance);
    SetSensorToleranceZ(sensorID, zTolerance);
  }

  void Gamepad::SetSensorToleranceX(SensorID sensorID, uint16_t tolerance) {
    if (!IsValid(sensorID)) {
      return;
    }
    Sensor& sensor = GetSensor(sensorID);
    sensor.SetToleranceX(tolerance);
  }

  void Gamepad::SetSensorToleranceY(SensorID sensorID, uint16_t tolerance) {
    if (!IsValid(sensorID)) {
      return;
    }
    Sensor& sensor = GetSensor(sensorID);
    sensor.SetToleranceY(tolerance);
  }

  void Gamepad::SetSensorToleranceZ(SensorID sensorID, uint16_t tolerance) {
    if (!IsValid(sensorID)) {
      return;
    }
    Sensor& sensor = GetSensor(sensorID);
    sensor.SetToleranceZ(tolerance);
  }

  // Outputs
  void Gamepad::SetRumbleOnChange(void (*fxPtr)(uint8_t gamepadIndex, RumbleID rumbleID, uint8_t force, uint8_t duration)) {
    m_rumbleOnChange = fxPtr;
  }

  void Gamepad::SetPlayerLedOnChange(void (*fxPtr)(uint8_t gamepadIndex, PlayerLedID playerLedID, bool illuminated)) {
    m_playerLedOnChange = fxPtr;
  }

  void Gamepad::SetColorLedOnChange(void (*fxPtr)(uint8_t gamepadIndex, ColorLedID colorLedID, uint8_t red, uint8_t green, uint8_t blue)) {
    m_colorLedOnChange = fxPtr;
  }

  void Gamepad::SetOnDisconnect(void (*fxPtr)(uint8_t gamepadIndex)) {
    m_onDisconnect = fxPtr;
  }

  void Gamepad::SetRumble(RumbleID rumbleID, uint8_t force, uint8_t duration) {
    if (!IsValid(rumbleID)) {
      return;
    }
    Rumble& rumble = GetRumble(rumbleID);
    if (rumble.Set(force, duration)) {
      if (m_rumbleOnChange) {
        m_rumbleOnChange(m_index, rumbleID, rumble.GetForce(), rumble.GetDuration());
      }
    }
  }

  void Gamepad::SetPlayerLeds(uint8_t player) {
    for(PlayerLed& playerLed : m_playerLeds) {
      if(playerLed.SetPlayer(player)) {
        if(m_playerLedOnChange) {
          PlayerLedID playerLedID = playerLed.GetID();
          if(!IsValid(playerLedID)) {
            continue;
          }
          m_playerLedOnChange(m_index, playerLedID, playerLed.GetIlluminated());
        }
      }
    }
  }

  void Gamepad::SetPlayerLed(PlayerLedID playerLedID, bool illuminated) {
    if (!IsValid(playerLedID)) {
      return;
    }
    PlayerLed& playerLed = GetPlayerLed(playerLedID);
    if (playerLed.Set(illuminated)) {
      if (m_playerLedOnChange) {
        m_playerLedOnChange(m_index, playerLedID, playerLed.GetIlluminated());
      }
    }
  }

  void Gamepad::TogglePlayerLed(PlayerLedID playerLedID) {
    if (!IsValid(playerLedID)) {
      return;
    }
    PlayerLed& playerLed = GetPlayerLed(playerLedID);
    if (playerLed.Toggle()) {
      if (m_playerLedOnChange) {
        m_playerLedOnChange(m_index, playerLedID, playerLed.GetIlluminated());
      }
    }
  }

  void Gamepad::SetColorLed(ColorLedID colorLedID, uint8_t red, uint8_t green, uint8_t blue) {
    if (!IsValid(colorLedID)) {
      return;
    }
    ColorLed& colorLed = GetColorLed(colorLedID);
    if (colorLed.SetColor(red, green, blue)) {
      if (m_colorLedOnChange) {
        internal::Color color = colorLed.GetColor();
        m_colorLedOnChange(m_index, colorLedID, color.red, color.green, color.blue);
      }
    }
  }

  void Gamepad::SetColorLed(ColorLedID colorLedID, internal::Color color) {
    if (!IsValid(colorLedID)) {
      return;
    }
    ColorLed& colorLed = GetColorLed(colorLedID);
    if (colorLed.SetColor(color)) {
      if (m_colorLedOnChange) {
        internal::Color color = colorLed.GetColor();
        m_colorLedOnChange(m_index, colorLedID, color.red, color.green, color.blue);
      }
    }
  }

  void Gamepad::ToggleColorLed(ColorLedID colorLedID) {
    if (!IsValid(colorLedID)) {
      return;
    }
    ColorLed& colorLed = GetColorLed(colorLedID);
    if (colorLed.Toggle()) {
      if (m_colorLedOnChange) {
        internal::Color color = colorLed.GetColor();
        m_colorLedOnChange(m_index, colorLedID, color.red, color.green, color.blue);
      }
    }
  }

  void Gamepad::Disconnect() {

  }


  // ---------- private accessors (use per-enum Index helpers) ----------
  // Inputs
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

  Battery& Gamepad::GetBattery(BatteryID batteryID) {
    return m_batteries[BatteryIndex(batteryID)];
  }
  
  const Battery& Gamepad::GetBattery(BatteryID batteryID) const {
    return m_batteries[BatteryIndex(batteryID)];
  }

  Sensor& Gamepad::GetSensor(SensorID sensorID) {
    return m_sensors[SensorIndex(sensorID)];
  }

  const Sensor& Gamepad::GetSensor(SensorID sensorID) const {
    return m_sensors[SensorIndex(sensorID)];
  }
} // namespace GSB
