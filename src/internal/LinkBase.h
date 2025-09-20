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
        bool Setup();
        void Loop();

      protected:
        //Allow std::function style syntax without including the 'functional' library
        template <typename... Args>
        struct Function {
          using Delegate = void(*)(void*, Args...);
          void* context{nullptr};
          Delegate delegate{nullptr};
          inline void operator()(Args... args) const {
            if (delegate) {
              delegate(context, args...);
            }
          }
          explicit inline operator bool() const {
            return delegate != nullptr;
          }
        };

        //Allow derived classes to define how Binary/ASCII messages should be parsed
        inline void SetParseBinary(Function<const uint8_t*, size_t> parseBinary);
        inline void SetParseASCII(Function<const char*, size_t> parseASCII);

        template <typename T>
        inline void SetParseBinary(T* objectPtr, void (T::*memberPtr)(const uint8_t* data, size_t length)) {
          m_parseBinary = {
            objectPtr, +[](void* context, const uint8_t* dat, size_t len) {
              (static_cast<T*>(context)->*memberPtr)(dat, len);
            }
          };
        }

        template <typename T>
        inline void SetParseASCII(T* objectPtr, void (T::*memberPtr)(const char* data, size_t length)) {
          m_parseASCII = {
            objectPtr, +[](void* context, const char* dat, size_t len) {
              (static_cast<T*>(context)->*memberPtr)(dat, len);
            }
          };
        }

      private:
        void ProcessSerial();
        static inline uint16_t CRC16_CCITT(const uint8_t* data, size_t length);
        static inline bool StrToHexU16(const char* text, size_t length, uint16_t& outHex);
        static inline long StrToInt(const char* text, size_t length);
        static inline bool StrEquals(const char* a, const char* b);
        static inline bool StrStartsWith(const char* str, const char* prefix);
        static inline void PrintHex(HardwareSerial* output, const char* buffer, size_t bufferLength);
        static inline uint8_t UInt8AtOffset(const uint8_t* data, size_t offset);
        static inline uint16_t UInt16AtOffset(const uint8_t* data, size_t offset);
        static inline int16_t Int16AtOffset(const uint8_t* data, size_t offset);
        template<typename T>
        inline void Log(const T& value) {
          if (m_logSerial != nullptr) {
            m_logSerial->print(value);
            m_logSerial->println();
          }
        }

        Function<const uint8_t*, size_t> m_parseBinary{};
        Function<const char*, size_t> m_parseASCII{};

        static constexpr size_t s_bufferLength = 128;
        static constexpr uint8_t s_binaryIdentifier = 0xA5;
        static constexpr uint8_t s_binaryPayloadLength = 30;
        static constexpr size_t s_binaryTotalLength  = 1 + 1 + s_binaryPayloadLength + 2; //identifier + length + payload + crc

        int m_gamepadCount;
        Gamepad m_gamepads[MAXCONTROLLERS];
        UartConfig m_uartConfig;
        Stream& m_linkSerial;
        Print& m_logSerial;
        char m_lineBuffer[s_bufferLength]{};
        size_t m_lineLength = 0;
        bool m_isDroppingOverflow = false;
    };
  }
}