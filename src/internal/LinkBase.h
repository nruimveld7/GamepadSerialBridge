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
        ~LinkBase() = default;
        LinkBase(const LinkBase&) = delete;
        LinkBase& operator=(const LinkBase&) = delete;
        bool Setup() noexcept;
        void Loop() noexcept;

      protected:
        //Lightweight delegate (no <functional>)
        template <typename... Args>
        struct Function {
          using Delegate = void(*)(void*, Args...);
          void* context{nullptr};
          Delegate delegate{nullptr};
          inline void operator()(Args... args) const noexcept {
            if (delegate) {
              delegate(context, args...);
            }
          }
          explicit inline operator bool() const noexcept {
            return delegate != nullptr;
          }
        };

        //Enable custom parsers
        void SetParseBinary(Function<const uint8_t*, size_t> parseBinary) noexcept;
        void SetParseASCII(Function<const char*, size_t> parseASCII) noexcept;

        //Enable custom class method parsers
        template <typename T, void (T::*Method)(const uint8_t* data, size_t length)>
        inline void SetParseBinary(T* objectPtr) noexcept {
          m_parseBinary = {
            objectPtr, +[](void* context, const uint8_t* dat, size_t len) {
              (static_cast<T*>(context)->*Method)(dat, len);
            }
          };
        }

        template <typename T, void (T::*Method)(const char* data, size_t length)>
        inline void SetParseASCII(T* objectPtr) noexcept {
          m_parseBinary = {
            m_parseASCII, +[](void* context, const char* dat, size_t len) {
              (static_cast<T*>(context)->*Method)(dat, len);
            }
          };
        }

        template<typename T>
        inline void Log(const T& value) {
          m_logSerial.println(value);
        }
        inline void Log(const __FlashStringHelper* message) {
          m_logSerial.println(message);
        }

      private:
        void ProcessSerial() noexcept;
        static uint16_t CRC16_CCITT(const uint8_t* data, size_t length) noexcept;
        static bool StrToHexU16(const char* text, size_t length, uint16_t& outHex) noexcept;
        static long StrToInt(const char* text, size_t length) noexcept;
        static bool StrEquals(const char* a, const char* b) noexcept;
        static bool StrStartsWith(const char* str, const char* prefix) noexcept;
        static void PrintHex(Print& out, const uint8_t* data, size_t length, bool spaced = true) noexcept;
        static uint8_t UInt8AtOffset(const uint8_t* data, size_t offset) noexcept;
        static uint16_t UInt16AtOffset(const uint8_t* data, size_t offset) noexcept;
        static int16_t Int16AtOffset(const uint8_t* data, size_t offset) noexcept;

        Function<const uint8_t*, size_t> m_parseBinary{};
        Function<const char*, size_t> m_parseASCII{};

        static constexpr size_t s_bufferLength = 128;
        static constexpr uint8_t s_binaryIdentifier = 0xA5;
        static constexpr size_t s_binaryHeaderLength = 2;
        static constexpr size_t s_binaryCRCLength = 2;
        static constexpr size_t s_binaryMaxPayloadLength = 64;

        int m_gamepadCount;
        Gamepad m_gamepads[MAXCONTROLLERS];
        UartConfig m_uartConfig;
        HardwareSerial& m_linkSerial;
        Print& m_logSerial;
        char m_lineBuffer[s_bufferLength]{};
        size_t m_lineLength{0};
        bool m_isDroppingOverflow{false};
    };
  } //namespace internal
} //namespace GSB