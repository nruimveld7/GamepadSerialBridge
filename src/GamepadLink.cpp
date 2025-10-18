#include "GamepadLink.h"

namespace GSB {
  GamepadLink::GamepadLink(const LinkConfig& linkConfig) noexcept : internal::LinkBase(linkConfig) {

  }

  // ──────────────────────────────
  // Callbacks (applied to all gamepads)
  // ──────────────────────────────
  void GamepadLink::SetOnDisconnect(void (*fxPtr)(uint8_t gamepadIndex)) noexcept {
    for (uint8_t i = 0; i < GetGamepadCount(); ++i) {
      GetGamepad(i).SetOnDisconnect(fxPtr);
    }
  }

  void GamepadLink::SetRumbleOnChange(void (*fxPtr)(uint8_t gamepadIndex, RumbleID rumbleID, uint8_t force, uint8_t duration)) noexcept {
    for (uint8_t i = 0; i < GetGamepadCount(); ++i) {
      GetGamepad(i).SetRumbleOnChange(fxPtr);
    }
  }

  void GamepadLink::SetPlayerLedOnChange(void (*fxPtr)(uint8_t gamepadIndex, PlayerLedID playerLedID, bool illuminated)) noexcept {
    for (uint8_t i = 0; i < GetGamepadCount(); ++i) {
      GetGamepad(i).SetPlayerLedOnChange(fxPtr);
    }
  }

  void GamepadLink::SetColorLedOnChange(void (*fxPtr)(uint8_t gamepadIndex, ColorLedID colorLedID, bool illuminated, uint8_t red, uint8_t green, uint8_t blue)) noexcept {
    for (uint8_t i = 0; i < GetGamepadCount(); ++i) {
      GetGamepad(i).SetColorLedOnChange(fxPtr);
    }
  }

  // ──────────────────────────────
  // STATUS
  // ──────────────────────────────
  void GamepadLink::SetButton(uint8_t gamepadIndex, ButtonID buttonID, bool pressed) noexcept {
    GetGamepad(gamepadIndex).SetButton(buttonID, pressed);
  }

  void GamepadLink::SetTrigger(uint8_t gamepadIndex, TriggerID triggerID, int16_t value) noexcept {
    GetGamepad(gamepadIndex).SetTrigger(triggerID, value);
  }

  void GamepadLink::SetJoystick(uint8_t gamepadIndex, JoystickID joystickID, int16_t valueX, int16_t valueY) noexcept {
    GetGamepad(gamepadIndex).SetJoystick(joystickID, valueX, valueY);
  }

  void GamepadLink::SetJoystickX(uint8_t gamepadIndex, JoystickID joystickID, int16_t value) noexcept {
    GetGamepad(gamepadIndex).SetJoystickX(joystickID, value);
  }

  void GamepadLink::SetJoystickY(uint8_t gamepadIndex, JoystickID joystickID, int16_t value) noexcept {
    GetGamepad(gamepadIndex).SetJoystickY(joystickID, value);
  }

  void GamepadLink::SetBattery(uint8_t gamepadIndex, BatteryID batteryID, uint8_t value) noexcept {
    GetGamepad(gamepadIndex).SetBattery(batteryID, value);
  }

  void GamepadLink::SetSensor(uint8_t gamepadIndex, SensorID sensorID, int16_t valueX, int16_t valueY, int16_t valueZ) noexcept {
    GetGamepad(gamepadIndex).SetSensor(sensorID, valueX, valueY, valueZ);
  }

  void GamepadLink::SetSensorX(uint8_t gamepadIndex, SensorID sensorID, int16_t value) noexcept {
    GetGamepad(gamepadIndex).SetSensorX(sensorID, value);
  }

  void GamepadLink::SetSensorY(uint8_t gamepadIndex, SensorID sensorID, int16_t value) noexcept {
    GetGamepad(gamepadIndex).SetSensorY(sensorID, value);
  }

  void GamepadLink::SetSensorZ(uint8_t gamepadIndex, SensorID sensorID, int16_t value) noexcept {
    GetGamepad(gamepadIndex).SetSensorZ(sensorID, value);
  }

  bool GamepadLink::SendStatus(uint8_t gamepadIndex) noexcept {
    if (gamepadIndex >= GetGamepadCount()) {
      Log(F("SendStatus: bad gamepad index"));
      return false;
    }
    internal::Status& status = GetGamepad(gamepadIndex).GetStatus();
    uint8_t data[internal::Status::MaximumLength()];
    const size_t length = status.Serialize(data, sizeof(data));
    if (length == 0) {
      return false;
    }
    return SendSerial(data, length);
  }
  
  // ------------------- serial ingest -------------------
  void GamepadLink::ParseSerial(const uint8_t* data, size_t length) noexcept {
    internal::Command command{};
    if(!command.Deserialize(data, length)) {
      Log(F("Invalid Command Payload"));
      return;
    }
    ApplyCommand(command);
  }

  void GamepadLink::ApplyCommand(const internal::Command& command) {
    const internal::Command::Target target = command.GetTarget();
    const bool targetAll = target.IsAll();
    if(!targetAll && target.GetValue() >= GetGamepadCount()) {
      Log(F("Invalid Command Target"));
      return;
    }
    const internal::Command::OpCode& opCode = command.GetOpCode();
    const internal::Command::Output& output = command.GetOutput();
    const bool outputAll = output.IsAll();
    const internal::Command::AppliedParameters& parameters = command.GetParameters();
    
    switch(opCode) {
      case internal::Command::OpCode::Disconnect: {
        if (targetAll) {
          DisconnectAllGamepads();
        } else {
          Disconnect(target.GetValue());
        }
        break;
      }

      case internal::Command::OpCode::RumbleStart: {
        const uint8_t force = parameters.rumble.force;
        const uint8_t duration = parameters.rumble.duration;
        if (outputAll) {
          if (targetAll) {
            StartRumbleForAllGamepads(force, duration);
          } else {
            StartRumble(target.GetValue(), force, duration);
          }
        } else {
          const RumbleID rumbleID = static_cast<RumbleID>(output.GetValue());
          if (targetAll) {
            StartRumbleForAllGamepads(rumbleID, force, duration);
          } else {
            StartRumble(target.GetValue(), rumbleID, force, duration);
          }
        }
        break;
      }

      case internal::Command::OpCode::RumbleStop: {
        if (outputAll) {
          if (targetAll) {
            StopRumbleForAllGamepads();
          } else {
            StopRumble(target.GetValue());
          }
        } else {
          const RumbleID rumbleID = static_cast<RumbleID>(output.GetValue());
          if (targetAll) {
            StopRumbleForAllGamepads(rumbleID);
          } else {
            StopRumble(target.GetValue(), rumbleID);
          }
        }
        break;
      }

      case internal::Command::OpCode::PlayerLedSet: {
        const bool illuminated = internal::Uint8ToBool(parameters.ledSet.illuminated);
        if (outputAll) {
          if (targetAll) {
            SetPlayerLedsForAllGamepads(illuminated);
          } else {
            SetPlayerLeds(target.GetValue(), illuminated);
          }
        } else {
          const PlayerLedID playerLedID = static_cast<PlayerLedID>(output.GetValue());
          if (targetAll) {
            SetPlayerLedForAllGamepads(playerLedID, illuminated);
          } else {
            SetPlayerLed(target.GetValue(), playerLedID, illuminated);
          }
        }
        break;
      }

      case internal::Command::OpCode::PlayerLedToggle: {
        if (outputAll) {
          if (targetAll) {
            TogglePlayerLedsForAllGamepads();
          } else {
            TogglePlayerLeds(target.GetValue());
          }
        } else {
          const PlayerLedID playerLedID = static_cast<PlayerLedID>(output.GetValue());
          if (targetAll) {
            TogglePlayerLedForAllGamepads(playerLedID);
          } else {
            TogglePlayerLed(target.GetValue(), playerLedID);
          }
        }
        break;
      }

      case internal::Command::OpCode::PlayerLedsSetMask: {
        const uint8_t mask = parameters.ledsMask.mask;
        if (targetAll) {
          SetPlayerLedsForAllGamepads(mask);
        } else {
          SetPlayerLeds(target.GetValue(), mask);
        }
        break;
      }

      case internal::Command::OpCode::PlayerLedsToggleMask: {
        const uint8_t mask = parameters.ledsMask.mask;
        if (targetAll) {
          TogglePlayerLedsForAllGamepads(mask);
        } else {
          TogglePlayerLeds(target.GetValue(), mask);
        }
        break;
      }

      case internal::Command::OpCode::ColorLedSet: {
        const bool illuminated = internal::Uint8ToBool(parameters.ledSet.illuminated);
        if (outputAll) {
          if (targetAll) {
            SetColorLedsForAllGamepads(illuminated);
          } else {
            SetColorLeds(target.GetValue(), illuminated);
          }
        } else {
          const ColorLedID colorLedID = static_cast<ColorLedID>(output.GetValue());
          if (targetAll) {
            SetColorLedForAllGamepads(colorLedID, illuminated);
          } else {
            SetColorLed(target.GetValue(), colorLedID, illuminated);
          }
        }
        break;
      }

      case internal::Command::OpCode::ColorLedSetColor: {
        const bool illuminated = internal::Uint8ToBool(parameters.ledSetColor.illuminated);
        const uint8_t red   = parameters.ledSetColor.red;
        const uint8_t green = parameters.ledSetColor.green;
        const uint8_t blue  = parameters.ledSetColor.blue;
        if (outputAll) {
          if (targetAll) {
            SetColorLedsForAllGamepads(illuminated, red, green, blue);
          } else {
            SetColorLeds(target.GetValue(), illuminated, red, green, blue);
          }
        } else {
          const ColorLedID colorLedID = static_cast<ColorLedID>(output.GetValue());
          if (targetAll) {
            SetColorLedForAllGamepads(colorLedID, illuminated, red, green, blue);
          } else {
            SetColorLed(target.GetValue(), colorLedID, illuminated, red, green, blue);
          }
        }
        break;
      }

      case internal::Command::OpCode::ColorLedToggle: {
        if (outputAll) {
          if (targetAll) {
            ToggleColorLedsForAllGamepads();
          } else {
            ToggleColorLeds(target.GetValue());
          }
        } else {
          const ColorLedID colorLedID = static_cast<ColorLedID>(output.GetValue());
          if (targetAll) {
            ToggleColorLedForAllGamepads(colorLedID);
          } else {
            ToggleColorLed(target.GetValue(), colorLedID);
          }
        }
        break;
      }

      default: {
        Log(F("Invalid Command Op Code"));
        break;
      }
    }
  }

  // ──────────────────────────────
  // COMMANDS
  // ──────────────────────────────
  // All outputs on all gamepads
  void GamepadLink::DisconnectAllGamepads() noexcept {
    for (uint8_t gamepadIndex = 0; gamepadIndex < GetGamepadCount(); ++gamepadIndex) {
      Disconnect(gamepadIndex);
    }
  }

  void GamepadLink::StartRumbleForAllGamepads(uint8_t force, uint8_t duration) noexcept {
    for (uint8_t gamepadIndex = 0; gamepadIndex < GetGamepadCount(); ++gamepadIndex) {
      StartRumble(gamepadIndex, force, duration);
    }
  }

  void GamepadLink::StopRumbleForAllGamepads() noexcept {
    for (uint8_t gamepadIndex = 0; gamepadIndex < GetGamepadCount(); ++gamepadIndex) {
      StopRumble(gamepadIndex);
    }
  }

  void GamepadLink::SetPlayerLedsForAllGamepads(bool illuminated) noexcept {
    for (uint8_t gamepadIndex = 0; gamepadIndex < GetGamepadCount(); ++gamepadIndex) {
      SetPlayerLeds(gamepadIndex, illuminated);
    }
  }

  void GamepadLink::SetPlayerLedsForAllGamepads(uint8_t mask) noexcept {
    for (uint8_t gamepadIndex = 0; gamepadIndex < GetGamepadCount(); ++gamepadIndex) {
      SetPlayerLeds(gamepadIndex, mask);
    }
  }

  void GamepadLink::TogglePlayerLedsForAllGamepads() noexcept {
    for (uint8_t gamepadIndex = 0; gamepadIndex < GetGamepadCount(); ++gamepadIndex) {
      TogglePlayerLeds(gamepadIndex);
    }
  }

  void GamepadLink::TogglePlayerLedsForAllGamepads(uint8_t mask) noexcept {
    for (uint8_t gamepadIndex = 0; gamepadIndex < GetGamepadCount(); ++gamepadIndex) {
      TogglePlayerLeds(gamepadIndex, mask);
    }
  }

  void GamepadLink::SetColorLedsForAllGamepads(bool illuminated) noexcept {
    for (uint8_t gamepadIndex = 0; gamepadIndex < GetGamepadCount(); ++gamepadIndex) {
      SetColorLeds(gamepadIndex, illuminated);
    }
  }

  void GamepadLink::SetColorLedsForAllGamepads(bool illuminated, uint8_t red, uint8_t green, uint8_t blue) noexcept {
    for (uint8_t gamepadIndex = 0; gamepadIndex < GetGamepadCount(); ++gamepadIndex) {
      SetColorLeds(gamepadIndex, illuminated, red, green, blue);
    }
  }

  void GamepadLink::ToggleColorLedsForAllGamepads() noexcept {
    for (uint8_t gamepadIndex = 0; gamepadIndex < GetGamepadCount(); ++gamepadIndex) {
      ToggleColorLeds(gamepadIndex);
    }
  }

  // One output on all gamepads
  void GamepadLink::StartRumbleForAllGamepads(RumbleID rumbleID, uint8_t force, uint8_t duration) noexcept {
    for (uint8_t gamepadIndex = 0; gamepadIndex < GetGamepadCount(); ++gamepadIndex) {
      StartRumble(gamepadIndex, rumbleID, force, duration);
    }
  }

  void GamepadLink::StopRumbleForAllGamepads(RumbleID rumbleID) noexcept {
    for (uint8_t gamepadIndex = 0; gamepadIndex < GetGamepadCount(); ++gamepadIndex) {
      StopRumble(gamepadIndex, rumbleID);
    }
  }

  void GamepadLink::SetPlayerLedForAllGamepads(PlayerLedID playerLedID, bool illuminated) noexcept {
    for (uint8_t gamepadIndex = 0; gamepadIndex < GetGamepadCount(); ++gamepadIndex) {
      SetPlayerLed(gamepadIndex, playerLedID, illuminated);
    }
  }

  void GamepadLink::TogglePlayerLedForAllGamepads(PlayerLedID playerLedID) noexcept {
    for (uint8_t gamepadIndex = 0; gamepadIndex < GetGamepadCount(); ++gamepadIndex) {
      TogglePlayerLed(gamepadIndex, playerLedID);
    }
  }

  void GamepadLink::SetColorLedForAllGamepads(ColorLedID colorLedID, bool illuminated) noexcept {
    for (uint8_t gamepadIndex = 0; gamepadIndex < GetGamepadCount(); ++gamepadIndex) {
      SetColorLed(gamepadIndex, colorLedID, illuminated);
    }
  }

  void GamepadLink::SetColorLedForAllGamepads(ColorLedID colorLedID, bool illuminated, uint8_t red, uint8_t green, uint8_t blue) noexcept {
    for (uint8_t gamepadIndex = 0; gamepadIndex < GetGamepadCount(); ++gamepadIndex) {
      SetColorLed(gamepadIndex, colorLedID, illuminated, red, green, blue);
    }
  }

  void GamepadLink::ToggleColorLedForAllGamepads(ColorLedID colorLedID) noexcept {
    for (uint8_t gamepadIndex = 0; gamepadIndex < GetGamepadCount(); ++gamepadIndex) {
      ToggleColorLed(gamepadIndex, colorLedID);
    }
  }

  // All outputs on one gamepad
  void GamepadLink::Disconnect(uint8_t gamepadIndex) noexcept {
    if (gamepadIndex >= GetGamepadCount()) {
      Log(F("Disconnect: bad gamepad index"));
      return;
    }
    GetGamepad(gamepadIndex).SetDisconnect();
  }

  void GamepadLink::StartRumble(uint8_t gamepadIndex, uint8_t force, uint8_t duration) noexcept {
    for (uint8_t i = 0; i < RumbleCount(); ++i) {
      StartRumble(gamepadIndex, static_cast<RumbleID>(i), force, duration);
    }
  }

  void GamepadLink::StopRumble(uint8_t gamepadIndex) noexcept {
    for (uint8_t i = 0; i < RumbleCount(); ++i) {
      StopRumble(gamepadIndex, static_cast<RumbleID>(i));
    }
  }

  void GamepadLink::SetPlayerLeds(uint8_t gamepadIndex, bool illuminated) noexcept {
    for (uint8_t i = 0; i < PlayerLedCount(); ++i) {
      SetPlayerLed(gamepadIndex, static_cast<PlayerLedID>(i), illuminated);
    }
  }

  void GamepadLink::SetPlayerLeds(uint8_t gamepadIndex, uint8_t mask) noexcept {
    const uint8_t validLeds = (PlayerLedCount() < 8) ? PlayerLedCount() : 8;
    for (uint8_t i = 0; i < validLeds; ++i) {
      const bool illuminated = ((mask >> i) & 0x1u) != 0;
      SetPlayerLed(gamepadIndex, static_cast<PlayerLedID>(i), illuminated);
    }
  }

  void GamepadLink::TogglePlayerLeds(uint8_t gamepadIndex) noexcept {
    for (uint8_t i = 0; i < PlayerLedCount(); ++i) {
      TogglePlayerLed(gamepadIndex, static_cast<PlayerLedID>(i));
    }
  }

  void GamepadLink::TogglePlayerLeds(uint8_t gamepadIndex, uint8_t mask) noexcept {
    const uint8_t validLeds = (PlayerLedCount() < 8) ? PlayerLedCount() : 8;
    for (uint8_t i = 0; i < validLeds; ++i) {
      if ((mask >> i) & 0x1u) {
        TogglePlayerLed(gamepadIndex, static_cast<PlayerLedID>(i));
      }
    }
  }

  void GamepadLink::SetColorLeds(uint8_t gamepadIndex, bool illuminated) noexcept {
    for (uint8_t i = 0; i < ColorLedCount(); ++i) {
      SetColorLed(gamepadIndex, static_cast<ColorLedID>(i), illuminated);
    }
  }

  void GamepadLink::SetColorLeds(uint8_t gamepadIndex, bool illuminated, uint8_t red, uint8_t green, uint8_t blue) noexcept {
    for (uint8_t i = 0; i < ColorLedCount(); ++i) {
      SetColorLed(gamepadIndex, static_cast<ColorLedID>(i), illuminated, red, green, blue);
    }
  }

  void GamepadLink::ToggleColorLeds(uint8_t gamepadIndex) noexcept {
    for (uint8_t i = 0; i < ColorLedCount(); ++i) {
      ToggleColorLed(gamepadIndex, static_cast<ColorLedID>(i));
    }
  }

  // One output on one gamepad
  void GamepadLink::StartRumble(uint8_t gamepadIndex, RumbleID rumbleID, uint8_t force, uint8_t duration) noexcept {
    if (gamepadIndex >= GetGamepadCount()) {
      Log(F("StartRumble: bad gamepad index"));
      return;
    }
    GetGamepad(gamepadIndex).SetRumble(rumbleID, force, duration);
  }

  void GamepadLink::StopRumble(uint8_t gamepadIndex, RumbleID rumbleID) noexcept {
    if (gamepadIndex >= GetGamepadCount()) {
      Log(F("StopRumble: bad gamepad index"));
      return;
    }
    GetGamepad(gamepadIndex).SetRumble(rumbleID, 0, 0);
  }

  void GamepadLink::SetPlayerLed(uint8_t gamepadIndex, PlayerLedID playerLedID, bool illuminated) noexcept {
    if (gamepadIndex >= GetGamepadCount()) {
      Log(F("SetPlayerLed: bad gamepad index"));
      return;
    }
    GetGamepad(gamepadIndex).SetPlayerLed(playerLedID, illuminated);
  }

  void GamepadLink::TogglePlayerLed(uint8_t gamepadIndex, PlayerLedID playerLedID) noexcept {
    if (gamepadIndex >= GetGamepadCount()) {
      Log(F("TogglePlayerLed: bad gamepad index"));
      return;
    }
    GetGamepad(gamepadIndex).TogglePlayerLed(playerLedID);
  }

  void GamepadLink::SetColorLed(uint8_t gamepadIndex, ColorLedID colorLedID, bool illuminated) noexcept {
    if (gamepadIndex >= GetGamepadCount()) {
      Log(F("SetColorLed: bad gamepad index"));
      return;
    }
    Gamepad& gamepad = GetGamepad(gamepadIndex);
    Color color = gamepad.GetColorLed(colorLedID).GetColor();
    gamepad.SetColorLed(colorLedID, illuminated, color);
  }

  void GamepadLink::SetColorLed(uint8_t gamepadIndex, ColorLedID colorLedID, bool illuminated, uint8_t red, uint8_t green, uint8_t blue) noexcept {
    if (gamepadIndex >= GetGamepadCount()) {
      Log(F("SetColorLed: bad gamepad index"));
      return;
    }
    Color color;
    color.red = red;
    color.green = green;
    color.blue = blue;
    GetGamepad(gamepadIndex).SetColorLed(colorLedID, illuminated, color);
  }

  void GamepadLink::ToggleColorLed(uint8_t gamepadIndex, ColorLedID colorLedID) noexcept {
    if (gamepadIndex >= GetGamepadCount()) {
      Log(F("ToggleColorLed: bad gamepad index"));
      return;
    }
    GetGamepad(gamepadIndex).ToggleColorLed(colorLedID);
  }
} // namespace GSB