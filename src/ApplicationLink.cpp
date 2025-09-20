#include "ApplicationLink.h"
namespace GSB {
  ApplicationLink::ApplicationLink(GSBConfig config) : internal::GSBBase(config) {

  }

  ApplicationLink::~ApplicationLink() {

  }

  // ------------------- callback pass-throughs -------------------
  void ApplicationLink::SetButtonOnPress(void (*fxPtr)(int gamepadIndex, ButtonID buttonID)) {
    for(uint8_t i = 0; i < m_gamepadCount; ++i) {
      m_gamepads[i].SetBtnOnPress(fxPtr);
    }
  }

  void ApplicationLink::SetButtonOnRelease(void (*fxPtr)(int gamepadIndex, ButtonID buttonID)) {
    for(uint8_t i = 0; i < m_gamepadCount; i++) {
      m_gamepads[i].SetBtnOnRelease(fxPtr);
    }
  }

  void ApplicationLink::SetTriggerOnChange(void (*fxPtr)(int gamepadIndex, TriggerID triggerID, int value)) {
    for(uint8_t i = 0; i < m_gamepadCount; i++) {
      m_gamepads[i].SetTriggerOnChange(fxPtr);
    }
  }

  void ApplicationLink::SetJoystickOnChange(void (*fxPtr)(int gamepadIndex, JoystickID joystickID, int xValue, int yValue)) {
    for(uint8_t i = 0; i < m_gamepadCount; i++) {
      m_gamepads[i].SetJoystickOnChange(fxPtr);
    }
  }

  void ApplicationLink::SetSensorOnChange(void (*fxPtr)(int gamepadIndex, SensorID sensorID, int xValue, int yValue, int zValue)) {
    for(uint8_t i = 0; i < m_gamepadCount; i++) {
      m_gamepads[i].SetSensorOnChange(fxPtr);
    }
  }

  // ------------------- per-controller tolerance setters -------------------
  void ApplicationLink::SetTriggerTolerance(int gamepadIndex, TriggerID triggerID, int tolerance) {
    if (gamepadIndex < 0 || gamepadIndex >= m_gamepadCount) {
      return;
    }
    m_gamepads[gamepadIndex].SetTriggerTolerance(triggerID, tolerance);
  }

  void ApplicationLink::SetJoystickTolerance(int gamepadIndex, JoystickID joystickID, int xTolerance, int yTolerance) {
    if (gamepadIndex < 0 || gamepadIndex >= m_gamepadCount) {
      return;
    }
    m_gamepads[gamepadIndex].SetJoystickTolerance(joystickID, xTolerance, yTolerance);
  }

  void ApplicationLink::SetJoystickToleranceX(int gamepadIndex, JoystickID joystickID, int tolerance) {
    if (gamepadIndex < 0 || gamepadIndex >= m_gamepadCount) {
      return;
    }
    m_gamepads[gamepadIndex].SetJoystickToleranceX(joystickID, tolerance);
  }

  void ApplicationLink::SetJoystickToleranceY(int gamepadIndex, JoystickID joystickID, int tolerance) {
    if (gamepadIndex < 0 || gamepadIndex >= m_gamepadCount) {
      return;
    }
    m_gamepads[gamepadIndex].SetJoystickToleranceY(joystickID, tolerance);
  }

  void ApplicationLink::SetSensorTolerance(int gamepadIndex, SensorID sensorID, int xTolerance, int yTolerance, int zTolerance) {
    if (gamepadIndex < 0 || gamepadIndex >= m_gamepadCount) {
      return;
    }
    m_gamepads[gamepadIndex].SetSensorTolerance(sensorID, xTolerance, yTolerance, zTolerance);
  }

  void ApplicationLink::SetSensorToleranceX(int gamepadIndex, SensorID sensorID, int tolerance) {
    if (gamepadIndex < 0 || gamepadIndex >= m_gamepadCount) {
      return;
    }
    m_gamepads[gamepadIndex].SetSensorToleranceX(sensorID, tolerance);
  }

  void ApplicationLink::SetSensorToleranceY(int gamepadIndex, SensorID sensorID, int tolerance) {
    if (gamepadIndex < 0 || gamepadIndex >= m_gamepadCount) {
      return;
    }
    m_gamepads[gamepadIndex].SetSensorToleranceY(sensorID, tolerance);
  }

  void ApplicationLink::SetSensorToleranceZ(int gamepadIndex, SensorID sensorID, int tolerance) {
    if (gamepadIndex < 0 || gamepadIndex >= m_gamepadCount) {
      return;
    }
    m_gamepads[gamepadIndex].SetSensorToleranceZ(sensorID, tolerance);
  }

  // ------------------- serial ingest -------------------
  void ApplicationLink::ParseBinary(const uint8_t* data, size_t length) {
    if (data == nullptr || length != s_binaryPayloadLength) {
      Log(F("Invalid Binary Payload Length"));
      return;
    }
    FrameData frame{};
    frame.gamepadIndex = static_cast<int>(UInt8AtOffset(data, 0));
    if (frame.gamepadIndex < 0 || frame.gamepadIndex >= m_gamepadCount) {
      Log(F("Invalid Binary Payload - Invalid Gamepad Index"));
      return;
    }
    frame.dpadMask = UInt8AtOffset(data, 1);
    frame.mainButtonsMask  = UInt16AtOffset(data, 2);
    frame.joystick1X = Int16AtOffset(data, 4);
    frame.joystick1Y = Int16AtOffset(data, 6);
    frame.joystick2X = Int16AtOffset(data, 8);
    frame.joystick2Y = Int16AtOffset(data, 10);
    frame.trigger1 = Int16AtOffset(data, 12);
    frame.trigger2 = Int16AtOffset(data, 14);
    frame.miscButtonsMask = UInt8AtOffset(data, 16);
    frame.sensor1X = Int16AtOffset(data, 18);
    frame.sensor1Y = Int16AtOffset(data, 20);
    frame.sensor1Z = Int16AtOffset(data, 22);
    frame.sensor2X = Int16AtOffset(data, 24);
    frame.sensor2Y = Int16AtOffset(data, 26);
    frame.sensor2Z = Int16AtOffset(data, 28);
    ApplyFrame(frame);
  }

  void ApplicationLink::ParseASCII(const char* data, size_t length) {
    if (data == nullptr || length == 0) {
      Log(F("Invalid ASCII Payload Length!"));
      return;
    }
    // Expect 16 data fields plus mandatory trailing "csum=XXXX"
    static constexpr size_t s_requiredFieldCount = 16;
    const char* fieldPtrs[s_requiredFieldCount + 1];
    size_t fieldLengths[s_requiredFieldCount + 1];
    size_t fieldCount = 0;
    fieldPtrs[0] = data;
    fieldLengths[0] = 0;
    fieldCount = 1;
    size_t currentLen = 0;
    for (size_t i = 0; i < length; ++i) {
      const char character = data[i];
      if (character == '|') {
        fieldLengths[fieldCount - 1] = currentLen;
        currentLen = 0;
        if (fieldCount < s_requiredFieldCount + 1) {
          fieldPtrs[fieldCount] = data + i + 1;
          fieldLengths[fieldCount] = 0;
          ++fieldCount;
        } else {
          Log(F("Invalid ASCII Payload - Too Many Fields"));
          return;
        }
      } else {
        ++currentLen;
      }
    }
    fieldLengths[fieldCount - 1] = currentLen;

    if (fieldCount != s_requiredFieldCount + 1) {
      Log(F("Invalid ASCII Payload - Missing Checksum"));
      return;
    }
    // Parse trailing "checksum=XXXX"
    const char checksumPrefix[] = "checksum=";
    const size_t checksumPrefixLen = sizeof(checksumPrefix) - 1;

    uint16_t checksumVal = 0;
    if (!(fieldLengths[s_requiredFieldCount] >= checksumPrefixLen &&
          memcmp(fieldPtrs[s_requiredFieldCount], checksumPrefix, checksumPrefixLen) == 0 &&
          StrToHexU16(fieldPtrs[s_requiredFieldCount] + checksumPrefixLen,
                      fieldLengths[s_requiredFieldCount] - checksumPrefixLen,
                      checksumVal))) {
      Log(F("Invalid ASCII Payload - Incorrect Checksum"));
      return;
    }
    // CRC over the data substring: from data[0] up to the '|' before csum
    const size_t crcLen = static_cast<size_t>((fieldPtrs[s_requiredFieldCount] - 1) - data);
    const uint16_t crc = CRC16_CCITT(reinterpret_cast<const uint8_t*>(data), crcLen);
    if (crc != checksumVal) {
      Log(F("Invalid ASCII Payload - Failed CRC"));
      return;
    }
    FrameData frame{};
    frame.gamepadIndex = static_cast<int>(StrToInt(fieldPtrs[0],  fieldLengths[0]));
    if (frame.gamepadIndex < 0 || frame.gamepadIndex >= m_gamepadCount) {
      Log(F("Invalid Gamepad!"));
      return;
    }
    frame.dpadMask = static_cast<uint8_t>(StrToInt(fieldPtrs[1], fieldLengths[1]));
    frame.mainButtonsMask = static_cast<uint16_t>(StrToInt(fieldPtrs[2], fieldLengths[2]));
    frame.joystick1X = static_cast<int>(StrToInt(fieldPtrs[3], fieldLengths[3]));
    frame.joystick1Y = static_cast<int>(StrToInt(fieldPtrs[4], fieldLengths[4]));
    frame.joystick2X = static_cast<int>(StrToInt(fieldPtrs[5], fieldLengths[5]));
    frame.joystick2Y = static_cast<int>(StrToInt(fieldPtrs[6], fieldLengths[6]));
    frame.trigger1 = static_cast<int>(StrToInt(fieldPtrs[7], fieldLengths[7]));
    frame.trigger2 = static_cast<int>(StrToInt(fieldPtrs[8], fieldLengths[8]));
    frame.miscButtonsMask  = static_cast<uint8_t>(StrToInt(fieldPtrs[9], fieldLengths[9]));
    frame.sensor1X = static_cast<int>(StrToInt(fieldPtrs[10], fieldLengths[10]));
    frame.sensor1Y = static_cast<int>(StrToInt(fieldPtrs[11], fieldLengths[11]));
    frame.sensor1Z = static_cast<int>(StrToInt(fieldPtrs[12], fieldLengths[12]));
    frame.sensor2X = static_cast<int>(StrToInt(fieldPtrs[13], fieldLengths[13]));
    frame.sensor2Y = static_cast<int>(StrToInt(fieldPtrs[14], fieldLengths[14]));
    frame.sensor2Z = static_cast<int>(StrToInt(fieldPtrs[15], fieldLengths[15]));
    ApplyFrame(frame);
  }

  void ApplicationLink::ApplyFrame(const FrameData& frame) {
    if (frame.gamepadIndex < 0 || frame.gamepadIndex >= m_gamepadCount) {
      return;
    }
    Gamepad& gamepad = m_gamepads[frame.gamepadIndex];
    HandleDPad(gamepad, frame.dpadMask);
    HandleMainButtons(gamepad, frame.mainButtonsMask);
    HandleJoystick(gamepad, JoystickID::JOYSTICK_1, frame.joystick1X, frame.joystick1Y);
    HandleJoystick(gamepad, JoystickID::JOYSTICK_2, frame.joystick2X, frame.joystick2Y);
    HandleTrigger(gamepad, TriggerID::TRIGGER_1, frame.trigger1);
    HandleTrigger(gamepad, TriggerID::TRIGGER_2, frame.trigger2);
    HandleMiscButtons(gamepad, frame.miscButtonsMask);
    HandleSensor(gamepad, SensorID::SENSOR_1, frame.sensor1X, frame.sensor1Y, frame.sensor1Z);
    HandleSensor(gamepad, SensorID::SENSOR_2, frame.sensor2X, frame.sensor2Y, frame.sensor2Z);
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

  void ApplicationLink::HandleJoystick(Gamepad& gamepad, JoystickID joystickID, int xValue, int yValue) {
    gamepad.SetJoystick(joystickID, xValue, yValue);
  }

  void ApplicationLink::HandleTrigger(Gamepad& gamepad, TriggerID triggerID, int value) {
    gamepad.SetTrigger(triggerID, value);
  }

  void ApplicationLink::HandleMiscButtons(Gamepad& gamepad, uint8_t bitfield) {
    for (uint8_t i = 0; i < MiscButtons::Count(); ++i) {
      gamepad.SetButton(MiscButtons::buttonIDs[i], (bitfield >> i) & 1u);
    }
  }

  void ApplicationLink::HandleSensor(Gamepad& gamepad, SensorID sensorID, int xValue, int yValue, int zValue) {
    gamepad.SetSensor(sensorID, xValue, yValue, zValue);
  }
}