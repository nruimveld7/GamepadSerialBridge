#include "GamepadLink.h"

namespace GSB {
  GamepadLink::GamepadLink(LinkConfig linkConfig) : internal::LinkBase(linkConfig) {

  }

  GamepadLink::~GamepadLink() {

  }

  void GamepadLink::SetButton(int gamepadIndex, ButtonID buttonID, bool pressed) {
    if (gamepadIndex < 0 || gamepadIndex >= m_gamepadCount) {
      return;
    }
    m_gamepads[gamepadIndex].SetButton(buttonID, pressed);
  }

  void GamepadLink::SetTrigger(int gamepadIndex, TriggerID triggerID, int value) {
    if (gamepadIndex < 0 || gamepadIndex >= m_gamepadCount) {
      return;
    }
    m_gamepads[gamepadIndex].SetTrigger(triggerID, value);
  }

  void GamepadLink::SetJoystick(int gamepadIndex, JoystickID joystickID, int xValue, int yValue) {
    if (gamepadIndex < 0 || gamepadIndex >= m_gamepadCount) {
      return;
    }
    m_gamepads[gamepadIndex].SetJoystick(joystickID, xValue, yValue);
  }

  void GamepadLink::SetJoystickX(int gamepadIndex, JoystickID joystickID, int value) {
    if (gamepadIndex < 0 || gamepadIndex >= m_gamepadCount) {
      return;
    }
    m_gamepads[gamepadIndex].SetJoystickX(joystickID, value);
  }

  void GamepadLink::SetJoystickY(int gamepadIndex, JoystickID joystickID, int value) {
    if (gamepadIndex < 0 || gamepadIndex >= m_gamepadCount) {
      return;
    }
    m_gamepads[gamepadIndex].SetJoystickY(joystickID, value);
  }

  void GamepadLink::SetSensor(int gamepadIndex, SensorID sensorID, int xValue, int yValue, int zValue) {
    if (gamepadIndex < 0 || gamepadIndex >= m_gamepadCount) {
      return;
    }
    m_gamepads[gamepadIndex].SetSensor(sensorID, xValue, yValue, zValue);
  }

  void GamepadLink::SetSensorX(int gamepadIndex, SensorID sensorID, int value) {
    if (gamepadIndex < 0 || gamepadIndex >= m_gamepadCount) {
      return;
    }
    m_gamepads[gamepadIndex].SetSensorX(sensorID, value);
  }

  void GamepadLink::SetSensorY(int gamepadIndex, SensorID sensorID, int value) {
    if (gamepadIndex < 0 || gamepadIndex >= m_gamepadCount) {
      return;
    }
    m_gamepads[gamepadIndex].SetSensorY(sensorID, value);
  }

  void GamepadLink::SetSensorZ(int gamepadIndex, SensorID sensorID, int value) {
    if (gamepadIndex < 0 || gamepadIndex >= m_gamepadCount) {
      return;
    }
    m_gamepads[gamepadIndex].SetSensorZ(sensorID, value);
  }
} // namespace GSB