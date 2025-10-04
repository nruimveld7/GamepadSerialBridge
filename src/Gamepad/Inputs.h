#pragma once

#include <Arduino.h>

namespace GSB {
  class Button {
    public:
      Button() = default;
      bool SetPressed(bool pressed);
      bool GetPressed() const;

    private:
      bool m_pressed{false};
  };

  class Axis {
    public:
      Axis() = default;
      void SetTolerance(uint16_t tolerance);
      bool SetValue(int16_t value);
      int16_t GetValue() const;

    private:
      uint16_t m_tolerance{1};
      int16_t m_value{0};
  };

  class Trigger {
    public:
      Trigger() = default;
      void SetTolerance(uint16_t tolerance);
      bool SetValue(int16_t triggerVal);
      int16_t GetValue() const;
      Axis& GetAxis();
      const Axis& GetAxis() const;

    private:
      Axis m_axis;
  };

  class Joystick {
    public:
      Joystick() = default;
      void SetToleranceX(uint16_t tolerance);
      void SetToleranceY(uint16_t tolerance);
      bool SetValueX(int16_t value);
      bool SetValueY(int16_t value);
      int16_t GetValueX() const;
      int16_t GetValueY() const;
      Axis& GetXAxis();
      const Axis& GetXAxis() const;
      Axis& GetYAxis();
      const Axis& GetYAxis() const;

    private:
      Axis m_xAxis;
      Axis m_yAxis;
  };

  class Battery {
    public:
      Battery() = default;
      void SetTolerance(uint8_t tolerance);
      bool SetValue(uint8_t value);
      uint8_t GetValue() const;
    
    private:
      uint8_t m_tolerance{1};
      uint8_t m_value{0};
  };

  class Sensor {
    public:
      Sensor() = default;
      void SetToleranceX(uint16_t tolerance);
      void SetToleranceY(uint16_t tolerance);
      void SetToleranceZ(uint16_t tolerance);
      bool SetValueX(int16_t valueX);
      bool SetValueY(int16_t valueY);
      bool SetValueZ(int16_t valueZ);
      int16_t GetValueX() const;
      int16_t GetValueY() const;
      int16_t GetValueZ() const;
      Axis& GetXAxis();
      const Axis& GetXAxis() const;
      Axis& GetYAxis();
      const Axis& GetYAxis() const;
      Axis& GetZAxis();
      const Axis& GetZAxis() const;

    private:
      Axis m_xAxis;
      Axis m_yAxis;
      Axis m_zAxis;
  };
} // namespace GSB