#pragma once

#define MAXCONTROLLERS 4

#include <Arduino.h>
#include "LinkConfig.h"
#include "UartConfig.h"
#include "Gamepad/InputIDs.h"
#include "Gamepad/Gamepad.h"

namespace GSB {
  namespace internal {
    struct Status {
      uint8_t gamepadIndex;
      uint8_t dpadMask;
      uint16_t mainButtonsMask;
      int16_t joystick1X;
      int16_t joystick1Y;
      int16_t joystick2X;
      int16_t joystick2Y;
      int16_t trigger1;
      int16_t trigger2;
      uint8_t miscButtonsMask;
      uint8_t battery;
      int16_t sensor1X;
      int16_t sensor1Y;
      int16_t sensor1Z;
      int16_t sensor2X;
      int16_t sensor2Y;
      int16_t sensor2Z;
    };

    enum class CommandOpCode : uint8_t {
      Rumble = 0x01, // p0=force, p1=durTicks, p2=0
      ColorLED = 0x02, // p0=R, p1=G, p2=B
      PlayerLEDs = 0x03, // p0=mask, p1=0, p2=0
      Disconnect = 0x7F, // no params
    };

    struct Command {
      uint8_t gamepadIndex;
      CommandOpCode opCode;
      uint8_t param1;
      uint8_t param2;
      uint8_t param3;
    };

    class LinkBase {
      public:
        explicit LinkBase(const LinkConfig& linkConfig) noexcept;
        virtual ~LinkBase() = default;
        LinkBase(const LinkBase&) = delete;
        LinkBase& operator=(const LinkBase&) = delete;
        bool Setup() noexcept;
        void Loop() noexcept;

      protected:
        // Protected access helpers for subclasses
        uint8_t GetGamepadCount() const noexcept;
        Gamepad& GetGamepad(uint8_t index) noexcept;
        const Gamepad& GetGamepad(uint8_t index) const noexcept;
        HardwareSerial& GetLinkSerial() noexcept;
        Print& GetLogSerial() noexcept;

        // Derived classes override these to handle parsed frames.
        // Default no-ops keep base usable without subclassing.
        virtual void ParseSerial(const uint8_t* data, size_t length) noexcept {}
        template<typename T>
        inline void Log(const T& value) noexcept {
          m_logSerial.println(value);
        }
        inline void Log(const __FlashStringHelper* message) noexcept {
          m_logSerial.println(message);
        }

        // Outbound helpers
        // Derived classes pass raw payloads; we frame + CRC + COBS for you
        bool SendSerial(const uint8_t* data, size_t length) noexcept;

        // Parsing helpers usable by subclasses
        static uint8_t UInt8AtOffset(const uint8_t* data, size_t offset) noexcept;
        static uint16_t UInt16AtOffset(const uint8_t* data, size_t offset) noexcept;
        static int16_t Int16AtOffset(const uint8_t* data, size_t offset) noexcept;

      private:
        void ReadSerial() noexcept;
        void WriteSerial(const uint8_t* data, size_t length) noexcept;
        static uint16_t CRC16_CCITT(const uint8_t* data, size_t length) noexcept;
        static size_t EncodeCOBS(const uint8_t* in, size_t len, uint8_t* out) noexcept;
        static size_t DecodeCOBS(const uint8_t* in, size_t len, uint8_t* out) noexcept;

        // ============= Framing constants =============
        // Packet before COBS: [ver(1)][type(1)][seq(1)][payload...][crc16(2)]
        static constexpr uint8_t s_protoVersion = 1;
        static constexpr size_t s_headerSize = 3;
        static constexpr size_t s_crcSize = 2;
        static constexpr size_t s_binaryMaxPayloadLength = 64;
        static constexpr size_t s_maxPacketSize = s_headerSize + s_binaryMaxPayloadLength + s_crcSize;
        // COBS worst-case growth ~= n/254 + 1; add a couple bytes for safety
        static constexpr size_t s_maxEncodedSize = s_maxPacketSize + (s_maxPacketSize/254) + 2;

        uint8_t m_gamepadCount;
        Gamepad m_gamepads[MAXCONTROLLERS];
        UartConfig m_uartConfig;
        HardwareSerial& m_linkSerial;
        Print& m_logSerial;
        
        // RX accumulator for encoded bytes until 0x00 delimiter
        uint8_t m_rxEncoded[s_maxEncodedSize]{};
        size_t  m_rxLength{0};
        // TX sequence (wrap is fine)
        uint8_t m_txSequence{0};
    };
  } // namespace internal
} // namespace GSB