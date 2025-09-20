#include "LinkBase.h"

namespace GSB {
  namespace internal {
    LinkBase::LinkBase(const LinkConfig& linkConfig) noexcept
      : m_gamepadCount(linkConfig.gamepadCount),
        m_uartConfig(linkConfig.uartConfig),
        m_linkSerial(linkConfig.linkSerial),
        m_logSerial(linkConfig.logSerial) {
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

    bool LinkBase::Setup() noexcept {
      const int uartConfig = m_uartConfig.GetSerialConfig();
      if (uartConfig < 0) {
        Log(F("Invalid UART configuration"));
        return false;
      }
      m_linkSerial.begin(m_uartConfig.GetBaudRate(), uartConfig);
      return true;
    }

    void LinkBase::Loop() noexcept {
      ProcessSerial();
    }

    void LinkBase::SetParseBinary(Function<const uint8_t*, size_t> parseBinary) noexcept {
      m_parseBinary = parseBinary;
    }

    void LinkBase::SetParseASCII(Function<const char*, size_t> parseASCII) noexcept {
      m_parseASCII = parseASCII;
    }

    // ------------------- serial ingest -------------------
    void LinkBase::ProcessSerial() noexcept {
      static const char s_dataPrefix[] = "DATA|";
      static constexpr size_t s_dataPrefixLength = sizeof(s_dataPrefix) - 1;
      while (m_linkSerial.available()) {
        // ----- Binary auto-detect by identifier -----
        int peekByte = m_linkSerial.peek();
        if (peekByte == s_binaryIdentifier) {
          // Require the full frame before consuming anything:
+         // header(2) + payload(len) + crc(2).
          const size_t maxMessageLength = s_binaryHeaderLength + s_binaryMaxPayloadLength + s_binaryCRCLength;
          if (m_linkSerial.available() < static_cast<int>(maxMessageLength)) {
            break;
          }

          // Read the 2-byte header (id, len)
          uint8_t header[s_binaryHeaderLength];
          size_t headerBytes = m_linkSerial.readBytes(header, s_binaryHeaderLength);
          if (headerBytes != s_binaryHeaderLength) {
            break;
          }
          const uint8_t binaryIdentifier = header[0];
          const uint8_t messageLength = header[1];
          if(binaryIdentifier != s_binaryIdentifier || messageLength == 0 || messageLength > s_binaryMaxPayloadLength) {
            Log(F("Corrupt binary payload: identifier/length error"));
            continue;
          }

          // Ensure payload + crc are available
          const size_t remainingBytes = static_cast<size_t>(messageLength) + s_binaryCRCLength;
          if (m_linkSerial.available() < static_cast<int>(remainingBytes)) {
            break;
          }

          // Read payload + crc
          uint8_t message[s_binaryMaxPayloadLength + s_binaryCRCLength];
          size_t receivedBytes = m_linkSerial.readBytes(message, remainingBytes);
          if (receivedBytes != remainingBytes) {
            break;
          }
          const uint16_t receivedCRC = static_cast<uint16_t>(message[messageLength]) | (static_cast<uint16_t>(message[messageLength + 1]) << 8);
          const uint16_t calculatedCRC = CRC16_CCITT(message, messageLength);
          if (receivedCRC != calculatedCRC) {
            Log(F("Corrupt binary payload: CRC error"));
            continue;
          }

          // Process message
          if (m_parseBinary) {
            m_parseBinary(message, messageLength);
          }
          continue;
        }

        // ----- ASCII line assembly -----
        const int incomingByte = m_linkSerial.read();
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
                // Hex dump of raw line bytes
                PrintHex(m_logSerial, reinterpret_cast<const uint8_t*>(lineStart), lineLength);
                m_logSerial.println();
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

    uint16_t LinkBase::CRC16_CCITT(const uint8_t* data, size_t length) noexcept {
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

    bool LinkBase::StrToHexU16(const char* text, size_t length, uint16_t& outHex) noexcept {
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

    long LinkBase::StrToInt(const char* text, size_t length) noexcept {
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

    bool LinkBase::StrEquals(const char* a, const char* b) noexcept {
      while (*a && (*a == *b)) {
        ++a;
        ++b;
      }
      return (*a == *b);
    }

    bool LinkBase::StrStartsWith(const char* str, const char* prefix) noexcept {
      while (*prefix) {
        if (*str++ != *prefix++) {
          return false;
        }
      }
      return true;
    }


    void LinkBase::PrintHex(Print& out, const uint8_t* data, size_t length, bool spaced) noexcept {
      if (!data || !length) {
        return;
      }
      // Precomputed ASCII for nybbles
      static constexpr char HEX[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
      for (size_t i = 0; i < length; ++i) {
        if (spaced && i) {
          out.write(' ');
        }
        uint8_t byte = data[i];
        char pair[2] = {
          HEX[(byte >> 4) & 0x0F],
          HEX[byte & 0x0F]
        };
        out.write(reinterpret_cast<const uint8_t*>(pair), 2);
      }
    }

    uint8_t LinkBase::UInt8AtOffset(const uint8_t* data, size_t offset) noexcept {
      return data[offset];
    }

    uint16_t LinkBase::UInt16AtOffset(const uint8_t* data, size_t offset) noexcept {
      return static_cast<uint16_t>(data[offset]) | (static_cast<uint16_t>(data[offset + 1]) << 8);
    }

    int16_t LinkBase::Int16AtOffset(const uint8_t* data, size_t offset) noexcept {
      return static_cast<int16_t>(UInt16AtOffset(data, offset));
    }
  }
}