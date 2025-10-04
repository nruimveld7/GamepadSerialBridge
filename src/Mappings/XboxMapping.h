// src/Mappings/XboxMapping.h
#pragma once

#include "Gamepad/InputIDs.h"

namespace GSB {
  namespace Xbox {

    // ---- D-Pad (bit order must match your DPadButtons.buttonIDs table) ----
    namespace DPad {
      static constexpr ButtonID UP    = ButtonID::DPAD_1;
      static constexpr ButtonID DOWN  = ButtonID::DPAD_2;
      static constexpr ButtonID RIGHT = ButtonID::DPAD_3;
      static constexpr ButtonID LEFT  = ButtonID::DPAD_4;
    }

    // ---- Face/shoulders/sticks as BUTTONS (digital) ----
    namespace Buttons {
      // Face (South/East/West/North)
      static constexpr ButtonID A = ButtonID::MAIN_1;  // South
      static constexpr ButtonID B = ButtonID::MAIN_2;  // East
      static constexpr ButtonID X = ButtonID::MAIN_3;  // West
      static constexpr ButtonID Y = ButtonID::MAIN_4;  // North

      // Bumpers + trigger "clicks" (digital)
      static constexpr ButtonID LB = ButtonID::MAIN_5;
      static constexpr ButtonID RB = ButtonID::MAIN_6;
      static constexpr ButtonID LT = ButtonID::MAIN_7;
      static constexpr ButtonID RT = ButtonID::MAIN_8;

      // Stick buttons (L3/R3)
      static constexpr ButtonID LS = ButtonID::MAIN_9;
      static constexpr ButtonID RS = ButtonID::MAIN_10;

      // Main Buttons 11-16 (Not supported on tested controller)
    //   static constexpr ButtonID MAIN_11 = ButtonID::MAIN_11;
    //   static constexpr ButtonID MAIN_12 = ButtonID::MAIN_12;
    //   static constexpr ButtonID MAIN_13 = ButtonID::MAIN_13;
    //   static constexpr ButtonID MAIN_14 = ButtonID::MAIN_14;
    //   static constexpr ButtonID MAIN_15 = ButtonID::MAIN_15;
    //   static constexpr ButtonID MAIN_16 = ButtonID::MAIN_16;

      // Misc/system
      static constexpr ButtonID XBOX  = ButtonID::MISC_1;
      static constexpr ButtonID VIEW  = ButtonID::MISC_2;
      static constexpr ButtonID MENU  = ButtonID::MISC_3;
      static constexpr ButtonID SHARE = ButtonID::MISC_4;
    }

    // ---- Analog triggers (axes) ----
    namespace Triggers {
      static constexpr TriggerID LT  = TriggerID::TRIGGER_1;
      static constexpr TriggerID RT = TriggerID::TRIGGER_2;
    }

    // ---- Joysticks (axes) ----
    namespace Joysticks {
      static constexpr JoystickID LS  = JoystickID::JOYSTICK_1;
      static constexpr JoystickID RS = JoystickID::JOYSTICK_2;
    }

    // ---- Sensors (Not supported on tested controller) ----
    // namespace Sensors {
    //   static constexpr SensorID GYROSCOPE  = SensorID::SENSOR_1;
    //   static constexpr SensorID ACCELEROMETER = SensorID::SENSOR_2;
    // }

  } // namespace Xbox
} // namespace GSB