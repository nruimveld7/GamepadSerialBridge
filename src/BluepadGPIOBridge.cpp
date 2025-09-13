#include "BluepadGPIOBridge.h"
#include "UartConfig.h"
#include "GamepadInputIDs.h"
#include "Gamepad.h"

BluepadGPIOBridge::BluepadGPIOBridge(BluepadGPIOBridgeConfig config)
  : m_gamepadCount(config.gamepadCount),
    m_uartConfig(config.uartConfig),
    m_processorSerial(config.processorSerial),
    m_receiverSerial(config.receiverSerial),
    m_resetPin(config.resetPin) {
  if (m_gamepadCount < 0) {
    m_gamepadCount = 0;
  }
  if (m_gamepadCount > MAXCONTROLLERS) {
    m_gamepadCount = MAXCONTROLLERS;
  }
  for (uint8_t i = 0; i < m_gamepadCount; ++i) {
    m_gamepads[i] = Gamepad::Gamepad(i);
  }
}

BluepadGPIOBridge::~BluepadGPIOBridge() {

}

bool BluepadGPIOBridge::Setup() {
  m_receiverSerial->begin(m_uartConfig.GetBaudRate(), m_uartConfig.GetSerialConfig());
  m_receiverSerial->setTimeout(10);
  pinMode(m_resetPin, OUTPUT);
  digitalWrite(m_resetPin, HIGH);
  delay(2000);
  m_processorSerial->println(F("BluepadGPIOBridge Initialized"));
  return true;
}

void BluepadGPIOBridge::Loop() {
  ProcessSerial();
}

void BluepadGPIOBridge::Reset() {
  m_processorSerial->println(F("Hardware Reset"));
  digitalWrite(m_resetPin, LOW);
  delay(1000);
  digitalWrite(m_resetPin, HIGH);
  delay(1000);
  m_processorSerial->println(F("Done"));
}

// ------------------- callback pass-throughs -------------------

void BluepadGPIOBridge::SetButtonOnPress(void (*fxPtr)(int gamepadIndex, Gamepad::ButtonID buttonID)) {
  for(uint8_t i = 0; i < m_gamepadCount; ++i) {
    m_gamepads[i].SetBtnOnPress(fxPtr);
  }
}

void BluepadGPIOBridge::SetButtonOnRelease(void (*fxPtr)(int gamepadIndex, Gamepad::ButtonID buttonID)) {
  for(uint8_t i = 0; i < m_gamepadCount; i++) {
    m_gamepads[i].SetBtnOnRelease(fxPtr);
  }
}

void BluepadGPIOBridge::SetTriggerOnChange(void (*fxPtr)(int gamepadIndex, Gamepad::TriggerID triggerID, int value)) {
  for(uint8_t i = 0; i < m_gamepadCount; i++) {
    m_gamepads[i].SetTriggerOnChange(fxPtr);
  }
}

void BluepadGPIOBridge::SetJoystickOnChange(void (*fxPtr)(int gamepadIndex, Gamepad::JoystickID joystickID, int xValue, int yValue)) {
  for(uint8_t i = 0; i < m_gamepadCount; i++) {
    m_gamepads[i].SetJoystickOnChange(fxPtr);
  }
}

void BluepadGPIOBridge::SetSensorOnChange(void (*fxPtr)(int gamepadIndex, Gamepad::SensorID sensorID, int xValue, int yValue, int zValue)) {
  for(uint8_t i = 0; i < m_gamepadCount; i++) {
    m_gamepads[i].SetSensorOnChange(fxPtr);
  }
}

// ------------------- per-controller tolerance setters -------------------

void BluepadGPIOBridge::SetTriggerTolerance(int gamepadIndex, Gamepad::TriggerID triggerID, int tolerance) {
  if (gamepadIndex < 0 || gamepadIndex >= m_gamepadCount) {
    return;
  }
  m_gamepads[gamepadIndex].SetTriggerTolerance(triggerID, tolerance);
}

void BluepadGPIOBridge::SetJoystickTolerance(int gamepadIndex, Gamepad::JoystickID joystickID, int xTolerance, int yTolerance) {
  if (gamepadIndex < 0 || gamepadIndex >= m_gamepadCount) {
    return;
  }
  m_gamepads[gamepadIndex].SetJoystickTolerance(joystickID, xTolerance, yTolerance);
}

void BluepadGPIOBridge::SetJoystickToleranceX(int gamepadIndex, Gamepad::JoystickID joystickID, int tolerance) {
  if (gamepadIndex < 0 || gamepadIndex >= m_gamepadCount) {
    return;
  }
  m_gamepads[gamepadIndex].SetJoystickToleranceX(joystickID, tolerance);
}

void BluepadGPIOBridge::SetJoystickToleranceY(int gamepadIndex, Gamepad::JoystickID joystickID, int tolerance) {
  if (gamepadIndex < 0 || gamepadIndex >= m_gamepadCount) {
    return;
  }
  m_gamepads[gamepadIndex].SetJoystickToleranceY(joystickID, tolerance);
}

void BluepadGPIOBridge::SetSensorTolerance(int gamepadIndex, Gamepad::SensorID sensorID, int xTolerance, int yTolerance, int zTolerance) {
  if (gamepadIndex < 0 || gamepadIndex >= m_gamepadCount) {
    return;
  }
  m_gamepads[gamepadIndex].SetSensorTolerance(sensorID, xTolerance, yTolerance, zTolerance);
}

void BluepadGPIOBridge::SetSensorToleranceX(int gamepadIndex, Gamepad::SensorID sensorID, int tolerance) {
  if (gamepadIndex < 0 || gamepadIndex >= m_gamepadCount) {
    return;
  }
  m_gamepads[gamepadIndex].SetSensorToleranceX(sensorID, tolerance);
}

void BluepadGPIOBridge::SetSensorToleranceY(int gamepadIndex, Gamepad::SensorID sensorID, int tolerance) {
  if (gamepadIndex < 0 || gamepadIndex >= m_gamepadCount) {
    return;
  }
  m_gamepads[gamepadIndex].SetSensorToleranceY(sensorID, tolerance);
}

void BluepadGPIOBridge::SetSensorToleranceZ(int gamepadIndex, Gamepad::SensorID sensorID, int tolerance) {
  if (gamepadIndex < 0 || gamepadIndex >= m_gamepadCount) {
    return;
  }
  m_gamepads[gamepadIndex].SetSensorToleranceZ(sensorID, tolerance);
}

// ------------------- serial ingest -------------------

void BluepadGPIOBridge::ProcessSerial() {
  // Parsing does not require m_processorSerial; it's only for optional logs.
  if (!m_receiverSerial) {
    return;
  }

  static char   s_lineBuffer[s_bufferLength];
  static size_t s_lineLength = 0;
  static bool   s_isDroppingOverflow = false;

  static const char     s_dataPrefix[] = "DATA|";
  static constexpr size_t s_dataPrefixLength = sizeof(s_dataPrefix) - 1; // 5

  while (m_receiverSerial->available()) {
    const int incomingByte = m_receiverSerial->read();
    if (incomingByte < 0) {
      break;
    }

    const char incomingChar = static_cast<char>(incomingByte);

    // Treat CR or LF as a terminator (supports CRLF/LFCR).
    if (incomingChar == '\n' || incomingChar == '\r') {
      s_isDroppingOverflow = false;

      // Trim any trailing CR/LF and terminate.
      while (s_lineLength > 0 &&
             (s_lineBuffer[s_lineLength - 1] == '\r' || s_lineBuffer[s_lineLength - 1] == '\n')) {
        --s_lineLength;
      }
      s_lineBuffer[s_lineLength] = '\0';

      if (s_lineLength > 0) {
        const char* lineStart = s_lineBuffer;
        size_t lineLength = s_lineLength;

        // If it somehow still starts with NUL(s), skip them here too.
        while (lineLength > 0 && *lineStart == '\0') {
          ++lineStart;
          --lineLength;
        }

        const bool hasDataPrefix =
          (lineLength >= s_dataPrefixLength) &&
          (lineStart[0] == 'D') &&
          (lineStart[1] == 'A') &&
          (lineStart[2] == 'T') &&
          (lineStart[3] == 'A') &&
          (lineStart[4] == '|');

        if (hasDataPrefix) {
          const char* payloadPtr = lineStart + s_dataPrefixLength;
          const size_t payloadLength = lineLength - s_dataPrefixLength;
          ParseData(const_cast<char*>(payloadPtr), payloadLength);
        } else if (m_processorSerial != nullptr) {
          // Recognize status lines; allow variants like "CONNECTED: 0"
          if (StrEquals(lineStart, "OUT OF SLOTS") ||
              StrStartsWith(lineStart, "DISCONNECT") ||
              StrStartsWith(lineStart, "CONNECTED")) {
            m_processorSerial->println(lineStart);
          } else {
            m_processorSerial->print(F("UNRECOGNIZED MESSAGE: "));
            m_processorSerial->print(lineLength);
            m_processorSerial->print(F(" - '"));
            m_processorSerial->print(lineStart);          // may look empty if there's an early NUL
            m_processorSerial->print(F("' - "));
            PrintHex(m_processorSerial, lineStart, lineLength);
            m_processorSerial->println();
          }
        }
      }

      s_lineLength = 0;  // reset for next line
      continue;
    }

    // Ignore leading NULs at the start of a new line entirely.
    if (s_lineLength == 0 && incomingChar == '\0') {
      continue;
    }

    if (s_isDroppingOverflow) {
      continue;  // drop until we see a terminator
    }

    if (s_lineLength < s_bufferLength - 1) {
      s_lineBuffer[s_lineLength++] = incomingChar;
    } else {
      s_isDroppingOverflow = true;  // line too long; drop until '\r' or '\n'
    }
  }
}

void BluepadGPIOBridge::ParseData(char* payloadData, size_t payloadLength) {
  // Expected frame fields:
  // <gamepadIndex>|<dpadBits>|<mainBits>|<j1x>|<j1y>|<j2x>|<j2y>|<tr1>|<tr2>|<miscBits>|<s1x>|<s1y>|<s1z>|<s2x>|<s2y>|<s2z>
  static constexpr size_t s_expectedFieldCount = 16;  // 1 index + 15 payloads
  if (payloadLength == 0) {
    if(m_processorSerial) {
      m_processorSerial->println("Invalid Payload! (" + String(payloadLength) + ")");
    }
    return;
  }

  const char* fieldPointers[s_expectedFieldCount];
  size_t fieldLengths[s_expectedFieldCount];
  size_t fieldCount = 0;

  fieldPointers[0] = payloadData;
  fieldLengths[0] = 0;
  fieldCount = 1;

  size_t currentFieldLength = 0;
  for (size_t byteIndex = 0; byteIndex < payloadLength; ++byteIndex) {
    const char currentChar = payloadData[byteIndex];

    if (currentChar == '|') {
      fieldLengths[fieldCount - 1] = currentFieldLength;
      currentFieldLength = 0;

      if (fieldCount < s_expectedFieldCount) {
        fieldPointers[fieldCount] = payloadData + byteIndex + 1;
        fieldLengths[fieldCount] = 0;
        ++fieldCount;
      }
    } else {
      ++currentFieldLength;
    }
  }

  fieldLengths[fieldCount - 1] = currentFieldLength;

  ApplyFrame(fieldPointers, fieldLengths, fieldCount);
}



void BluepadGPIOBridge::ApplyFrame(const char** fieldPointers, const size_t* fieldLengths, size_t fieldCount) {
  static constexpr size_t s_expectedFieldCount = 16;
  if (fieldCount != s_expectedFieldCount) {
    if(m_processorSerial) {
      m_processorSerial->println("Invalid Field Count! (" + String(fieldCount) + "/" + String(s_expectedFieldCount) + ")");
    }
    return;
  }

  const int gamepadIndex = static_cast<int>(StrToInt(fieldPointers[0], fieldLengths[0]));
  if (gamepadIndex < 0 || gamepadIndex >= m_gamepadCount) {
    if(m_processorSerial) {
      m_processorSerial->println("Invalid Gamepad! (" + String(gamepadIndex) + ")");
    }
    return;
  }

  Gamepad::Gamepad& gamepad = m_gamepads[gamepadIndex];

  const uint8_t  dpadButtonMask = static_cast<uint8_t>( StrToInt(fieldPointers[1],  fieldLengths[1]) );
  const uint16_t mainButtonMask = static_cast<uint16_t>(StrToInt(fieldPointers[2],  fieldLengths[2]) );
  const int      leftStickX     = StrToInt(fieldPointers[3],  fieldLengths[3]);
  const int      leftStickY     = StrToInt(fieldPointers[4],  fieldLengths[4]);
  const int      rightStickX    = StrToInt(fieldPointers[5],  fieldLengths[5]);
  const int      rightStickY    = StrToInt(fieldPointers[6],  fieldLengths[6]);
  const int      trigger1Value  = StrToInt(fieldPointers[7],  fieldLengths[7]);
  const int      trigger2Value  = StrToInt(fieldPointers[8],  fieldLengths[8]);
  const uint8_t  miscButtonMask = static_cast<uint8_t>( StrToInt(fieldPointers[9],  fieldLengths[9]) );
  const int      sensor1X       = StrToInt(fieldPointers[10], fieldLengths[10]);
  const int      sensor1Y       = StrToInt(fieldPointers[11], fieldLengths[11]);
  const int      sensor1Z       = StrToInt(fieldPointers[12], fieldLengths[12]);
  const int      sensor2X       = StrToInt(fieldPointers[13], fieldLengths[13]);
  const int      sensor2Y       = StrToInt(fieldPointers[14], fieldLengths[14]);
  const int      sensor2Z       = StrToInt(fieldPointers[15], fieldLengths[15]);

  HandleDPad(gamepad, dpadButtonMask);
  HandleMainButtons(gamepad, mainButtonMask);
  HandleJoystick(gamepad, Gamepad::JoystickID::JOYSTICK_1, leftStickX,  leftStickY);
  HandleJoystick(gamepad, Gamepad::JoystickID::JOYSTICK_2, rightStickX, rightStickY);
  HandleTrigger(gamepad, Gamepad::TriggerID::TRIGGER_1, trigger1Value);
  HandleTrigger(gamepad, Gamepad::TriggerID::TRIGGER_2, trigger2Value);
  HandleMiscButtons(gamepad, miscButtonMask);
  HandleSensor(gamepad, Gamepad::SensorID::SENSOR_1, sensor1X, sensor1Y, sensor1Z);
  HandleSensor(gamepad, Gamepad::SensorID::SENSOR_2, sensor2X, sensor2Y, sensor2Z);
}



void BluepadGPIOBridge::HandleDPad(Gamepad::Gamepad& gamepad, uint8_t bitfield) {
  for (uint8_t i = 0; i < Gamepad::DPadButtons::Count(); ++i) {
    gamepad.SetButton(Gamepad::DPadButtons::buttonIDs[i], (bitfield >> i) & 1u);
  }
}

void BluepadGPIOBridge::HandleMainButtons(Gamepad::Gamepad& gamepad, uint16_t bitfield) {
  for (uint8_t i = 0; i < Gamepad::MainButtons::Count(); ++i) {
    gamepad.SetButton(Gamepad::MainButtons::buttonIDs[i], (bitfield >> i) & 1u);
  }
}

void BluepadGPIOBridge::HandleJoystick(Gamepad::Gamepad& gamepad, Gamepad::JoystickID joystickID, int xValue, int yValue) {
  gamepad.SetJoystick(joystickID, xValue, yValue);
}

void BluepadGPIOBridge::HandleTrigger(Gamepad::Gamepad& gamepad, Gamepad::TriggerID triggerID, int value) {
  gamepad.SetTrigger(triggerID, value);
}

void BluepadGPIOBridge::HandleMiscButtons(Gamepad::Gamepad& gamepad, uint8_t bitfield) {
  for (uint8_t i = 0; i < Gamepad::MiscButtons::Count(); ++i) {
    gamepad.SetButton(Gamepad::MiscButtons::buttonIDs[i], (bitfield >> i) & 1u);
  }
}

void BluepadGPIOBridge::HandleSensor(Gamepad::Gamepad& gamepad, Gamepad::SensorID sensorID, int xValue, int yValue, int zValue) {
  gamepad.SetSensor(sensorID, xValue, yValue, zValue);
}

inline void BluepadGPIOBridge::LogUnrecognizedIndex(int messageIndex, String data) {
  if (m_processorSerial) {
    m_processorSerial->print(F("Unrecognized Index "));
    m_processorSerial->print(messageIndex);
    m_processorSerial->print(F(" Data: "));
    m_processorSerial->println(data);
  }
}

inline long BluepadGPIOBridge::StrToInt(const char* text, size_t textLength) {
  if (text == nullptr || textLength == 0) {
    return 0;
  }

  const bool hasMinusSign = (text[0] == '-');
  const size_t startIndex = hasMinusSign ? 1u : 0u;

  if (startIndex >= textLength) {
    return 0;  // "-" only
  }

  long value = 0;
  for (size_t i = startIndex; i < textLength; ++i) {
    const char ch = text[i];
    if (ch < '0' || ch > '9') {
      break;
    }
    value = (value * 10) + (ch - '0');
  }

  if (hasMinusSign) {
    return -value;
  }
  return value;
}

inline long BluepadGPIOBridge::StrToInt(const char* text) {
  if (text == nullptr) {
    return 0;
  }
  size_t len = 0;
  while (text[len] != '\0') {
    ++len;
  }
  return StrToInt(text, len);
}

inline long BluepadGPIOBridge::StrToInt(const String& text) {
  return StrToInt(text.c_str(), static_cast<size_t>(text.length()));
}

inline bool BluepadGPIOBridge::StrEquals(const char* a, const char* b) {
  while (*a && (*a == *b)) {
    ++a;
    ++b;
  }
  return (*a == *b);
}

inline bool BluepadGPIOBridge::StrStartsWith(const char* str, const char* prefix) {
  while (*prefix) {
    if (*str++ != *prefix++) {
      return false;
    }
  }
  return true;
}


inline void BluepadGPIOBridge::PrintHex(HardwareSerial* output, const char* buffer, size_t bufferLength) {
  if (!output || !buffer) {
    return;
  }
  const char hex[] = "0123456789ABCDEF";
  for (size_t i = 0; i < bufferLength; ++i) {
    if (i) {
      output->print(' ');
    }
    uint8_t v = (uint8_t)buffer[i];
    // Two nibbles -> hex chars
    output->print(hex[v >> 4]);
    output->print(hex[v & 0x0F]);
  }
}