#include "Gamepad.h"

namespace GSB {
  // ---------- ctor / callback setters ----------
  Gamepad::Gamepad(uint8_t index)
    : m_buttonOnPress(nullptr),
      m_buttonOnRelease(nullptr),
      m_triggerOnChange(nullptr),
      m_joystickOnChange(nullptr),
      m_batteryOnChange(nullptr),
      m_sensorOnChange(nullptr),
      m_rumbleOnChange(nullptr),
      m_playerLedOnChange(nullptr),
      m_colorLedOnChange(nullptr),
      m_onDisconnect(nullptr),
      m_index(index) {
    m_status.gamepadIndex = m_index;
    for (uint8_t i = 0; i < PlayerLedCount(); ++i) {
      m_playerLeds[i].SetID(static_cast<PlayerLedID>(i));
    }
  }

  const internal::Status& Gamepad::GetStatus() const noexcept {
    return m_status;
  }

  internal::Status& Gamepad::GetStatus() noexcept {
    return m_status;
  }

  uint8_t Gamepad::GetIndex() const noexcept {
    return m_index;
  }

  // Inputs
  void Gamepad::SetButtonOnPress(void (*fxPtr)(uint8_t gamepadIndex, ButtonID buttonID)) {
    m_buttonOnPress = fxPtr;
  }

  void Gamepad::SetButtonOnRelease(void (*fxPtr)(uint8_t gamepadIndex, ButtonID buttonID)) {
    m_buttonOnRelease = fxPtr;
  }

  void Gamepad::SetTriggerOnChange(void (*fxPtr)(uint8_t gamepadIndex, TriggerID triggerID, int16_t value)) {
    m_triggerOnChange = fxPtr;
  }

  void Gamepad::SetJoystickOnChange(void (*fxPtr)(uint8_t gamepadIndex, JoystickID joystickID, int16_t valueX, int16_t valueY)) {
    m_joystickOnChange = fxPtr;
  }

  void Gamepad::SetBatteryOnChange(void (*fxPtr)(uint8_t gamepadIndex, BatteryID batteryID, uint8_t value)) {
    m_batteryOnChange = fxPtr;
  }

  void Gamepad::SetSensorOnChange(void (*fxPtr)(uint8_t gamepadIndex, SensorID sensorID, int16_t valueX, int16_t valueY, int16_t valueZ)) {
    m_sensorOnChange = fxPtr;
  }

  // ---------- state update methods (only fire on real changes) ----------
  void Gamepad::SetButton(ButtonID buttonID, bool pressed) {
    if (!IsValid(buttonID)) {
      return;
    }
    Button& button = GetButton(buttonID);
    if (button.SetPressed(pressed)) {
      m_status.Update(buttonID, pressed);
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
      m_status.Update(triggerID, value);
      if (m_triggerOnChange) {
        m_triggerOnChange(m_index, triggerID, trigger.GetValue());
      }
    }
  }

  void Gamepad::SetJoystick(JoystickID joystickID, int16_t valueX, int16_t valueY) {
    if (!IsValid(joystickID)) {
      return;
    }
    Joystick& joystick = GetJoystick(joystickID);
    if (joystick.SetValueX(valueX) || joystick.SetValueY(valueY)) {
      m_status.Update(joystickID, valueX, valueY);
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
      m_status.Update(joystickID, value, joystick.GetValueY());
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
      m_status.Update(joystickID, joystick.GetValueX(), value);
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
      m_status.Update(batteryID, value);
      if (m_batteryOnChange) {
        m_batteryOnChange(m_index, batteryID, battery.GetValue());
      }
    }
  }

  void Gamepad::SetSensor(SensorID sensorID, int16_t valueX, int16_t valueY, int16_t valueZ) {
    if (!IsValid(sensorID)) {
      return;
    }
    Sensor& sensor = GetSensor(sensorID);
    if (sensor.SetValueX(valueX) || sensor.SetValueY(valueY) || sensor.SetValueZ(valueZ)) {
      m_status.Update(sensorID, valueX, valueY, valueZ);
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
      m_status.Update(sensorID, value, sensor.GetValueY(), sensor.GetValueZ());
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
      m_status.Update(sensorID, sensor.GetValueX(), value, sensor.GetValueZ());
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
      m_status.Update(sensorID, sensor.GetValueX(), sensor.GetValueY(), value);
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

  void Gamepad::SetJoystickTolerance(JoystickID joystickID, uint16_t toleranceX, uint16_t toleranceY) {
    if (!IsValid(joystickID)) {
      return;
    }
    SetJoystickToleranceX(joystickID, toleranceX);
    SetJoystickToleranceY(joystickID, toleranceY);
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

  void Gamepad::SetSensorTolerance(SensorID sensorID, uint16_t toleranceX, uint16_t toleranceY, uint16_t toleranceZ) {
    if (!IsValid(sensorID)) {
      return;
    }
    SetSensorToleranceX(sensorID, toleranceX);
    SetSensorToleranceY(sensorID, toleranceY);
    SetSensorToleranceZ(sensorID, toleranceZ);
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

  void Gamepad::SetColorLedOnChange(void (*fxPtr)(uint8_t gamepadIndex, ColorLedID colorLedID, bool illuminated, uint8_t red, uint8_t green, uint8_t blue)) {
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
    rumble.Set(force, duration);
    if (m_rumbleOnChange) {
      m_rumbleOnChange(m_index, rumbleID, rumble.GetForce(), rumble.GetDuration());
    }
  }

  void Gamepad::SetPlayerLeds(uint8_t playerBitmask) {
    for (uint8_t i = 0; i < PlayerLedCount(); ++i) {
      PlayerLed& playerLed = m_playerLeds[i];
      if (playerLed.SetPlayer(playerBitmask) && m_playerLedOnChange) {
        m_playerLedOnChange(m_index, static_cast<PlayerLedID>(i), playerLed.GetIlluminated());
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
    playerLed.Toggle();
    if (m_playerLedOnChange) {
      m_playerLedOnChange(m_index, playerLedID, playerLed.GetIlluminated());
    }
  }

  void Gamepad::SetColorLed(ColorLedID colorLedID, bool illuminated, uint8_t red, uint8_t green, uint8_t blue) {
    if (!IsValid(colorLedID)) {
      return;
    }
    ColorLed& colorLed = GetColorLed(colorLedID);
    bool changed = colorLed.SetColor(red, green, blue);
    changed |= colorLed.Set(illuminated);
    if (changed) {
      if (m_colorLedOnChange) {
        Color color = colorLed.GetColor();
        m_colorLedOnChange(m_index, colorLedID, colorLed.GetIlluminated(), color.red, color.green, color.blue);
      }
    }
  }

  void Gamepad::SetColorLed(ColorLedID colorLedID, bool illuminated, Color color) {
    if (!IsValid(colorLedID)) {
      return;
    }
    ColorLed& colorLed = GetColorLed(colorLedID);
    bool changed = colorLed.SetColor(color);
    changed |= colorLed.Set(illuminated);
    if (changed) {
      if (m_colorLedOnChange) {
        color = colorLed.GetColor();
        m_colorLedOnChange(m_index, colorLedID, colorLed.GetIlluminated(), color.red, color.green, color.blue);
      }
    }
  }

  void Gamepad::ToggleColorLed(ColorLedID colorLedID) {
    if (!IsValid(colorLedID)) {
      return;
    }
    ColorLed& colorLed = GetColorLed(colorLedID);
    colorLed.Toggle();
    if (m_colorLedOnChange) {
      Color color = colorLed.GetColor();
      m_colorLedOnChange(m_index, colorLedID, colorLed.GetIlluminated(), color.red, color.green, color.blue);
    }
  }

  void Gamepad::SetDisconnect() {
    if (m_onDisconnect) {
      m_onDisconnect(m_index);
    }
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

  // Outputs
  Rumble& Gamepad::GetRumble(RumbleID rumbleID) {
    return m_rumbles[RumbleIndex(rumbleID)];
  }
  
  const Rumble& Gamepad::GetRumble(RumbleID rumbleID) const {
    return m_rumbles[RumbleIndex(rumbleID)];
  }

  PlayerLed& Gamepad::GetPlayerLed(PlayerLedID playerLedID) {
    return m_playerLeds[PlayerLedIndex(playerLedID)];
  }

  const PlayerLed& Gamepad::GetPlayerLed(PlayerLedID playerLedID) const {
    return m_playerLeds[PlayerLedIndex(playerLedID)];
  }

  ColorLed& Gamepad::GetColorLed(ColorLedID colorLedID) {
    return m_colorLeds[ColorLedIndex(colorLedID)];
  }

  const ColorLed& Gamepad::GetColorLed(ColorLedID colorLedID) const {
    return m_colorLeds[ColorLedIndex(colorLedID)];
  }
} // namespace GSB
