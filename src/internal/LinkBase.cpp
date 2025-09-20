#include "LinkBase.h"

namespace GSB {
  namespace internal {
    LinkBase::LinkBase(const LinkConfig& linkConfig)
      : m_gamepadCount(linkConfig.gamepadCount),
        m_uartConfig(linkConfig.uartConfig),
        m_logSerial(linkConfig.logSerial),
        m_linkSerial(linkConfig.linkSerial) {
      if (m_gamepadCount < 0) {
        m_gamepadCount = 0;
      }
      if (m_gamepadCount > MAXCONTROLLERS) {
        m_gamepadCount = MAXCONTROLLERS;
      }
      for (uint8_t i = 0; i < m_gamepadCount; ++i) {
        m_gamepads[i] = Gamepad(i);
      }
    }

    LinkBase::~LinkBase() {

    }

    bool LinkBase::Setup() {
      if(!m_linkSerial) {
        return false;
      }
      const int uartConfig = m_uartConfig.GetSerialConfig();
      if (uartConfig < 0) {
        Log(F("Invalid serial config"));
        return false;
      }
      m_linkSerial->begin(m_uartConfig.GetBaudRate(), m_uartConfig.GetSerialConfig());
      delay(2000);
      Log(F("Base Initialize"));
      return true;
    }

    void LinkBase::Loop() {
      ProcessSerial();
    }

    void LinkBase::SetParseBinary(Function<const uint8_t*, size_t> parseBinary) {
      if(parseBinary) {
        m_parseBinary = parseBinary;
      }
    }

    void LinkBase::SetParseASCII(Function<const char*, size_t> parseASCII) {
      if(parseASCII) {
        m_parseASCII = parseASCII;
      }
    }

    // ------------------- serial ingest -------------------
    void LinkBase::ProcessSerial() {
      if (m_linkSerial == nullptr) {
        return;
      }
      static const char s_dataPrefix[] = "DATA|";
      static constexpr size_t s_dataPrefixLength = sizeof(s_dataPrefix) - 1;
      while (m_linkSerial->available()) {
        // ----- Binary auto-detect by identifier -----
        int peekByte = m_linkSerial->peek();
        if (peekByte == s_binaryIdentifier) {
          if (m_linkSerial->available() < static_cast<int>(s_binaryTotalLength)) {
            break;
          }
          uint8_t packet[s_binaryTotalLength];
          for (size_t i = 0; i < s_binaryTotalLength; ++i) {
            packet[i] = static_cast<uint8_t>(m_linkSerial->read());
          }
          if (packet[1] != s_binaryPayloadLength) {
            Log(F("Invalid Binary Packet Length"));
            continue;
          }
          const uint16_t crcRx = (uint16_t)packet[s_binaryTotalLength - 2] | ((uint16_t)packet[s_binaryTotalLength - 1] << 8);
          const uint16_t crc = CRC16_CCITT(&packet[2], s_binaryPayloadLength);
          if (crc != crcRx) {
            Log(F("Invalid Binary Packet CRC"));
            continue;
          }

          // Provide only the 30-byte payload to the binary parser
          if(m_parseBinary) {
            m_parseBinary(&packet[2], s_binaryPayloadLength);
          }
          continue;
        }

        // ----- ASCII line assembly -----
        const int incomingByte = m_linkSerial->read();
        if (incomingByte < 0) {
          break;
        }
        const char incomingChar = static_cast<char>(incomingByte);
        if (incomingChar == '\n' || incomingChar == '\r') {
          m_isDroppingOverflow = false;
          // Trim trailing CR/LF/NUL and terminate
          while (m_lineLength > 0 && (
            m_lineBuffer[m_lineLength - 1] == '\r' ||
            m_lineBuffer[m_lineLength - 1] == '\n' ||
            m_lineBuffer[m_lineLength - 1] == '\0'
          )) {
            --m_lineLength;
          }
          m_lineBuffer[m_lineLength] = '\0';

          if (m_lineLength > 0) {
            const char* lineStart = m_lineBuffer;
            size_t lineLength = m_lineLength;
            // Skip leading NULs
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
              if(m_parseASCII) {
                m_parseASCII(payloadPtr, payloadLength);
              }
            } else {
              // Pass-through status lines or log unknowns
              if (StrEquals(lineStart, "OUT OF SLOTS") ||
                  StrStartsWith(lineStart, "DISCONNECT") ||
                  StrStartsWith(lineStart, "CONNECTED")) {
                Log(lineStart);
              } else {
                Log(F("UNRECOGNIZED MESSAGE:"));
                Log(lineStart);
                // Hex dump (keeps your existing helper)
                if (m_logSerial != nullptr) {
                  PrintHex(m_logSerial, lineStart, lineLength);
                  m_logSerial->println();
                }
              }
            }
          }
          m_lineLength = 0;
          continue;
        }
        // Ignore leading NULs
        if (m_lineLength == 0 && incomingChar == '\0') {
          continue;
        }
        if (m_isDroppingOverflow) {
          continue;
        }
        if (m_lineLength < s_bufferLength - 1) {
          m_lineBuffer[m_lineLength++] = incomingChar;
        } else {
          // Drop until terminator
          m_isDroppingOverflow = true;
        }
      }
    }

    inline uint16_t LinkBase::CRC16_CCITT(const uint8_t* data, size_t length) {
      uint16_t crc = 0xFFFF;
      for (size_t i = 0; i < length; ++i) {
        crc ^= static_cast<uint16_t>(data[i]) << 8;
        for (uint8_t bit = 0; bit < 8; ++bit) {
          if (crc & 0x8000) {
            crc = static_cast<uint16_t>((crc << 1) ^ 0x1021);
          } else {
            crc = static_cast<uint16_t>(crc << 1);
          }
        }
      }
      return crc;
    }

    inline bool LinkBase::StrToHexU16(const char* text, size_t length, uint16_t& outHex) {
      if (!text || length == 0) {
        return false;
      }
      uint32_t value = 0;
      for (size_t i = 0; i < length; ++i) {
        const char character = text[i];
        uint8_t nibble = 0xFF;
        if (character >= '0' && character <= '9') {
          nibble = static_cast<uint8_t>(character - '0');
        } else if (character >= 'a' && character <= 'f') {
          nibble = static_cast<uint8_t>(10 + (character - 'a'));
        } else if (character >= 'A' && character <= 'F') {
          nibble = static_cast<uint8_t>(10 + (character - 'A'));
        } else {
          return false;
        }
        value = (value << 4) | nibble;
        if (value > 0xFFFFu) {
          return false;
        }
      }
      outHex = static_cast<uint16_t>(value);
      return true;
    }

    inline long LinkBase::StrToInt(const char* text, size_t length) {
      if (text == nullptr || length == 0) {
        return 0;
      }
      const bool hasMinusSign = (text[0] == '-');
      const size_t startIndex = hasMinusSign ? 1u : 0u;
      if (startIndex >= length) {
        return 0;  // "-" only
      }
      long value = 0;
      for (size_t i = startIndex; i < length; ++i) {
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

    inline bool LinkBase::StrEquals(const char* a, const char* b) {
      while (*a && (*a == *b)) {
        ++a;
        ++b;
      }
      return (*a == *b);
    }

    inline bool LinkBase::StrStartsWith(const char* str, const char* prefix) {
      while (*prefix) {
        if (*str++ != *prefix++) {
          return false;
        }
      }
      return true;
    }


    inline void LinkBase::PrintHex(HardwareSerial* output, const char* buffer, size_t bufferLength) {
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

    inline uint8_t LinkBase::UInt8AtOffset(const uint8_t* data, size_t offset) {
      return data[offset];
    }

    inline uint16_t LinkBase::UInt16AtOffset(const uint8_t* data, size_t offset) {
      return static_cast<uint16_t>(data[offset]) | (static_cast<uint16_t>(data[offset + 1]) << 8);
    }

    inline int16_t LinkBase::Int16AtOffset(const uint8_t* data, size_t offset) {
      return static_cast<int16_t>(UInt16AtOffset(data, offset));
    }
  }
}