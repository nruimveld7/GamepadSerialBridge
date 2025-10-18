#include "internal/LinkBase.h"

namespace GSB {
  namespace internal {
    LinkBase::LinkBase(const LinkConfig& linkConfig) noexcept
      : m_gamepadCount(linkConfig.gamepadCount),
        m_uartConfig(linkConfig.uartConfig),
        m_linkSerial(linkConfig.linkSerial),
        m_logSerial(linkConfig.logSerial) {
      if (m_gamepadCount > s_maxControllers) {
        m_gamepadCount = s_maxControllers;
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
      ReadSerial();
    }

    uint8_t LinkBase::MaxControllers() noexcept {
      return s_maxControllers;
    }

    uint8_t LinkBase::GetGamepadCount() const noexcept {
      return m_gamepadCount;
    }

    Gamepad& LinkBase::GetGamepad(uint8_t index) noexcept {
      return m_gamepads[index];
    }

    const Gamepad& LinkBase::GetGamepad(uint8_t index) const noexcept {
      return m_gamepads[index];
    }

    HardwareSerial& LinkBase::GetLinkSerial() noexcept {
      return m_linkSerial;
    }

    Print& LinkBase::GetLogSerial() noexcept {
      return m_logSerial;
    }

    bool LinkBase::SendSerial(const uint8_t* data, size_t length) noexcept {
      if ((length != 0 && !data) || length > s_binaryMaxPayloadLength) {
        Log(F("Binary payload error: invalid length"));
        return false;
      }
      uint8_t raw[s_maxPacketSize];
      size_t pos = 0;
      raw[pos++] = s_protoVersion;
      if (length && data) {
        CopyBytes(raw + pos, data, length);
        pos += length;
      }
      const uint16_t crc = CRC16_CCITT(raw, pos);
      WriteLE16(raw + pos, crc);
      pos += 2;

      uint8_t encoded[s_maxEncodedSize];
      const size_t encodedLength = EncodeCOBS(raw, pos, encoded, s_maxEncodedSize);
      if (encodedLength == 0 || encodedLength > s_maxEncodedSize) {
        Log(F("COBS encode failed"));
        return false;
      }
      WriteSerial(encoded, encodedLength);
      const uint8_t delimiter = 0x00;
      WriteSerial(&delimiter, 1);
      return true;
    }

    uint8_t LinkBase::UInt8AtOffset(const uint8_t* data, size_t offset) noexcept {
      return data[offset];
    }

    uint16_t LinkBase::UInt16AtOffset(const uint8_t* data, size_t offset) noexcept {
      return ReadLE16(data + offset);
    }

    int16_t LinkBase::Int16AtOffset(const uint8_t* data, size_t offset) noexcept {
      return static_cast<int16_t>(ReadLE16(data + offset));
    }

    // ------------------- serial ingest -------------------
    void LinkBase::ReadSerial() noexcept {
      while (m_linkSerial.available()) {
        const int readByte = m_linkSerial.read();
        if (readByte < 0) {
          break;
        }
        const uint8_t byte = static_cast<uint8_t>(readByte);
        if (byte == 0x00) { // end-of-frame delimiter for COBS
          if (m_rxLength > 0) {
            // Decode and dispatch
            uint8_t raw[s_maxPacketSize];
            const size_t rawLen = DecodeCOBS(m_rxEncoded, m_rxLength, raw, s_maxPacketSize);
            m_rxLength = 0;
            if (rawLen >= s_headerSize + s_crcSize) {
              const size_t dataLen = rawLen - s_crcSize;
              const uint16_t receivedCRC = ReadLE16(raw + dataLen);
              const uint16_t calculatedCRC  = CRC16_CCITT(raw, dataLen);
              if (receivedCRC == calculatedCRC) {
                const uint8_t version = raw[0];
                if (version == s_protoVersion) {
                  const uint8_t* payload = raw + s_headerSize;
                  const size_t payloadLength = dataLen - s_headerSize;
                  ParseSerial(payload, payloadLength);
                } else {
                  Log(F("Bad protocol version"));
                }
              } else {
                Log(F("CRC mismatch"));
              }
            } else {
              // too short after decode; drop
            }
          }
        } else if (m_rxLength < sizeof(m_rxEncoded)) {
          m_rxEncoded[m_rxLength++] = byte;
        } else {
          // overflow: drop partial frame until next delimiter
          m_rxLength = 0;
          Log(F("RX overflow, dropping frame"));
        }
      }
    }

    void LinkBase::WriteSerial(const uint8_t* data, size_t length) noexcept {
      if (!data || !length) {
        return;
      }
      m_linkSerial.write(data, length);
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

    // ================= COBS =================
    size_t LinkBase::EncodeCOBS(const uint8_t* in, size_t length, uint8_t* out, size_t maxOut) noexcept {
      if (!in || !out || maxOut == 0) {
        return 0;
      }
      // Worst-case expansion: +1 code byte per 254 data bytes, +1 for the first code byte.
      const size_t worst = length + (length / 254) + 1;
      if (worst > maxOut) {
        return 0;
      }
      uint8_t* const outStart = out;
      // Reserve space for the first code byte
      if (maxOut < 1) {
        return 0;
      }
      uint8_t* codePtr = out++;
      size_t remaining = maxOut - 1;
      uint8_t code = 1;
      for (size_t i = 0; i < length; ++i) {
        const uint8_t byte = in[i];
        if (byte == 0) {
          // Close current block
          *codePtr = code;
          if (remaining == 0) {
            return 0;
          }
          codePtr = out++;
          --remaining;
          code = 1;
        } else {
          if (remaining == 0) {
            return 0;
          }
          *out++ = byte;
          --remaining;
          if (++code == 0xFF) {
            // Block full: emit code and start a new block
            *codePtr = code;
            if (remaining == 0) {
              return 0;
            }
            codePtr = out++;
            --remaining;
            code = 1;
          }
        }
      }
      // Finalize the last block
      *codePtr = code;
      return static_cast<size_t>(out - outStart);
    }

    size_t LinkBase::DecodeCOBS(const uint8_t* in, size_t length, uint8_t* out, size_t maxOut) noexcept {
      if (!in || !out) {
        return 0;
      }
      const uint8_t* end = in + length;
      size_t outLength = 0;
      while (in < end) {
        const uint8_t code = *in++;
        if (code == 0 || in + code - 1 > end) {
          return 0;
        }
        for (uint8_t i = 1; i < code; ++i) {
          if (outLength >= maxOut) {
            return 0;
          }
          out[outLength++] = *in++;
        }
        if (code != 0xFF && in < end) {
          if (outLength >= maxOut) {
            return 0;
          }
          out[outLength++] = 0;
        }
      }
      return outLength;
    }
  } // namespace internal
} // namespace GSB