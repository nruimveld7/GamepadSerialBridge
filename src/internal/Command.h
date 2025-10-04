#pragma once

#include <Arduino.h>
#include "Utilities.h"

namespace GSB {
  namespace internal {
    struct Command {
      enum class OpCode : uint8_t {
        // System
        Disconnect = 0x01, // params: none

        // Rumble
        RumbleStart = 0x21, // params: force(u8), durationTicks(u8)
        RumbleStop = 0x22, // params: none

        // Player LEDs
        PlayerLedSet = 0x41, // params: illuminated(u8: 0/1)
        PlayerLedToggle = 0x42, // params: none
        PlayerLedsSetMask = 0x45, // params: mask(u8)
        PlayerLedsToggleMask = 0x46, // params: mask(u8)

        // Color LEDs
        ColorLedSet = 0x61, // params: illuminated(u8: 0/1)
        ColorLedSetColor = 0x62, // params: illuminated(u8: 0/1), r(u8), g(u8), b(u8)
        ColorLedToggle = 0x63  // params: none
      };

      struct Target {
        public:
          static Target Gamepad(uint8_t gamepadIndex) {
            Target target;
            target.m_value = gamepadIndex;
            return target;
          }
          
          static Target All() {
            Target target;
            target.m_value = m_all;
            return target;
          }

          constexpr inline uint8_t GetValue() const noexcept {
            return m_value;
          }
          
          constexpr bool IsAll() const noexcept {
            return m_value == m_all;
          }

          static constexpr uint8_t AllValue() noexcept {
            return m_all;
          }

        private:
          Target() = default;
          static constexpr uint8_t m_all = 0xFF;
          uint8_t m_value{};
      };

      struct Output {
        public:
          static Output ID(uint8_t outputID) {
            Output output;
            output.m_value = outputID;
            return output;
          }

          template <typename T>
          static Output ID(T outputID) {
            Output output;
            output.m_value = static_cast<uint8_t>(outputID);
            return output;
          }

          static Output All() {
            Output output;
            output.m_value = m_all;
            return output;
          }

          static Output None() {
            Output output;
            output.m_value = m_none;
            return output;
          }

          constexpr inline uint8_t GetValue() const noexcept {
            return m_value;
          }

          constexpr bool IsAll() const noexcept {
            return m_value == m_all;
          }

          constexpr bool IsNone() const noexcept {
            return m_value == m_none;
          }

          static constexpr uint8_t AllValue() noexcept {
            return m_all;
          }

          static constexpr uint8_t NoneValue() noexcept {
            return m_none;
          }

        private:
          Output() = default;
          static constexpr uint8_t m_none = 0xFE;
          static constexpr uint8_t m_all = 0xFF;
          uint8_t m_value{};
      };

      struct Parameters {
        struct None {
          // Intentionally Empty
        };

        struct Rumble {
          uint8_t force;
          uint8_t duration;
        };

        struct LedSet {
          uint8_t illuminated;
        };

        struct LedsMask  {
          uint8_t mask;
        };

        struct LedSetColor {
          uint8_t illuminated;
          uint8_t red;
          uint8_t green;
          uint8_t blue;
        };
      };

      struct Build {
        Build() = delete;

        static Command Disconnect(Target target) noexcept {
          Command command;
          command.m_opCode = OpCode::Disconnect;
          command.m_target = target;
          command.m_output = Output::None();
          return command;
        }

        static Command RumbleStart(Target target, Output output, Parameters::Rumble parameters) noexcept {
          Command command;
          command.m_opCode = OpCode::RumbleStart;
          command.m_target = target;
          command.m_output = output;
          command.m_parameters.rumble = parameters;
          return command;
        }

        static Command RumbleStop(Target target, Output output) noexcept {
          Command command;
          command.m_opCode = OpCode::RumbleStop;
          command.m_target = target;
          command.m_output = output;
          return command;
        }

        static Command PlayerLedSet(Target target, Output output, Parameters::LedSet parameters) noexcept {
          Command command;
          command.m_opCode = OpCode::PlayerLedSet;
          command.m_target = target;
          command.m_output = output;
          command.m_parameters.ledSet = parameters;
          return command;
        }

        static Command PlayerLedToggle(Target target, Output output) noexcept {
          Command command;
          command.m_opCode = OpCode::PlayerLedToggle;
          command.m_target = target;
          command.m_output = output;
          return command;
        }

        static Command PlayerLedsSetMask(Target target, Output output, Parameters::LedsMask parameters) noexcept {
          Command command;
          command.m_opCode = OpCode::PlayerLedsSetMask;
          command.m_target = target;
          command.m_output = output;
          command.m_parameters.ledsMask = parameters;
          return command;
        }

        static Command PlayerLedsToggleMask(Target target, Output output, Parameters::LedsMask parameters) noexcept {
          Command command;
          command.m_opCode = OpCode::PlayerLedsToggleMask;
          command.m_target = target;
          command.m_output = output;
          command.m_parameters.ledsMask = parameters;
          return command;
        }

        static Command ColorLedSet(Target target, Output output, Parameters::LedSet parameters) noexcept {
          Command command;
          command.m_opCode = OpCode::ColorLedSet;
          command.m_target = target;
          command.m_output = output;
          command.m_parameters.ledSet = parameters;
          return command;
        }

        static Command ColorLedSetColor(Target target, Output output, Parameters::LedSetColor parameters) noexcept {
          Command command;
          command.m_opCode = OpCode::ColorLedSetColor;
          command.m_target = target;
          command.m_output = output;
          command.m_parameters.ledSetColor = parameters;
          return command;
        }

        static Command ColorLedToggle(Target target, Output output) noexcept {
          Command command;
          command.m_opCode = OpCode::ColorLedToggle;
          command.m_target = target;
          command.m_output = output;
          return command;
        }
      };

      static constexpr size_t MaximumLength() noexcept {
        return 3u + sizeof(AppliedParameters);
      }

      OpCode GetOpCode() const noexcept {
        return m_opCode;
      }
      
      Target GetTarget() const noexcept {
        return m_target;
      }

      Output GetOutput() const noexcept {
        return m_output;
      }

      size_t Serialize(uint8_t* data, size_t maxLength) const noexcept {
        if (!data) {
          return 0;
        }
        size_t parameterLength = ParameterLength(m_opCode);
        size_t totalLength = 3u + parameterLength;
        if (maxLength < totalLength) {
          return 0;
        }
        data[0] = static_cast<uint8_t>(m_opCode);
        data[1] = m_target.GetValue();
        data[2] = m_output.GetValue();
        if (parameterLength > 0) {
          const uint8_t* parameters = ParameterData(m_opCode);
          if (!parameters) {
            return 0;
          }
          CopyBytes(data + 3, parameters, parameterLength);
        }
        return totalLength;
      }

      bool Deserialize(const uint8_t* data, size_t length) noexcept {
        if (!data || length < 3u) {
          return false;
        }
        const OpCode opCode = static_cast<OpCode>(data[0]);
        const uint8_t target = data[1];
        const uint8_t output = data[2];
        const size_t parameterLength = ParameterLength(opCode);
        const size_t totalLength = 3u + parameterLength;
        if (length != totalLength) {
          return false;
        }
        m_opCode = opCode;
        if (target == Target::AllValue()) {
          m_target = Target::All();
        } else {
          m_target = Target::Gamepad(target);
        }
        if (output == Output::AllValue()) {
          m_output = Output::All();
        } else if (output == Output::NoneValue()){
          m_output = Output::None();
        } else {
          m_output = Output::ID(output);
        }
        if (parameterLength > 0) {
          uint8_t* parameters = ParameterData(opCode);
          if (!parameters) {
            return false;
          }
          CopyBytes(parameters, data + 3, parameterLength);
        } else if (parameterLength == 0) {
          m_parameters = {};
        }
        return true;
      }

    private:
      OpCode m_opCode{OpCode::Disconnect};
      Target m_target{Target::All()};
      Output m_output{Output::None()};
      union AppliedParameters {
        Parameters::Rumble rumble;
        Parameters::LedSet ledSet;
        Parameters::LedsMask ledsMask;
        Parameters::LedSetColor ledSetColor;
      } m_parameters{};
      
      static constexpr size_t ParameterLength(OpCode opCode) noexcept {
        switch (opCode) {
          case OpCode::Disconnect: {
            return 0;
          }
          case OpCode::RumbleStart: {
            return sizeof(Parameters::Rumble);
          }
          case OpCode::RumbleStop: {
            return 0;
          }
          case OpCode::PlayerLedSet: {
            return sizeof(Parameters::LedSet);
          }
          case OpCode::PlayerLedToggle: {
            return 0;
          }
          case OpCode::PlayerLedsSetMask: {
            return sizeof(Parameters::LedsMask);
          }
          case OpCode::PlayerLedsToggleMask: {
            return sizeof(Parameters::LedsMask);
          }
          case OpCode::ColorLedSet: {
            return sizeof(Parameters::LedSet);
          }
          case OpCode::ColorLedSetColor: {
            return sizeof(Parameters::LedSetColor);
          }
          case OpCode::ColorLedToggle: {
            return 0;
          }
        }
        return 0;
      }

      const uint8_t* ParameterData(OpCode opCode) const noexcept {
        return const_cast<Command*>(this)->ParameterData(opCode);
      }

      uint8_t* ParameterData(OpCode opCode) noexcept {
        switch (opCode) {
          case OpCode::RumbleStart: {
            return reinterpret_cast<uint8_t*>(&m_parameters.rumble);
          }
          case OpCode::PlayerLedSet: {
            return reinterpret_cast<uint8_t*>(&m_parameters.ledSet);
          }
          case OpCode::PlayerLedsSetMask: {
            return reinterpret_cast<uint8_t*>(&m_parameters.ledsMask);
          }
          case OpCode::PlayerLedsToggleMask: {
            return reinterpret_cast<uint8_t*>(&m_parameters.ledsMask);
          }
          case OpCode::ColorLedSet: {
            return reinterpret_cast<uint8_t*>(&m_parameters.ledSet);
          }
          case OpCode::ColorLedSetColor: {
            return reinterpret_cast<uint8_t*>(&m_parameters.ledSetColor);
          }
          default: {
            return nullptr;
          }
        }
      }
    };

    static_assert(sizeof(Command::Parameters::Rumble) == 2, "Parameters::Rumble must be 2 bytes");
    static_assert(sizeof(Command::Parameters::LedSet) == 1, "Parameters::LedSet must be 1 byte");
    static_assert(sizeof(Command::Parameters::LedsMask) == 1, "Parameters::LedsMask must be 1 byte");
    static_assert(sizeof(Command::Parameters::LedSetColor) == 4, "Parameters::LedSetColor must be 4 bytes");
  } // namespace internal
} // namespace GSB
