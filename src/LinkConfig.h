#pragma once

#include <Arduino.h>
#include "UartConfig.h"

namespace GSB {
  namespace internal {
    struct NullPrint final : Print {
      public:
      size_t write(uint8_t byte) override noexcept {
        return 1;
      }
      size_t write(const uint8_t* bytes, size_t byteCount) override noexcept {
        return byteCount;
      }
      static NullPrint& GetInstance() noexcept {
        static NullPrint nullPrint;
        return nullPrint;
      }
    private:
      NullPrint() = default;
      NullPrint(const NullPrint&) = delete;
      NullPrint& operator=(const NullPrint&) = delete;
    };
  }

  struct LinkConfig {
    int gamepadCount{1};
    UartConfig uartConfig{};
    HardwareSerial& linkSerial;
    Print& logSerial = internal::NullPrint::GetInstance();
  };
}