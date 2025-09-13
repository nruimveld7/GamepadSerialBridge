#pragma once

#include <Arduino.h>

namespace Gamepad {
  class Button {
    public:
      Button();
      bool SetPressed(bool pressed);
      bool GetPressed() const;

    private:
      bool m_pressed = false;
  };

  class Axis {
    public:
      Axis();
      void SetTolerance(int tolerance);
      bool SetValue(int value);
      int GetValue() const;

    private:
      int m_value = 0;
      int m_tolerance = 1;
  };

  class Trigger {
    public:
      Trigger();
      void SetTolerance(int tolerance);
      bool SetValue(int triggerVal);
      int GetValue() const;
      Axis& GetAxis();
      const Axis& GetAxis() const;

    private:
      Axis m_axis;
  };

  class Joystick {
    public:
      Joystick();
      void SetToleranceX(int tolerance);
      void SetToleranceY(int tolerance);
      bool SetValueX(int value);
      bool SetValueY(int value);
      int GetValueX() const;
      int GetValueY() const;
      Axis& GetXAxis();
      const Axis& GetXAxis() const;
      Axis& GetYAxis();
      const Axis& GetYAxis() const;

    private:
      Axis m_xAxis;
      Axis m_yAxis;
  };

  class Sensor {
    public:
      Sensor();
      void SetToleranceX(int tolerance);
      void SetToleranceY(int tolerance);
      void SetToleranceZ(int tolerance);
      bool SetValueX(int valueX);
      bool SetValueY(int valueY);
      bool SetValueZ(int valueZ);
      int GetValueX() const;
      int GetValueY() const;
      int GetValueZ() const;
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
}