#pragma once

#include <Arduino.h>
#include "LinkConfig.h"
#include "UartConfig.h"
#include "Gamepad/InputIDs.h"
#include "Gamepad/Gamepad.h"
#include "internal/Status.h"
#include "internal/Command.h"
#include "internal/Utilities.h"

namespace GSB {
  namespace internal {
    class LinkBase {
      public:
        explicit LinkBase(const LinkConfig& linkConfig) noexcept;
        virtual ~LinkBase() noexcept = default;
        LinkBase(const LinkBase&) = delete;
        LinkBase& operator=(const LinkBase&) = delete;
        bool Setup() noexcept;
        void Loop() noexcept;
        static uint8_t MaxControllers() noexcept;

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
        static size_t EncodeCOBS(const uint8_t* in, size_t len, uint8_t* out, size_t maxOut) noexcept;
        static size_t DecodeCOBS(const uint8_t* in, size_t len, uint8_t* out, size_t maxOut) noexcept;

        // ============= Framing constants =============
        // Packet before COBS: [ver(1)][payload...][crc16(2)]
        static constexpr uint8_t s_protoVersion = 1;
        static constexpr size_t s_headerSize = 1;
        static constexpr size_t s_crcSize = 2;
        static constexpr size_t s_binaryMaxPayloadLength = 64;
        static constexpr size_t s_maxPacketSize = s_headerSize + s_binaryMaxPayloadLength + s_crcSize;
        // COBS worst-case growth ~= n/254 + 1; add a couple bytes for safety
        static constexpr size_t s_maxEncodedSize = s_maxPacketSize + (s_maxPacketSize/254) + 2;
        static constexpr uint8_t s_maxControllers = 4;

        uint8_t m_gamepadCount;
        Gamepad m_gamepads[s_maxControllers];
        UartConfig m_uartConfig;
        HardwareSerial& m_linkSerial;
        Print& m_logSerial;
        
        // RX accumulator for encoded bytes until 0x00 delimiter
        uint8_t m_rxEncoded[s_maxEncodedSize]{};
        size_t  m_rxLength{0};
    };
  } // namespace internal
} // namespace GSB