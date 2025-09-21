#include "ApplicationLink.h"

namespace GSB {
  ApplicationLink::ApplicationLink(const LinkConfig& linkConfig) noexcept : internal::LinkBase(linkConfig) {

  }

  ApplicationLink::~ApplicationLink() {

  }

  // ------------------- callback pass-throughs -------------------
  void ApplicationLink::SetButtonOnPress(void (*fxPtr)(uint8_t gamepadIndex, ButtonID buttonID)) {
    for(uint8_t i = 0; i < m_gamepadCount; ++i) {
      m_gamepads[i].SetBtnOnPress(fxPtr);
    }
  }

  void ApplicationLink::SetButtonOnRelease(void (*fxPtr)(uint8_t gamepadIndex, ButtonID buttonID)) {
    for(uint8_t i = 0; i < m_gamepadCount; ++i) {
      m_gamepads[i].SetBtnOnRelease(fxPtr);
    }
  }

  void ApplicationLink::SetTriggerOnChange(void (*fxPtr)(uint8_t gamepadIndex, TriggerID triggerID, int16_t value)) {
    for(uint8_t i = 0; i < m_gamepadCount; ++i) {
      m_gamepads[i].SetTriggerOnChange(fxPtr);
    }
  }

  void ApplicationLink::SetJoystickOnChange(void (*fxPtr)(uint8_t gamepadIndex, JoystickID joystickID, int16_t xValue, int16_t yValue)) {
    for(uint8_t i = 0; i < m_gamepadCount; ++i) {
      m_gamepads[i].SetJoystickOnChange(fxPtr);
    }
  }

  void ApplicationLink::SetBatteryOnChange(void (*fxPtr)(uint8_t gamepadIndex, BatteryID batteryID, uint8_t value)) {
    for(uint8_t i = 0; i < m_gamepadCount; ++i) {
      m_gamepads[i].SetBatteryOnChange(fxPtr);
    }
  }

  void ApplicationLink::SetSensorOnChange(void (*fxPtr)(uint8_t gamepadIndex, SensorID sensorID, int16_t xValue, int16_t yValue, int16_t zValue)) {
    for(uint8_t i = 0; i < m_gamepadCount; ++i) {
      m_gamepads[i].SetSensorOnChange(fxPtr);
    }
  }

  // ------------------- per-controller tolerance setters -------------------
  void ApplicationLink::SetTriggerTolerance(uint8_t gamepadIndex, TriggerID triggerID, uint16_t tolerance) {
    if (gamepadIndex >= m_gamepadCount) {
      return;
    }
    m_gamepads[gamepadIndex].SetTriggerTolerance(triggerID, tolerance);
  }

  void ApplicationLink::SetJoystickTolerance(uint8_t gamepadIndex, JoystickID joystickID, uint16_t xTolerance, uint16_t yTolerance) {
    if (ggamepadIndex >= m_gamepadCount) {
      return;
    }
    m_gamepads[gamepadIndex].SetJoystickTolerance(joystickID, xTolerance, yTolerance);
  }

  void ApplicationLink::SetJoystickToleranceX(uint8_t gamepadIndex, JoystickID joystickID, uint16_t tolerance) {
    if (ggamepadIndex >= m_gamepadCount) {
      return;
    }
    m_gamepads[gamepadIndex].SetJoystickToleranceX(joystickID, tolerance);
  }

  void ApplicationLink::SetJoystickToleranceY(uint8_t gamepadIndex, JoystickID joystickID, uint16_t tolerance) {
    if (gamepadIndex >= m_gamepadCount) {
      return;
    }
    m_gamepads[gamepadIndex].SetJoystickToleranceY(joystickID, tolerance);
  }

  void ApplicationLink::SetBatteryTolerance(uint8_t gamepadIndex, BatteryID batteryID, uint8_t tolerance) {
    if (gamepadIndex >= m_gamepadCount) {
      return;
    }
    m_gamepads[gamepadIndex].SetBatteryTolerance(batteryID, tolerance);
  }

  void ApplicationLink::SetSensorTolerance(uint8_t gamepadIndex, SensorID sensorID, uint16_t xTolerance, uint16_t yTolerance, uint16_t zTolerance) {
    if (gamepadIndex >= m_gamepadCount) {
      return;
    }
    m_gamepads[gamepadIndex].SetSensorTolerance(sensorID, xTolerance, yTolerance, zTolerance);
  }

  void ApplicationLink::SetSensorToleranceX(uint8_t gamepadIndex, SensorID sensorID, uint16_t tolerance) {
    if (gamepadIndex >= m_gamepadCount) {
      return;
    }
    m_gamepads[gamepadIndex].SetSensorToleranceX(sensorID, tolerance);
  }

  void ApplicationLink::SetSensorToleranceY(uint8_t gamepadIndex, SensorID sensorID, uint16_t tolerance) {
    if (gamepadIndex >= m_gamepadCount) {
      return;
    }
    m_gamepads[gamepadIndex].SetSensorToleranceY(sensorID, tolerance);
  }

  void ApplicationLink::SetSensorToleranceZ(uint8_t gamepadIndex, SensorID sensorID, uint16_t tolerance) {
    if (gamepadIndex >= m_gamepadCount) {
      return;
    }
    m_gamepads[gamepadIndex].SetSensorToleranceZ(sensorID, tolerance);
  }

  // ------------------- serial ingest -------------------
  void ApplicationLink::ParseSerial(const uint8_t* data, size_t length) noexcept {
    if (data == nullptr || length != sizeof(internal::Status)) {
      Log(F("Invalid Binary Payload Length"));
      return;
    }
    internal::Status status{};
    status.gamepadIndex = UInt8AtOffset(data, 0);
    if (status.gamepadIndex >= GetGamepadCount()) {
      Log(F("Invalid Binary Payload - Invalid Gamepad Index"));
      return;
    }
    status.dpadMask = UInt8AtOffset(data, 1);
    status.mainButtonsMask  = UInt16AtOffset(data, 2);
    status.joystick1X = Int16AtOffset(data, 4);
    status.joystick1Y = Int16AtOffset(data, 6);
    status.joystick2X = Int16AtOffset(data, 8);
    status.joystick2Y = Int16AtOffset(data, 10);
    status.trigger1 = Int16AtOffset(data, 12);
    status.trigger2 = Int16AtOffset(data, 14);
    status.miscButtonsMask = UInt8AtOffset(data, 16);
    status.battery = UInt8AtOffset(data, 17);
    status.sensor1X = Int16AtOffset(data, 18);
    status.sensor1Y = Int16AtOffset(data, 20);
    status.sensor1Z = Int16AtOffset(data, 22);
    status.sensor2X = Int16AtOffset(data, 24);
    status.sensor2Y = Int16AtOffset(data, 26);
    status.sensor2Z = Int16AtOffset(data, 28);
    ApplyStatus(status);
  }

  void ApplicationLink::ApplyStatus(const internal::Status& status) {
    if (status.gamepadIndex >= m_gamepadCount) {
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
    HandleBattery(gamepad, status.battery);
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

  void ApplicationLink::HandleJoystick(Gamepad& gamepad, JoystickID joystickID, int16_t xValue, int16_t yValue) {
    gamepad.SetJoystick(joystickID, xValue, yValue);
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

  void ApplicationLink::HandleSensor(Gamepad& gamepad, SensorID sensorID, int16_t xValue, int16_t yValue, int16_t zValue) {
    gamepad.SetSensor(sensorID, xValue, yValue, zValue);
  }
} // namespace GSB