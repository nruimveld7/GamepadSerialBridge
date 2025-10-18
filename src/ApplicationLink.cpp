#include "ApplicationLink.h"

namespace GSB {
  ApplicationLink::ApplicationLink(const LinkConfig& linkConfig) noexcept : internal::LinkBase(linkConfig) {

  }

  // ──────────────────────────────
  // Callbacks (applied to all gamepads)
  // ──────────────────────────────
  void ApplicationLink::SetButtonOnPress(void (*fxPtr)(uint8_t gamepadIndex, ButtonID buttonID)) noexcept {
    for(uint8_t i = 0; i < GetGamepadCount(); ++i) {
      GetGamepad(i).SetButtonOnPress(fxPtr);
    }
  }

  void ApplicationLink::SetButtonOnRelease(void (*fxPtr)(uint8_t gamepadIndex, ButtonID buttonID)) noexcept {
    for(uint8_t i = 0; i < GetGamepadCount(); ++i) {
      GetGamepad(i).SetButtonOnRelease(fxPtr);
    }
  }

  void ApplicationLink::SetTriggerOnChange(void (*fxPtr)(uint8_t gamepadIndex, TriggerID triggerID, int16_t value)) noexcept {
    for(uint8_t i = 0; i < GetGamepadCount(); ++i) {
      GetGamepad(i).SetTriggerOnChange(fxPtr);
    }
  }

  void ApplicationLink::SetJoystickOnChange(void (*fxPtr)(uint8_t gamepadIndex, JoystickID joystickID, int16_t valueX, int16_t valueY)) noexcept {
    for(uint8_t i = 0; i < GetGamepadCount(); ++i) {
      GetGamepad(i).SetJoystickOnChange(fxPtr);
    }
  }

  void ApplicationLink::SetBatteryOnChange(void (*fxPtr)(uint8_t gamepadIndex, BatteryID batteryID, uint8_t value)) noexcept {
    for(uint8_t i = 0; i < GetGamepadCount(); ++i) {
      GetGamepad(i).SetBatteryOnChange(fxPtr);
    }
  }

  void ApplicationLink::SetSensorOnChange(void (*fxPtr)(uint8_t gamepadIndex, SensorID sensorID, int16_t valueX, int16_t valueY, int16_t valueZ)) noexcept {
    for(uint8_t i = 0; i < GetGamepadCount(); ++i) {
      GetGamepad(i).SetSensorOnChange(fxPtr);
    }
  }

  // ──────────────────────────────
  // TOLERANCES
  // ──────────────────────────────
  // All inputs on all gamepads (set defaults across the board)
  void ApplicationLink::SetTriggerToleranceForAllGamepads(uint16_t tolerance) noexcept {
    for (uint8_t gamepadIndex = 0; gamepadIndex < GetGamepadCount(); ++gamepadIndex) {
      SetTriggerTolerance(gamepadIndex, tolerance);
    }
  }

  void ApplicationLink::SetJoystickToleranceForAllGamepads(uint16_t toleranceX, uint16_t toleranceY) noexcept {
    for (uint8_t gamepadIndex = 0; gamepadIndex < GetGamepadCount(); ++gamepadIndex) {
      SetJoystickTolerance(gamepadIndex, toleranceX, toleranceY);
    }
  }

  void ApplicationLink::SetJoystickToleranceXForAllGamepads(uint16_t tolerance) noexcept {
    for (uint8_t gamepadIndex = 0; gamepadIndex < GetGamepadCount(); ++gamepadIndex) {
      SetJoystickToleranceX(gamepadIndex, tolerance);
    }
  }

  void ApplicationLink::SetJoystickToleranceYForAllGamepads(uint16_t tolerance) noexcept {
    for (uint8_t gamepadIndex = 0; gamepadIndex < GetGamepadCount(); ++gamepadIndex) {
      SetJoystickToleranceY(gamepadIndex, tolerance);
    }
  }

  void ApplicationLink::SetBatteryToleranceForAllGamepads(uint8_t tolerance) noexcept {
    for (uint8_t gamepadIndex = 0; gamepadIndex < GetGamepadCount(); ++gamepadIndex) {
      SetBatteryTolerance(gamepadIndex, tolerance);
    }
  }

  void ApplicationLink::SetSensorToleranceForAllGamepads(uint16_t toleranceX, uint16_t toleranceY, uint16_t toleranceZ) noexcept {
    for (uint8_t gamepadIndex = 0; gamepadIndex < GetGamepadCount(); ++gamepadIndex) {
      SetSensorTolerance(gamepadIndex, toleranceX, toleranceY, toleranceZ);
    }
  }

  void ApplicationLink::SetSensorToleranceXForAllGamepads(uint16_t tolerance) noexcept {
    for (uint8_t gamepadIndex = 0; gamepadIndex < GetGamepadCount(); ++gamepadIndex) {
      SetSensorToleranceX(gamepadIndex, tolerance);
    }
  }

  void ApplicationLink::SetSensorToleranceYForAllGamepads(uint16_t tolerance) noexcept {
    for (uint8_t gamepadIndex = 0; gamepadIndex < GetGamepadCount(); ++gamepadIndex) {
      SetSensorToleranceY(gamepadIndex, tolerance);
    }
  }

  void ApplicationLink::SetSensorToleranceZForAllGamepads(uint16_t tolerance) noexcept {
    for (uint8_t gamepadIndex = 0; gamepadIndex < GetGamepadCount(); ++gamepadIndex) {
      SetSensorToleranceZ(gamepadIndex, tolerance);
    }
  }

  // One input on all gamepads
  void ApplicationLink::SetTriggerToleranceForAllGamepads(TriggerID triggerID, uint16_t tolerance) noexcept {
    for (uint8_t gamepadIndex = 0; gamepadIndex < GetGamepadCount(); ++gamepadIndex) {
      SetTriggerTolerance(gamepadIndex, triggerID, tolerance);
    }
  }

  void ApplicationLink::SetJoystickToleranceForAllGamepads(JoystickID joystickID, uint16_t toleranceX, uint16_t toleranceY) noexcept {
    for (uint8_t gamepadIndex = 0; gamepadIndex < GetGamepadCount(); ++gamepadIndex) {
      SetJoystickTolerance(gamepadIndex, joystickID, toleranceX, toleranceY);
    }
  }

  void ApplicationLink::SetJoystickToleranceXForAllGamepads(JoystickID joystickID, uint16_t tolerance) noexcept {
    for (uint8_t gamepadIndex = 0; gamepadIndex < GetGamepadCount(); ++gamepadIndex) {
      SetJoystickToleranceX(gamepadIndex, joystickID, tolerance);
    }
  }

  void ApplicationLink::SetJoystickToleranceYForAllGamepads(JoystickID joystickID, uint16_t tolerance) noexcept {
    for (uint8_t gamepadIndex = 0; gamepadIndex < GetGamepadCount(); ++gamepadIndex) {
      SetJoystickToleranceY(gamepadIndex, joystickID, tolerance);
    }
  }

  void ApplicationLink::SetBatteryToleranceForAllGamepads(BatteryID batteryID, uint8_t tolerance) noexcept {
    for (uint8_t gamepadIndex = 0; gamepadIndex < GetGamepadCount(); ++gamepadIndex) {
      SetBatteryTolerance(gamepadIndex, batteryID, tolerance);
    }
  }

  void ApplicationLink::SetSensorToleranceForAllGamepads(SensorID sensorID, uint16_t toleranceX, uint16_t toleranceY, uint16_t toleranceZ) noexcept {
    for (uint8_t gamepadIndex = 0; gamepadIndex < GetGamepadCount(); ++gamepadIndex) {
      SetSensorTolerance(gamepadIndex, sensorID, toleranceX, toleranceY, toleranceZ);
    }
  }

  void ApplicationLink::SetSensorToleranceXForAllGamepads(SensorID sensorID, uint16_t tolerance) noexcept {
    for (uint8_t gamepadIndex = 0; gamepadIndex < GetGamepadCount(); ++gamepadIndex) {
      SetSensorToleranceX(gamepadIndex, sensorID, tolerance);
    }
  }

  void ApplicationLink::SetSensorToleranceYForAllGamepads(SensorID sensorID, uint16_t tolerance) noexcept {
    for (uint8_t gamepadIndex = 0; gamepadIndex < GetGamepadCount(); ++gamepadIndex) {
      SetSensorToleranceY(gamepadIndex, sensorID, tolerance);
    }
  }

  void ApplicationLink::SetSensorToleranceZForAllGamepads(SensorID sensorID, uint16_t tolerance) noexcept {
    for (uint8_t gamepadIndex = 0; gamepadIndex < GetGamepadCount(); ++gamepadIndex) {
      SetSensorToleranceZ(gamepadIndex, sensorID, tolerance);
    }
  }

  // All inputs on one gamepad
  void ApplicationLink::SetTriggerTolerance(uint8_t gamepadIndex, uint16_t tolerance) noexcept {
    for(uint8_t triggerID = 0; triggerID < TriggerCount(); ++triggerID) {
      SetTriggerTolerance(gamepadIndex, static_cast<TriggerID>(triggerID), tolerance);
    }
  }

  void ApplicationLink::SetJoystickTolerance(uint8_t gamepadIndex, uint16_t toleranceX, uint16_t toleranceY) noexcept {
    for(uint8_t joystickID = 0; joystickID < JoystickCount(); ++joystickID) {
      SetJoystickTolerance(gamepadIndex, static_cast<JoystickID>(joystickID), toleranceX, toleranceY);
    }
  }

  void ApplicationLink::SetJoystickToleranceX(uint8_t gamepadIndex, uint16_t tolerance) noexcept {
    for(uint8_t joystickID = 0; joystickID < JoystickCount(); ++joystickID) {
      SetJoystickToleranceX(gamepadIndex, static_cast<JoystickID>(joystickID), tolerance);
    }
  }

  void ApplicationLink::SetJoystickToleranceY(uint8_t gamepadIndex, uint16_t tolerance) noexcept {
    for(uint8_t joystickID = 0; joystickID < JoystickCount(); ++joystickID) {
      SetJoystickToleranceY(gamepadIndex, static_cast<JoystickID>(joystickID), tolerance);
    }
  }

  void ApplicationLink::SetBatteryTolerance(uint8_t gamepadIndex, uint8_t tolerance) noexcept {
    for(uint8_t batteryID = 0; batteryID < BatteryCount(); ++batteryID) {
      SetBatteryTolerance(gamepadIndex, static_cast<BatteryID>(batteryID), tolerance);
    }
  }

  void ApplicationLink::SetSensorTolerance(uint8_t gamepadIndex, uint16_t toleranceX, uint16_t toleranceY, uint16_t toleranceZ) noexcept {
    for(uint8_t sensorID = 0; sensorID < SensorCount(); ++sensorID) {
      SetSensorTolerance(gamepadIndex, static_cast<SensorID>(sensorID), toleranceX, toleranceY, toleranceZ);
    }
  }

  void ApplicationLink::SetSensorToleranceX(uint8_t gamepadIndex, uint16_t tolerance) noexcept {
    for(uint8_t sensorID = 0; sensorID < SensorCount(); ++sensorID) {
      SetSensorToleranceX(gamepadIndex, static_cast<SensorID>(sensorID), tolerance);
    }
  }

  void ApplicationLink::SetSensorToleranceY(uint8_t gamepadIndex, uint16_t tolerance) noexcept {
    for(uint8_t sensorID = 0; sensorID < SensorCount(); ++sensorID) {
      SetSensorToleranceY(gamepadIndex, static_cast<SensorID>(sensorID), tolerance);
    }
  }

  void ApplicationLink::SetSensorToleranceZ(uint8_t gamepadIndex, uint16_t tolerance) noexcept {
    for(uint8_t sensorID = 0; sensorID < SensorCount(); ++sensorID) {
      SetSensorToleranceZ(gamepadIndex, static_cast<SensorID>(sensorID), tolerance);
    }
  }

  // One input on one gamepad
  void ApplicationLink::SetTriggerTolerance(uint8_t gamepadIndex, TriggerID triggerID, uint16_t tolerance) noexcept {
    if (gamepadIndex >= GetGamepadCount()) {
      return;
    }
    GetGamepad(gamepadIndex).SetTriggerTolerance(triggerID, tolerance);
  }

  void ApplicationLink::SetJoystickTolerance(uint8_t gamepadIndex, JoystickID joystickID, uint16_t toleranceX, uint16_t toleranceY) noexcept {
    if (gamepadIndex >= GetGamepadCount()) {
      return;
    }
    GetGamepad(gamepadIndex).SetJoystickTolerance(joystickID, toleranceX, toleranceY);
  }

  void ApplicationLink::SetJoystickToleranceX(uint8_t gamepadIndex, JoystickID joystickID, uint16_t tolerance) noexcept {
    if (gamepadIndex >= GetGamepadCount()) {
      return;
    }
    GetGamepad(gamepadIndex).SetJoystickToleranceX(joystickID, tolerance);
  }

  void ApplicationLink::SetJoystickToleranceY(uint8_t gamepadIndex, JoystickID joystickID, uint16_t tolerance) noexcept {
    if (gamepadIndex >= GetGamepadCount()) {
      return;
    }
    GetGamepad(gamepadIndex).SetJoystickToleranceY(joystickID, tolerance);
  }

  void ApplicationLink::SetBatteryTolerance(uint8_t gamepadIndex, BatteryID batteryID, uint8_t tolerance) noexcept {
    if (gamepadIndex >= GetGamepadCount()) {
      return;
    }
    GetGamepad(gamepadIndex).SetBatteryTolerance(batteryID, tolerance);
  }

  void ApplicationLink::SetSensorTolerance(uint8_t gamepadIndex, SensorID sensorID, uint16_t toleranceX, uint16_t toleranceY, uint16_t toleranceZ) noexcept {
    if (gamepadIndex >= GetGamepadCount()) {
      return;
    }
    GetGamepad(gamepadIndex).SetSensorTolerance(sensorID, toleranceX, toleranceY, toleranceZ);
  }

  void ApplicationLink::SetSensorToleranceX(uint8_t gamepadIndex, SensorID sensorID, uint16_t tolerance) noexcept {
    if (gamepadIndex >= GetGamepadCount()) {
      return;
    }
    GetGamepad(gamepadIndex).SetSensorToleranceX(sensorID, tolerance);
  }

  void ApplicationLink::SetSensorToleranceY(uint8_t gamepadIndex, SensorID sensorID, uint16_t tolerance) noexcept {
    if (gamepadIndex >= GetGamepadCount()) {
      return;
    }
    GetGamepad(gamepadIndex).SetSensorToleranceY(sensorID, tolerance);
  }

  void ApplicationLink::SetSensorToleranceZ(uint8_t gamepadIndex, SensorID sensorID, uint16_t tolerance) noexcept {
    if (gamepadIndex >= GetGamepadCount()) {
      return;
    }
    GetGamepad(gamepadIndex).SetSensorToleranceZ(sensorID, tolerance);
  }


  // ──────────────────────────────
  // COMMANDS
  // ──────────────────────────────
  bool ApplicationLink::DisconnectAllGamepads() noexcept {
    internal::Command::Target target = internal::Command::Target::All();
    internal::Command command = internal::Command::Build::Disconnect(target);
    return SendCommand(command);
  }

  // All outputs on all gamepads
  bool ApplicationLink::StartRumbleForAllGamepads(uint8_t force, uint8_t duration) noexcept {
    internal::Command::Target target = internal::Command::Target::All();
    internal::Command::Output output = internal::Command::Output::All();
    internal::Command::Parameters::Rumble parameters;
    parameters.force = force;
    parameters.duration = duration;
    internal::Command command = internal::Command::Build::RumbleStart(target, output, parameters);
    return SendCommand(command);
  }

  bool ApplicationLink::StopRumbleForAllGamepads() noexcept {
    internal::Command::Target target = internal::Command::Target::All();
    internal::Command::Output output = internal::Command::Output::All();
    internal::Command command = internal::Command::Build::RumbleStop(target, output);
    return SendCommand(command);
  }

  bool ApplicationLink::SetPlayerLedsForAllGamepads(bool illuminated) noexcept {
    internal::Command::Target target = internal::Command::Target::All();
    internal::Command::Output output = internal::Command::Output::All();
    internal::Command::Parameters::LedSet parameters;
    parameters.illuminated = illuminated;
    internal::Command command = internal::Command::Build::PlayerLedSet(target, output, parameters);
    return SendCommand(command);
  }

  bool ApplicationLink::SetPlayerLedsForAllGamepads(uint8_t mask) noexcept {
    internal::Command::Target target = internal::Command::Target::All();
    internal::Command::Output output = internal::Command::Output::All();
    internal::Command::Parameters::LedsMask parameters;
    parameters.mask = mask;
    internal::Command command = internal::Command::Build::PlayerLedsSetMask(target, output, parameters);
    return SendCommand(command);
  }

  bool ApplicationLink::TogglePlayerLedsForAllGamepads() noexcept {
    internal::Command::Target target = internal::Command::Target::All();
    internal::Command::Output output = internal::Command::Output::All();
    internal::Command command = internal::Command::Build::PlayerLedToggle(target, output);
    return SendCommand(command);
  }

  bool ApplicationLink::TogglePlayerLedsForAllGamepads(uint8_t mask) noexcept {
    internal::Command::Target target = internal::Command::Target::All();
    internal::Command::Output output = internal::Command::Output::All();
    internal::Command::Parameters::LedsMask parameters;
    parameters.mask = mask;
    internal::Command command = internal::Command::Build::PlayerLedsToggleMask(target, output, parameters);
    return SendCommand(command);
  }

  bool ApplicationLink::SetColorLedsForAllGamepads(bool illuminated) noexcept {
    internal::Command::Target target = internal::Command::Target::All();
    internal::Command::Output output = internal::Command::Output::All();
    internal::Command::Parameters::LedSet parameters;
    parameters.illuminated = illuminated;
    internal::Command command = internal::Command::Build::ColorLedSet(target, output, parameters);
    return SendCommand(command);
  }

  bool ApplicationLink::SetColorLedsForAllGamepads(bool illuminated, uint8_t red, uint8_t green, uint8_t blue) noexcept {
    internal::Command::Target target = internal::Command::Target::All();
    internal::Command::Output output = internal::Command::Output::All();
    internal::Command::Parameters::LedSetColor parameters;
    parameters.illuminated = illuminated;
    parameters.red = red;
    parameters.green = green;
    parameters.blue = blue;
    internal::Command command = internal::Command::Build::ColorLedSetColor(target, output, parameters);
    return SendCommand(command);
  }

  bool ApplicationLink::ToggleColorLedsForAllGamepads() noexcept {
    internal::Command::Target target = internal::Command::Target::All();
    internal::Command::Output output = internal::Command::Output::All();
    internal::Command command = internal::Command::Build::ColorLedToggle(target, output);
    return SendCommand(command);
  }

  // One output on all gamepad  
  bool ApplicationLink::StartRumbleForAllGamepads(RumbleID rumbleID, uint8_t force, uint8_t duration) noexcept {
    internal::Command::Target target = internal::Command::Target::All();
    internal::Command::Output output = internal::Command::Output::ID(rumbleID);
    internal::Command::Parameters::Rumble parameters;
    parameters.force = force;
    parameters.duration = duration;
    internal::Command command = internal::Command::Build::RumbleStart(target, output, parameters);
    return SendCommand(command);
  }

  bool ApplicationLink::StopRumbleForAllGamepads(RumbleID rumbleID) noexcept {
    internal::Command::Target target = internal::Command::Target::All();
    internal::Command::Output output = internal::Command::Output::ID(rumbleID);
    internal::Command command = internal::Command::Build::RumbleStop(target, output);
    return SendCommand(command);
  }

  bool ApplicationLink::SetPlayerLedForAllGamepads(PlayerLedID playerLedID, bool illuminated) noexcept {
    internal::Command::Target target = internal::Command::Target::All();
    internal::Command::Output output = internal::Command::Output::ID(playerLedID);
    internal::Command::Parameters::LedSet parameters;
    parameters.illuminated = illuminated;
    internal::Command command = internal::Command::Build::PlayerLedSet(target, output, parameters);
    return SendCommand(command);
  }

  bool ApplicationLink::TogglePlayerLedForAllGamepads(PlayerLedID playerLedID) noexcept {
    internal::Command::Target target = internal::Command::Target::All();
    internal::Command::Output output = internal::Command::Output::ID(playerLedID);
    internal::Command command = internal::Command::Build::PlayerLedToggle(target, output);
    return SendCommand(command);
  }

  bool ApplicationLink::SetColorLedForAllGamepads(ColorLedID colorLedID, bool illuminated) noexcept {
    internal::Command::Target target = internal::Command::Target::All();
    internal::Command::Output output = internal::Command::Output::ID(colorLedID);
    internal::Command::Parameters::LedSet parameters;
    parameters.illuminated = illuminated;
    internal::Command command = internal::Command::Build::ColorLedSet(target, output, parameters);
    return SendCommand(command);
  }

  bool ApplicationLink::SetColorLedForAllGamepads(ColorLedID colorLedID, bool illuminated, uint8_t red, uint8_t green, uint8_t blue) noexcept {
    internal::Command::Target target = internal::Command::Target::All();
    internal::Command::Output output = internal::Command::Output::ID(colorLedID);
    internal::Command::Parameters::LedSetColor parameters;
    parameters.illuminated = illuminated;
    parameters.red = red;
    parameters.green = green;
    parameters.blue = blue;
    internal::Command command = internal::Command::Build::ColorLedSetColor(target, output, parameters);
    return SendCommand(command);
  }

  bool ApplicationLink::ToggleColorLedForAllGamepads(ColorLedID colorLedID) noexcept {
    internal::Command::Target target = internal::Command::Target::All();
    internal::Command::Output output = internal::Command::Output::ID(colorLedID);
    internal::Command command = internal::Command::Build::ColorLedToggle(target, output);
    return SendCommand(command);
  }

  // All outputs on one gamepad  
  bool ApplicationLink::Disconnect(uint8_t gamepadIndex) noexcept {
    internal::Command::Target target = internal::Command::Target::Gamepad(gamepadIndex);
    internal::Command command = internal::Command::Build::Disconnect(target);
    return SendCommand(command);
  }

  bool ApplicationLink::StartRumble(uint8_t gamepadIndex, uint8_t force, uint8_t duration) noexcept {
    internal::Command::Target target = internal::Command::Target::Gamepad(gamepadIndex);
    internal::Command::Output output = internal::Command::Output::All();
    internal::Command::Parameters::Rumble parameters;
    parameters.force = force;
    parameters.duration = duration;
    internal::Command command = internal::Command::Build::RumbleStart(target, output, parameters);
    return SendCommand(command);
  }

  bool ApplicationLink::StopRumble(uint8_t gamepadIndex) noexcept {
    internal::Command::Target target = internal::Command::Target::Gamepad(gamepadIndex);
    internal::Command::Output output = internal::Command::Output::All();
    internal::Command command = internal::Command::Build::RumbleStop(target, output);
    return SendCommand(command);
  }

  bool ApplicationLink::SetPlayerLeds(uint8_t gamepadIndex, bool illuminated) noexcept {
    internal::Command::Target target = internal::Command::Target::Gamepad(gamepadIndex);
    internal::Command::Output output = internal::Command::Output::All();
    internal::Command::Parameters::LedSet parameters;
    parameters.illuminated = illuminated;
    internal::Command command = internal::Command::Build::PlayerLedSet(target, output, parameters);
    return SendCommand(command);
  }

  bool ApplicationLink::SetPlayerLeds(uint8_t gamepadIndex, uint8_t mask) noexcept {
    internal::Command::Target target = internal::Command::Target::Gamepad(gamepadIndex);
    internal::Command::Output output = internal::Command::Output::All();
    internal::Command::Parameters::LedsMask parameters;
    parameters.mask = mask;
    internal::Command command = internal::Command::Build::PlayerLedsSetMask(target, output, parameters);
    return SendCommand(command);
  }

  bool ApplicationLink::TogglePlayerLeds(uint8_t gamepadIndex) noexcept {
    internal::Command::Target target = internal::Command::Target::Gamepad(gamepadIndex);
    internal::Command::Output output = internal::Command::Output::All();
    internal::Command command = internal::Command::Build::PlayerLedToggle(target, output);
    return SendCommand(command);
  }

  bool ApplicationLink::TogglePlayerLeds(uint8_t gamepadIndex, uint8_t mask) noexcept {
    internal::Command::Target target = internal::Command::Target::Gamepad(gamepadIndex);
    internal::Command::Output output = internal::Command::Output::All();
    internal::Command::Parameters::LedsMask parameters;
    parameters.mask = mask;
    internal::Command command = internal::Command::Build::PlayerLedsToggleMask(target, output, parameters);
    return SendCommand(command);
  }

  bool ApplicationLink::SetColorLeds(uint8_t gamepadIndex, bool illuminated) noexcept {
    internal::Command::Target target = internal::Command::Target::Gamepad(gamepadIndex);
    internal::Command::Output output = internal::Command::Output::All();
    internal::Command::Parameters::LedSet parameters;
    parameters.illuminated = illuminated;
    internal::Command command = internal::Command::Build::ColorLedSet(target, output, parameters);
    return SendCommand(command);
  }

  bool ApplicationLink::SetColorLeds(uint8_t gamepadIndex, bool illuminated, uint8_t red, uint8_t green, uint8_t blue) noexcept {
    internal::Command::Target target = internal::Command::Target::Gamepad(gamepadIndex);
    internal::Command::Output output = internal::Command::Output::All();
    internal::Command::Parameters::LedSetColor parameters;
    parameters.illuminated = illuminated;
    parameters.red = red;
    parameters.green = green;
    parameters.blue = blue;
    internal::Command command = internal::Command::Build::ColorLedSetColor(target, output, parameters);
    return SendCommand(command);
  }

  bool ApplicationLink::ToggleColorLeds(uint8_t gamepadIndex) noexcept {
    internal::Command::Target target = internal::Command::Target::Gamepad(gamepadIndex);
    internal::Command::Output output = internal::Command::Output::All();
    internal::Command command = internal::Command::Build::ColorLedToggle(target, output);
    return SendCommand(command);
  }

  // One output on one gamepad
  bool ApplicationLink::StartRumble(uint8_t gamepadIndex, RumbleID rumbleID, uint8_t force, uint8_t duration) noexcept {
    internal::Command::Target target = internal::Command::Target::Gamepad(gamepadIndex);
    internal::Command::Output output = internal::Command::Output::ID(rumbleID);
    internal::Command::Parameters::Rumble parameters;
    parameters.force = force;
    parameters.duration = duration;
    internal::Command command = internal::Command::Build::RumbleStart(target, output, parameters);
    return SendCommand(command);
  }

  bool ApplicationLink::StopRumble(uint8_t gamepadIndex, RumbleID rumbleID) noexcept {
    internal::Command::Target target = internal::Command::Target::Gamepad(gamepadIndex);
    internal::Command::Output output = internal::Command::Output::ID(rumbleID);
    internal::Command command = internal::Command::Build::RumbleStop(target, output);
    return SendCommand(command);
  }

  bool ApplicationLink::SetPlayerLed(uint8_t gamepadIndex, PlayerLedID playerLedID, bool illuminated) noexcept {
    internal::Command::Target target = internal::Command::Target::Gamepad(gamepadIndex);
    internal::Command::Output output = internal::Command::Output::ID(playerLedID);
    internal::Command::Parameters::LedSet parameters;
    parameters.illuminated = illuminated;
    internal::Command command = internal::Command::Build::PlayerLedSet(target, output, parameters);
    return SendCommand(command);
  }

  bool ApplicationLink::TogglePlayerLed(uint8_t gamepadIndex, PlayerLedID playerLedID) noexcept {
    internal::Command::Target target = internal::Command::Target::Gamepad(gamepadIndex);
    internal::Command::Output output = internal::Command::Output::ID(playerLedID);
    internal::Command command = internal::Command::Build::PlayerLedToggle(target, output);
    return SendCommand(command);
  }

  bool ApplicationLink::SetColorLed(uint8_t gamepadIndex, ColorLedID colorLedID, bool illuminated) noexcept {
    internal::Command::Target target = internal::Command::Target::Gamepad(gamepadIndex);
    internal::Command::Output output = internal::Command::Output::ID(colorLedID);
    internal::Command::Parameters::LedSet parameters;
    parameters.illuminated = illuminated;
    internal::Command command = internal::Command::Build::ColorLedSet(target, output, parameters);
    return SendCommand(command);
  }

  bool ApplicationLink::SetColorLed(uint8_t gamepadIndex, ColorLedID colorLedID, bool illuminated, uint8_t red, uint8_t green, uint8_t blue) noexcept {
    internal::Command::Target target = internal::Command::Target::Gamepad(gamepadIndex);
    internal::Command::Output output = internal::Command::Output::ID(colorLedID);
    internal::Command::Parameters::LedSetColor parameters;
    parameters.illuminated = illuminated;
    parameters.red = red;
    parameters.green = green;
    parameters.blue = blue;
    internal::Command command = internal::Command::Build::ColorLedSetColor(target, output, parameters);
    return SendCommand(command);
  }

  bool ApplicationLink::ToggleColorLed(uint8_t gamepadIndex, ColorLedID colorLedID) noexcept {
    internal::Command::Target target = internal::Command::Target::Gamepad(gamepadIndex);
    internal::Command::Output output = internal::Command::Output::ID(colorLedID);
    internal::Command command = internal::Command::Build::ColorLedToggle(target, output);
    return SendCommand(command);
  }

  // ------------------- serial ingest -------------------
  void ApplicationLink::ParseSerial(const uint8_t* data, size_t length) noexcept {
    if (data == nullptr) {
      Log(F("Invalid Binary Payload"));
      return;
    }
    internal::Status status{};
    if(!internal::Status::Deserialize(data, length, status)) {
      Log(F("Invalid Binary Payload Length"));
      return;
    }
    if (status.gamepadIndex >= GetGamepadCount()) {
      Log(F("Invalid Binary Payload - Invalid Gamepad Index"));
      return;
    }
    ApplyStatus(status);
  }

  void ApplicationLink::ApplyStatus(const internal::Status& status) {
    if (status.gamepadIndex >= GetGamepadCount()) {
      return;
    }
    Gamepad& gamepad = GetGamepad(status.gamepadIndex);
    HandleDPad(gamepad, status.dpadMask);
    HandleMainButtons(gamepad, status.mainButtonsMask);
    HandleJoystick(gamepad, JoystickID::JOYSTICK_1, status.joystick1X, status.joystick1Y);
    HandleJoystick(gamepad, JoystickID::JOYSTICK_2, status.joystick2X, status.joystick2Y);
    HandleTrigger(gamepad, TriggerID::TRIGGER_1, status.trigger1);
    HandleTrigger(gamepad, TriggerID::TRIGGER_2, status.trigger2);
    HandleMiscButtons(gamepad, status.miscButtonsMask);
    HandleBattery(gamepad, BatteryID::BATTERY_1, status.battery1);
    HandleSensor(gamepad, SensorID::SENSOR_1, status.sensor1X, status.sensor1Y, status.sensor1Z);
    HandleSensor(gamepad, SensorID::SENSOR_2, status.sensor2X, status.sensor2Y, status.sensor2Z);
  }

  void ApplicationLink::HandleDPad(Gamepad& gamepad, uint8_t bitfield) {
    for (uint8_t i = 0; i < DPadButtons::Count(); ++i) {
      gamepad.SetButton(DPadButtons::buttonIDs[i], (bitfield >> i) & 1u);
    }
  }

  void ApplicationLink::HandleMainButtons(Gamepad& gamepad, uint16_t bitfield) {
    for (uint8_t i = 0; i < MainButtons::Count(); ++i) {
      gamepad.SetButton(MainButtons::buttonIDs[i], (bitfield >> i) & 1u);
    }
  }

  void ApplicationLink::HandleJoystick(Gamepad& gamepad, JoystickID joystickID, int16_t valueX, int16_t valueY) {
    gamepad.SetJoystick(joystickID, valueX, valueY);
  }

  void ApplicationLink::HandleTrigger(Gamepad& gamepad, TriggerID triggerID, int16_t value) {
    gamepad.SetTrigger(triggerID, value);
  }

  void ApplicationLink::HandleMiscButtons(Gamepad& gamepad, uint8_t bitfield) {
    for (uint8_t i = 0; i < MiscButtons::Count(); ++i) {
      gamepad.SetButton(MiscButtons::buttonIDs[i], (bitfield >> i) & 1u);
    }
  }

  void ApplicationLink::HandleBattery(Gamepad& gamepad, BatteryID batteryID, uint8_t value) {
    gamepad.SetBattery(batteryID, value);
  }

  void ApplicationLink::HandleSensor(Gamepad& gamepad, SensorID sensorID, int16_t valueX, int16_t valueY, int16_t valueZ) {
    gamepad.SetSensor(sensorID, valueX, valueY, valueZ);
  }

  bool ApplicationLink::SendCommand(const internal::Command& command) noexcept {
    uint8_t data[internal::Command::MaximumLength()];
    const size_t length = command.Serialize(data, sizeof(data));
    if(length == 0) {
      return false;
    }
    return SendSerial(data, length);
  }
} // namespace GSB