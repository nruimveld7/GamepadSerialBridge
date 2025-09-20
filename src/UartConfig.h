#pragma once

#include <Arduino.h>

namespace GSB {
  enum class BaudRate : uint32_t {
    BAUD_4800 = 4800UL,
    BAUD_9600 = 9600UL,
    BAUD_19200 = 19200UL,
    BAUD_31250 = 31250UL,
    BAUD_38400 = 38400UL,
    BAUD_57600 = 57600UL,
    BAUD_74880 = 74880UL,
    BAUD_115200 = 115200UL,
  };

  enum class DataBits : uint8_t {
    DATA_5 = 5,
    DATA_6 = 6,
    DATA_7 = 7,
    DATA_8 = 8,
  };

  enum class StopBits : uint8_t {
    STOP_1 = 1,
    STOP_2 = 2,
  };

  enum class ParityBits : uint8_t {
    PARITY_NONE = 0,
    PARITY_ODD = 1,
    PARITY_EVEN = 2,
  };

  struct UartConfig {
    constexpr UartConfig(
      BaudRate br = BaudRate::BAUD_115200,
      DataBits db = DataBits::DATA_8,
      StopBits sb = StopBits::STOP_1,
      ParityBits pb = ParityBits::PARITY_NONE
    ) : baudRate(br), dataBits(db), stopBits(sb), parityBits(pb) {

    }

    BaudRate baudRate{BaudRate::BAUD_115200};
    DataBits dataBits{DataBits::DATA_8};
    StopBits stopBits{StopBits::STOP_1};
    ParityBits parityBits{ParityBits::PARITY_NONE};

    unsigned long GetBaudRate() const {
      return static_cast<unsigned long>(baudRate);
    }

    // Lookup Arduino SERIAL_* config constant
    int GetSerialConfig() const {
      struct Entry {
        DataBits db;
        ParityBits pb;
        StopBits sb;
        int cfg;
      };
      constexpr Entry table[] = {
        { DataBits::DATA_5, ParityBits::PARITY_NONE, StopBits::STOP_1, SERIAL_5N1 },
        { DataBits::DATA_5, ParityBits::PARITY_NONE, StopBits::STOP_2, SERIAL_5N2 },
        { DataBits::DATA_5, ParityBits::PARITY_ODD,  StopBits::STOP_1, SERIAL_5O1 },
        { DataBits::DATA_5, ParityBits::PARITY_ODD,  StopBits::STOP_2, SERIAL_5O2 },
        { DataBits::DATA_5, ParityBits::PARITY_EVEN, StopBits::STOP_1, SERIAL_5E1 },
        { DataBits::DATA_5, ParityBits::PARITY_EVEN, StopBits::STOP_2, SERIAL_5E2 },

        { DataBits::DATA_6, ParityBits::PARITY_NONE, StopBits::STOP_1, SERIAL_6N1 },
        { DataBits::DATA_6, ParityBits::PARITY_NONE, StopBits::STOP_2, SERIAL_6N2 },
        { DataBits::DATA_6, ParityBits::PARITY_ODD,  StopBits::STOP_1, SERIAL_6O1 },
        { DataBits::DATA_6, ParityBits::PARITY_ODD,  StopBits::STOP_2, SERIAL_6O2 },
        { DataBits::DATA_6, ParityBits::PARITY_EVEN, StopBits::STOP_1, SERIAL_6E1 },
        { DataBits::DATA_6, ParityBits::PARITY_EVEN, StopBits::STOP_2, SERIAL_6E2 },

        { DataBits::DATA_7, ParityBits::PARITY_NONE, StopBits::STOP_1, SERIAL_7N1 },
        { DataBits::DATA_7, ParityBits::PARITY_NONE, StopBits::STOP_2, SERIAL_7N2 },
        { DataBits::DATA_7, ParityBits::PARITY_ODD,  StopBits::STOP_1, SERIAL_7O1 },
        { DataBits::DATA_7, ParityBits::PARITY_ODD,  StopBits::STOP_2, SERIAL_7O2 },
        { DataBits::DATA_7, ParityBits::PARITY_EVEN, StopBits::STOP_1, SERIAL_7E1 },
        { DataBits::DATA_7, ParityBits::PARITY_EVEN, StopBits::STOP_2, SERIAL_7E2 },

        { DataBits::DATA_8, ParityBits::PARITY_NONE, StopBits::STOP_1, SERIAL_8N1 },
        { DataBits::DATA_8, ParityBits::PARITY_NONE, StopBits::STOP_2, SERIAL_8N2 },
        { DataBits::DATA_8, ParityBits::PARITY_ODD,  StopBits::STOP_1, SERIAL_8O1 },
        { DataBits::DATA_8, ParityBits::PARITY_ODD,  StopBits::STOP_2, SERIAL_8O2 },
        { DataBits::DATA_8, ParityBits::PARITY_EVEN, StopBits::STOP_1, SERIAL_8E1 },
        { DataBits::DATA_8, ParityBits::PARITY_EVEN, StopBits::STOP_2, SERIAL_8E2 },
      };

      for (auto&& entry : table) {
        if (entry.db == dataBits && entry.pb == parityBits && entry.sb == stopBits) {
          return entry.cfg;
        }
      }
      return -1; // invalid combo
    }
  };
}