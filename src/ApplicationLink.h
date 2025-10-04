#pragma once

#include <Arduino.h>
#include "internal/LinkBase.h"
#include "internal/Command.h"

namespace GSB {
  class ApplicationLink : public internal::LinkBase {
    public:
      ApplicationLink(const LinkConfig& linkConfig) noexcept;
      ~ApplicationLink() noexcept = default;

      // ──────────────────────────────
      // Callbacks (applied to all gamepads)
      // ──────────────────────────────
      void SetButtonOnPress(void (*fxPtr)(uint8_t gamepadIndex, ButtonID buttonID)) noexcept;
      void SetButtonOnRelease(void (*fxPtr)(uint8_t gamepadIndex, ButtonID buttonID)) noexcept;
      void SetTriggerOnChange(void (*fxPtr)(uint8_t gamepadIndex, TriggerID triggerID, int16_t value)) noexcept;
      void SetJoystickOnChange(void (*fxPtr)(uint8_t gamepadIndex, JoystickID joystickID, int16_t valueX, int16_t valueY)) noexcept;
      void SetBatteryOnChange(void (*fxPtr)(uint8_t gamepadIndex, BatteryID batteryID, uint8_t value)) noexcept;
      void SetSensorOnChange(void (*fxPtr)(uint8_t gamepadIndex, SensorID sensorID, int16_t valueX, int16_t valueY, int16_t valueZ)) noexcept;

      // ──────────────────────────────
      // TOLERANCES
      // ──────────────────────────────
      // All inputs on all gamepads (set defaults across the board)
      void SetTriggerToleranceForAllGamepads(uint16_t tolerance) noexcept;
      void SetJoystickToleranceForAllGamepads(uint16_t toleranceX, uint16_t toleranceY) noexcept;
      void SetJoystickToleranceXForAllGamepads(uint16_t tolerance) noexcept;
      void SetJoystickToleranceYForAllGamepads(uint16_t tolerance) noexcept;
      void SetBatteryToleranceForAllGamepads(uint8_t tolerance) noexcept;
      void SetSensorToleranceForAllGamepads(uint16_t toleranceX, uint16_t toleranceY, uint16_t toleranceZ) noexcept;
      void SetSensorToleranceXForAllGamepads(uint16_t tolerance) noexcept;
      void SetSensorToleranceYForAllGamepads(uint16_t tolerance) noexcept;
      void SetSensorToleranceZForAllGamepads(uint16_t tolerance) noexcept;

      // One input on all gamepads
      void SetTriggerToleranceForAllGamepads(TriggerID triggerID, uint16_t tolerance) noexcept;
      void SetJoystickToleranceForAllGamepads(JoystickID joystickID, uint16_t toleranceX, uint16_t toleranceY) noexcept;
      void SetJoystickToleranceXForAllGamepads(JoystickID joystickID, uint16_t tolerance) noexcept;
      void SetJoystickToleranceYForAllGamepads(JoystickID joystickID, uint16_t tolerance) noexcept;
      void SetBatteryToleranceForAllGamepads(BatteryID batteryID, uint8_t tolerance) noexcept;
      void SetSensorToleranceForAllGamepads(SensorID sensorID, uint16_t toleranceX, uint16_t toleranceY, uint16_t toleranceZ) noexcept;
      void SetSensorToleranceXForAllGamepads(SensorID sensorID, uint16_t tolerance) noexcept;
      void SetSensorToleranceYForAllGamepads(SensorID sensorID, uint16_t tolerance) noexcept;
      void SetSensorToleranceZForAllGamepads(SensorID sensorID, uint16_t tolerance) noexcept;

      // All inputs on one gamepad
      void SetTriggerTolerance(uint8_t gamepadIndex, uint16_t tolerance) noexcept;
      void SetJoystickTolerance(uint8_t gamepadIndex, uint16_t toleranceX, uint16_t toleranceY) noexcept;
      void SetJoystickToleranceX(uint8_t gamepadIndex, uint16_t tolerance) noexcept;
      void SetJoystickToleranceY(uint8_t gamepadIndex, uint16_t tolerance) noexcept;
      void SetBatteryTolerance(uint8_t gamepadIndex, uint8_t tolerance) noexcept;
      void SetSensorTolerance(uint8_t gamepadIndex, uint16_t toleranceX, uint16_t toleranceY, uint16_t toleranceZ) noexcept;
      void SetSensorToleranceX(uint8_t gamepadIndex, uint16_t tolerance) noexcept;
      void SetSensorToleranceY(uint8_t gamepadIndex, uint16_t tolerance) noexcept;
      void SetSensorToleranceZ(uint8_t gamepadIndex, uint16_t tolerance) noexcept;

      // One input on one gamepad
      void SetTriggerTolerance(uint8_t gamepadIndex, TriggerID triggerID, uint16_t tolerance) noexcept;
      void SetJoystickTolerance(uint8_t gamepadIndex, JoystickID joystickID, uint16_t toleranceX, uint16_t toleranceY) noexcept;
      void SetJoystickToleranceX(uint8_t gamepadIndex, JoystickID joystickID, uint16_t tolerance) noexcept;
      void SetJoystickToleranceY(uint8_t gamepadIndex, JoystickID joystickID, uint16_t tolerance) noexcept;
      void SetBatteryTolerance(uint8_t gamepadIndex, BatteryID batteryID, uint8_t tolerance) noexcept;
      void SetSensorTolerance(uint8_t gamepadIndex, SensorID sensorID, uint16_t toleranceX, uint16_t toleranceY, uint16_t toleranceZ) noexcept;
      void SetSensorToleranceX(uint8_t gamepadIndex, SensorID sensorID, uint16_t tolerance) noexcept;
      void SetSensorToleranceY(uint8_t gamepadIndex, SensorID sensorID, uint16_t tolerance) noexcept;
      void SetSensorToleranceZ(uint8_t gamepadIndex, SensorID sensorID, uint16_t tolerance) noexcept;

      // ──────────────────────────────
      // COMMANDS
      // ──────────────────────────────
      // All outputs on all gamepads
      bool DisconnectAllGamepads() noexcept;
      bool StartRumbleForAllGamepads(uint8_t force, uint8_t duration) noexcept;
      bool StopRumbleForAllGamepads() noexcept;
      bool SetPlayerLedsForAllGamepads(bool illuminated) noexcept;
      bool SetPlayerLedsForAllGamepads(uint8_t mask) noexcept;
      bool TogglePlayerLedsForAllGamepads() noexcept;
      bool TogglePlayerLedsForAllGamepads(uint8_t mask) noexcept;
      bool SetColorLedsForAllGamepads(bool illuminated) noexcept;
      bool SetColorLedsForAllGamepads(bool illuminated, uint8_t red, uint8_t green, uint8_t blue) noexcept;
      bool ToggleColorLedsForAllGamepads() noexcept;

      // One output on all gamepads
      bool StartRumbleForAllGamepads(RumbleID rumbleID, uint8_t force, uint8_t duration) noexcept;
      bool StopRumbleForAllGamepads(RumbleID rumbleID) noexcept;
      bool SetPlayerLedForAllGamepads(PlayerLedID playerLedID, bool illuminated) noexcept;
      bool TogglePlayerLedForAllGamepads(PlayerLedID playerLedID) noexcept;
      bool SetColorLedForAllGamepads(ColorLedID colorLedID, bool illuminated) noexcept;
      bool SetColorLedForAllGamepads(ColorLedID colorLedID, bool illuminated, uint8_t red, uint8_t green, uint8_t blue) noexcept;
      bool ToggleColorLedForAllGamepads(ColorLedID colorLedID) noexcept;


      // All outputs on one gamepad
      bool Disconnect(uint8_t gamepadIndex) noexcept;
      bool StartRumble(uint8_t gamepadIndex, uint8_t force, uint8_t duration) noexcept;
      bool StopRumble(uint8_t gamepadIndex) noexcept;
      bool SetPlayerLeds(uint8_t gamepadIndex, bool illuminated) noexcept;
      bool SetPlayerLeds(uint8_t gamepadIndex, uint8_t mask) noexcept;
      bool TogglePlayerLeds(uint8_t gamepadIndex) noexcept;
      bool TogglePlayerLeds(uint8_t gamepadIndex, uint8_t mask) noexcept;
      bool SetColorLeds(uint8_t gamepadIndex, bool illuminated) noexcept;
      bool SetColorLeds(uint8_t gamepadIndex, bool illuminated, uint8_t red, uint8_t green, uint8_t blue) noexcept;
      bool ToggleColorLeds(uint8_t gamepadIndex) noexcept;

      // One output on one gamepad
      bool StartRumble(uint8_t gamepadIndex, RumbleID rumbleID, uint8_t force, uint8_t duration) noexcept;
      bool StopRumble(uint8_t gamepadIndex, RumbleID rumbleID) noexcept;
      bool SetPlayerLed(uint8_t gamepadIndex, PlayerLedID playerLedID, bool illuminated) noexcept;
      bool TogglePlayerLed(uint8_t gamepadIndex, PlayerLedID playerLedID) noexcept;
      bool SetColorLed(uint8_t gamepadIndex, ColorLedID colorLedID, bool illuminated) noexcept;
      bool SetColorLed(uint8_t gamepadIndex, ColorLedID colorLedID, bool illuminated, uint8_t red, uint8_t green, uint8_t blue) noexcept;
      bool ToggleColorLed(uint8_t gamepadIndex, ColorLedID colorLedID) noexcept;

    private:
      // Process status messages
      void ParseSerial(const uint8_t* data, size_t length) noexcept override;
      void ApplyStatus(const internal::Status& status);
      void HandleDPad(Gamepad& gamepad, uint8_t bitfield);
      void HandleMainButtons(Gamepad& gamepad, uint16_t bitfield);
      void HandleJoystick(Gamepad& gamepad, JoystickID joystickID, int16_t valueX, int16_t valueY);
      void HandleTrigger(Gamepad& gamepad, TriggerID triggerID, int16_t value);
      void HandleMiscButtons(Gamepad& gamepad, uint8_t bitfield);
      void HandleBattery(Gamepad& gamepad, BatteryID batteryID, uint8_t value);
      void HandleSensor(Gamepad& gamepad, SensorID sensorID, int16_t valueX, int16_t valueY, int16_t valueZ);

      // Send command messages
      bool SendCommand(const internal::Command& command) noexcept;
  };
} // namespace GSB