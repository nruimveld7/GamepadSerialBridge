#pragma once

#include <Arduino.h>
#include "internal/LinkBase.h"
#include "internal/Utilities.h"

namespace GSB {
  class GamepadLink : public internal::LinkBase {
    public:
      GamepadLink(LinkConfig linkConfig);
      ~GamepadLink() noexcept = default;

      // ──────────────────────────────
      // Callbacks (applied to all gamepads)
      // ──────────────────────────────
      void SetOnDisconnect(void (*fxPtr)(uint8_t gamepadIndex)) noexcept;
      void SetRumbleOnChange(void (*fxPtr)(uint8_t gamepadIndex, RumbleID rumbleID, uint8_t force, uint8_t duration)) noexcept;
      void SetPlayerLedOnChange(void (*fxPtr)(uint8_t gamepadIndex, PlayerLedID playerLedID, bool illuminated)) noexcept;
      void SetColorLedOnChange(void (*fxPtr)(uint8_t gamepadIndex, ColorLedID colorLedID, bool illuminated, uint8_t red, uint8_t green, uint8_t blue)) noexcept;

      // ──────────────────────────────
      // STATUS
      // ──────────────────────────────
      void SetButton(uint8_t gamepadIndex, ButtonID buttonID, bool pressed) noexcept;
      void SetTrigger(uint8_t gamepadIndex, TriggerID triggerID, int16_t value) noexcept;
      void SetJoystick(uint8_t gamepadIndex, JoystickID joystickID, int16_t valueX, int16_t valueY) noexcept;
      void SetJoystickX(uint8_t gamepadIndex, JoystickID joystickID, int16_t value) noexcept;
      void SetJoystickY(uint8_t gamepadIndex, JoystickID joystickID, int16_t value) noexcept;
      void SetBattery(uint8_t gamepadIndex, BatteryID batteryID, uint8_t value) noexcept;
      void SetSensor(uint8_t gamepadIndex, SensorID sensorID, int16_t valueX, int16_t valueY, int16_t valueZ) noexcept;
      void SetSensorX(uint8_t gamepadIndex, SensorID sensorID, int16_t value) noexcept;
      void SetSensorY(uint8_t gamepadIndex, SensorID sensorID, int16_t value) noexcept;
      void SetSensorZ(uint8_t gamepadIndex, SensorID sensorID, int16_t value) noexcept;
      bool SendStatus(uint8_t gamepadIndex) noexcept;

    private:
      // Process command messages
      void ParseSerial(const uint8_t* data, size_t length) noexcept override;
      void ApplyCommand(const internal::Command& command);
      
      // ──────────────────────────────
      // COMMANDS
      // ──────────────────────────────
      // All outputs on all gamepads
      void DisconnectAllGamepads() noexcept;
      void StartRumbleForAllGamepads(uint8_t force, uint8_t duration) noexcept;
      void StopRumbleForAllGamepads() noexcept;
      void SetPlayerLedsForAllGamepads(bool illuminated) noexcept;
      void SetPlayerLedsForAllGamepads(uint8_t mask) noexcept;
      void TogglePlayerLedsForAllGamepads() noexcept;
      void TogglePlayerLedsForAllGamepads(uint8_t mask) noexcept;
      void SetColorLedsForAllGamepads(bool illuminated) noexcept;
      void SetColorLedsForAllGamepads(bool illuminated, uint8_t red, uint8_t green, uint8_t blue) noexcept;
      void ToggleColorLedsForAllGamepads() noexcept;

      // One output on all gamepads
      void StartRumbleForAllGamepads(RumbleID rumbleID, uint8_t force, uint8_t duration) noexcept;
      void StopRumbleForAllGamepads(RumbleID rumbleID) noexcept;
      void SetPlayerLedForAllGamepads(PlayerLedID playerLedID, bool illuminated) noexcept;
      void TogglePlayerLedForAllGamepads(PlayerLedID playerLedID) noexcept;
      void SetColorLedForAllGamepads(ColorLedID colorLedID, bool illuminated) noexcept;
      void SetColorLedForAllGamepads(ColorLedID colorLedID, bool illuminated, uint8_t red, uint8_t green, uint8_t blue) noexcept;
      void ToggleColorLedForAllGamepads(ColorLedID colorLedID) noexcept;


      // All outputs on one gamepad
      void Disconnect(uint8_t gamepadIndex) noexcept;
      void StartRumble(uint8_t gamepadIndex, uint8_t force, uint8_t duration) noexcept;
      void StopRumble(uint8_t gamepadIndex) noexcept;
      void SetPlayerLeds(uint8_t gamepadIndex, bool illuminated) noexcept;
      void SetPlayerLeds(uint8_t gamepadIndex, uint8_t mask) noexcept;
      void TogglePlayerLeds(uint8_t gamepadIndex) noexcept;
      void TogglePlayerLeds(uint8_t gamepadIndex, uint8_t mask) noexcept;
      void SetColorLeds(uint8_t gamepadIndex, bool illuminated) noexcept;
      void SetColorLeds(uint8_t gamepadIndex, bool illuminated, uint8_t red, uint8_t green, uint8_t blue) noexcept;
      void ToggleColorLeds(uint8_t gamepadIndex) noexcept;

      // One output on one gamepad
      void StartRumble(uint8_t gamepadIndex, RumbleID rumbleID, uint8_t force, uint8_t duration) noexcept;
      void StopRumble(uint8_t gamepadIndex, RumbleID rumbleID) noexcept;
      void SetPlayerLed(uint8_t gamepadIndex, PlayerLedID playerLedID, bool illuminated) noexcept;
      void TogglePlayerLed(uint8_t gamepadIndex, PlayerLedID playerLedID) noexcept;
      void SetColorLed(uint8_t gamepadIndex, ColorLedID colorLedID, bool illuminated) noexcept;
      void SetColorLed(uint8_t gamepadIndex, ColorLedID colorLedID, bool illuminated, uint8_t red, uint8_t green, uint8_t blue) noexcept;
      void ToggleColorLed(uint8_t gamepadIndex, ColorLedID colorLedID) noexcept;
  };
} // namespace GSB