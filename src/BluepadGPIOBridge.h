#pragma once

#define MAXCONTROLLERS 4

#include <Arduino.h>
#include "UartConfig.h"
#include "GamepadInputIDs.h"
#include "Gamepad.h"
#include "Mappings/Mappings.h"

struct BluepadGPIOBridgeConfig {
  UartConfig uartConfig;
  HardwareSerial* processorSerial;
  HardwareSerial* receiverSerial;
  int resetPin;
  int gamepadCount;
};

class BluepadGPIOBridge {
  public:
    BluepadGPIOBridge(BluepadGPIOBridgeConfig config);
    ~BluepadGPIOBridge();

    bool Setup();
    void Loop();
    void Reset();

    // Pass-through callback setters (applied to all controllers)
    void SetButtonOnPress(void (*fxPtr)(int gamepadIndex, Gamepad::ButtonID buttonID));
    void SetButtonOnRelease(void (*fxPtr)(int gamepadIndex, Gamepad::ButtonID buttonID));
    void SetTriggerOnChange(void (*fxPtr)(int gamepadIndex, Gamepad::TriggerID triggerID, int value));
    void SetJoystickOnChange(void (*fxPtr)(int gamepadIndex, Gamepad::JoystickID joystickID, int xValue, int yValue));
    void SetSensorOnChange(void (*fxPtr)(int gamepadIndex, Gamepad::SensorID sensorID, int xValue, int yValue, int zValue));

    // Per-controller tolerance configuration
    void SetTriggerTolerance(int gamepadIndex, Gamepad::TriggerID triggerID, int tolerance);
    void SetJoystickTolerance(int gamepadIndex, Gamepad::JoystickID joystickID, int xTolerance, int yTolerance);
    void SetJoystickToleranceX(int gamepadIndex, Gamepad::JoystickID joystickID, int tolerance);
    void SetJoystickToleranceY(int gamepadIndex, Gamepad::JoystickID joystickID, int tolerance);
    void SetSensorTolerance(int gamepadIndex, Gamepad::SensorID sensorID, int xTolerance, int yTolerance, int zTolerance);
    void SetSensorToleranceX(int gamepadIndex, Gamepad::SensorID sensorID, int tolerance);
    void SetSensorToleranceY(int gamepadIndex, Gamepad::SensorID sensorID, int tolerance);
    void SetSensorToleranceZ(int gamepadIndex, Gamepad::SensorID sensorID, int tolerance);

  private:
    void ProcessSerial();
    void ParseData(char* payloadData, size_t payloadLength);
    void ApplyFrame(const char** fieldPointers, const size_t* fieldLengths, size_t fieldCount);
    void HandleDPad(Gamepad::Gamepad& gamepad, uint8_t bitfield);
    void HandleMainButtons(Gamepad::Gamepad& gamepad, uint16_t bitfield);
    void HandleJoystick(Gamepad::Gamepad& gamepad, Gamepad::JoystickID joystickID, int xValue, int yValue);
    void HandleTrigger(Gamepad::Gamepad& gamepad, Gamepad::TriggerID triggerID, int value);
    void HandleMiscButtons(Gamepad::Gamepad& gamepad, uint8_t bitfield);
    void HandleSensor(Gamepad::Gamepad& gamepad, Gamepad::SensorID sensorID, int xValue, int yValue, int zValue);
    inline void LogUnrecognizedIndex(int messageIndex, String data);
    static inline long StrToInt(const char* intStr, size_t length);
    static inline long StrToInt(const char* intStr);
    static inline long StrToInt(const String& intStr);
    static inline bool StrEquals(const char* a, const char* b);
    static inline bool StrStartsWith(const char* str, const char* prefix);
    static inline void PrintHex(HardwareSerial* output, const char* buffer, size_t bufferLength);

    int m_gamepadCount;
    //Gamepad::Gamepad* m_gamepads;
    Gamepad::Gamepad m_gamepads[MAXCONTROLLERS];

    UartConfig m_uartConfig;
    HardwareSerial* m_processorSerial;
    HardwareSerial* m_receiverSerial;
    int m_resetPin;
    static constexpr size_t s_bufferLength = 100;
};