#pragma once

#include <Arduino.h>
#include "UartConfig.h"

namespace GSB {
  struct LinkConfig {
    UartConfig uartConfig{};
    Stream& linkSerial;
    Print& logSerial;
    int gamepadCount{1};
  };
}