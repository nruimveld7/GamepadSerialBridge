#pragma once
#include <Arduino.h>
#include "Gamepad/Inputs.h"
#include "Gamepad/InputIDs.h"
#include "Gamepad/Outputs.h"
#include "Gamepad/OutputIDs.h"
#include "internal/Status.h"

namespace GSB {
  class Gamepad {
    public:
      Gamepad() = delete;
      explicit Gamepad(uint8_t index) noexcept;

      Gamepad(const Gamepad&) = delete;
      Gamepad& operator=(const Gamepad&) = delete;
      Gamepad(Gamepad&&) = delete;
      Gamepad& operator=(Gamepad&&) = delete;

      const internal::Status& GetStatus() const noexcept;
      internal::Status& GetStatus() noexcept;
      uint8_t GetIndex() const noexcept;

      //Inputs
      void SetButtonOnPress(void (*fxPtr)(uint8_t gamepadIndex, ButtonID buttonID));
      void SetButtonOnRelease(void (*fxPtr)(uint8_t gamepadIndex, ButtonID buttonID));
      void SetTriggerOnChange(void (*fxPtr)(uint8_t gamepadIndex, TriggerID triggerID, int16_t value));
      void SetJoystickOnChange(void (*fxPtr)(uint8_t gamepadIndex, JoystickID joystickID, int16_t valueX, int16_t valueY));
      void SetBatteryOnChange(void (*fxPtr)(uint8_t gamepadIndex, BatteryID batteryID, uint8_t value));
      void SetSensorOnChange(void (*fxPtr)(uint8_t gamepadIndex, SensorID sensorID, int16_t valueX, int16_t valueY, int16_t valueZ));

      void SetButton(ButtonID buttonID, bool pressed);
      void SetTrigger(TriggerID triggerID, int16_t value);
      void SetJoystick(JoystickID joystickID, int16_t valueX, int16_t valueY);
      void SetJoystickX(JoystickID joystickID, int16_t value);
      void SetJoystickY(JoystickID joystickID, int16_t value);
      void SetBattery(BatteryID batteryID, uint8_t value);
      void SetSensor(SensorID sensorID, int16_t valueX, int16_t valueY, int16_t valueZ);
      void SetSensorX(SensorID sensorID, int16_t value);
      void SetSensorY(SensorID sensorID, int16_t value);
      void SetSensorZ(SensorID sensorID, int16_t value);

      void SetTriggerTolerance(TriggerID triggerID, uint16_t tolerance);
      void SetJoystickTolerance(JoystickID joystickID, uint16_t toleranceX, uint16_t toleranceY);
      void SetJoystickToleranceX(JoystickID joystickID, uint16_t tolerance);
      void SetJoystickToleranceY(JoystickID joystickID, uint16_t tolerance);
      void SetBatteryTolerance(BatteryID batteryID, uint8_t tolerance);
      void SetSensorTolerance(SensorID sensorID, uint16_t toleranceX, uint16_t toleranceY, uint16_t toleranceZ);
      void SetSensorToleranceX(SensorID sensorID, uint16_t tolerance);
      void SetSensorToleranceY(SensorID sensorID, uint16_t tolerance);
      void SetSensorToleranceZ(SensorID sensorID, uint16_t tolerance);

      //Outputs
      void SetOnDisconnect(void (*fxPtr)(uint8_t gamepadIndex));
      void SetRumbleOnChange(void (*fxPtr)(uint8_t gamepadIndex, RumbleID rumbleID, uint8_t force, uint8_t duration));
      void SetPlayerLedOnChange(void (*fxPtr)(uint8_t gamepadIndex, PlayerLedID playerLedID, bool illuminated));
      void SetColorLedOnChange(void (*fxPtr)(uint8_t gamepadIndex, ColorLedID colorLedID, bool illuminated, uint8_t red, uint8_t green, uint8_t blue));

      void SetDisconnect();
      void SetRumble(RumbleID rumbleID, uint8_t force, uint8_t duration);
      void SetPlayerLeds(uint8_t playerBitmask);
      void SetPlayerLed(PlayerLedID playerLedID, bool illuminated);
      void TogglePlayerLed(PlayerLedID playerLedID);
      void SetColorLed(ColorLedID colorLedID, bool illuminated, uint8_t red, uint8_t green, uint8_t blue);
      void SetColorLed(ColorLedID colorLedID, bool illuminated, Color color);
      void ToggleColorLed(ColorLedID colorLedID);

    private:
      uint8_t m_index;
      internal::Status m_status{};

      //Inputs
      Button& GetButton(ButtonID buttonID);
      const Button& GetButton(ButtonID buttonID) const;
      Trigger& GetTrigger(TriggerID triggerID);
      const Trigger& GetTrigger(TriggerID triggerID) const;
      Joystick& GetJoystick(JoystickID joystickID);
      const Joystick& GetJoystick(JoystickID joystickID) const;
      Battery& GetBattery(BatteryID batteryID);
      const Battery& GetBattery(BatteryID batteryID) const;
      Sensor& GetSensor(SensorID sensorID);
      const Sensor& GetSensor(SensorID sensorID) const;

      void (*m_buttonOnPress)(uint8_t gamepadIndex, ButtonID buttonID);
      void (*m_buttonOnRelease)(uint8_t gamepadIndex, ButtonID buttonID);
      void (*m_triggerOnChange)(uint8_t gamepadIndex, TriggerID triggerID, int16_t value);
      void (*m_joystickOnChange)(uint8_t gamepadIndex, JoystickID joystickID, int16_t valueX, int16_t valueY);
      void (*m_batteryOnChange)(uint8_t gamepadIndex, BatteryID batteryID, uint8_t value);
      void (*m_sensorOnChange)(uint8_t gamepadIndex, SensorID sensorID, int16_t valueX, int16_t valueY, int16_t valueZ);

      Button m_buttons[ButtonCount() + 1];
      Trigger m_triggers[TriggerCount() + 1];
      Joystick m_joysticks[JoystickCount() + 1];
      Battery m_batteries[BatteryCount() + 1];
      Sensor m_sensors[SensorCount() + 1];

      //Outputs
      Rumble& GetRumble(RumbleID rumbleID);
      const Rumble& GetRumble(RumbleID rumbleID) const;
      PlayerLed& GetPlayerLed(PlayerLedID playerLedID);
      const PlayerLed& GetPlayerLed(PlayerLedID playerLedID) const;
      ColorLed& GetColorLed(ColorLedID colorLedID);
      const ColorLed& GetColorLed(ColorLedID colorLedID) const;

      void (*m_rumbleOnChange)(uint8_t gamepadIndex, RumbleID rumbleID, uint8_t force, uint8_t duration);
      void (*m_playerLedOnChange)(uint8_t gamepadIndex, PlayerLedID playerLedID, bool illuminated);
      void (*m_colorLedOnChange)(uint8_t gamepadIndex, ColorLedID colorLedID, bool illuminated, uint8_t red, uint8_t green, uint8_t blue);
      void (*m_onDisconnect)(uint8_t gamepadIndex);

      Rumble m_rumbles[RumbleCount() + 1];
      PlayerLed m_playerLeds[PlayerLedCount() + 1];
      ColorLed m_colorLeds[ColorLedCount() + 1];
  };
} // namespace GSB