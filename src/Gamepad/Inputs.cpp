#include "Gamepad/Inputs.h"

namespace GSB {
  // ----- Button -----
  bool Button::SetPressed(bool pressed) {
    if (m_pressed == pressed) {
      return false;
    }
    m_pressed = pressed;
    return true;
  }

  bool Button::GetPressed() const {
    return m_pressed;
  }

  // ----- Axis -----
  void Axis::SetTolerance(uint16_t tolerance) {
    if(tolerance == 0) {
      tolerance = 1;
    }
    if(tolerance > 32767) {
      tolerance = 32767;
    }
    m_tolerance = tolerance;
  }

  bool Axis::SetValue(int16_t value) {
    if (value == m_value) {
      return false;
    }
    int32_t difference  = static_cast<int32_t>(value) - static_cast<int32_t>(m_value);
    difference = (difference >= 0) ? difference : -difference; 
    if (difference >= static_cast<int32_t>(m_tolerance)) {
      m_value = value;
      return true;
    }
    return false;
  }

  int16_t Axis::GetValue() const {
    return m_value;
  }

  // ----- Trigger (1D axis + button) -----
  void Trigger::SetTolerance(uint16_t tolerance) {
    m_axis.SetTolerance(tolerance);
  }

  bool Trigger::SetValue(int16_t triggerVal) {
    return m_axis.SetValue(triggerVal);
  }

  int16_t Trigger::GetValue() const {
    return m_axis.GetValue();
  }

  Axis& Trigger::GetAxis() {
    return m_axis;
  }

  const Axis& Trigger::GetAxis() const {
    return m_axis;
  }

  // ----- Joystick (2D axis + button) -----
  void Joystick::SetToleranceX(uint16_t tolerance) {
    m_xAxis.SetTolerance(tolerance);
  }

  void Joystick::SetToleranceY(uint16_t tolerance) {
    m_yAxis.SetTolerance(tolerance);
  }

  bool Joystick::SetValueX(int16_t value) {
    return m_xAxis.SetValue(value);
  }

  bool Joystick::SetValueY(int16_t value) {
    return m_yAxis.SetValue(value);
  }

  int16_t Joystick::GetValueX() const {
    return m_xAxis.GetValue();
  }

  int16_t Joystick::GetValueY() const {
    return m_yAxis.GetValue();
  }

  Axis& Joystick::GetXAxis() {
    return m_xAxis;
  }

  const Axis& Joystick::GetXAxis() const {
    return m_xAxis;
  }

  Axis& Joystick::GetYAxis() {
    return m_yAxis;
  }

  const Axis& Joystick::GetYAxis() const {
    return m_yAxis;
  }

  // ----- Battery -----
  void Battery::SetTolerance(uint8_t tolerance) {
    if(tolerance == 0) {
      tolerance = 1;
    }
    m_tolerance = tolerance;
  }

  bool Battery::SetValue(uint8_t value) {
    if (value == m_value) {
      return false;
    }
    int16_t difference  = static_cast<int16_t>(value) - static_cast<int16_t>(m_value);
    difference = (difference >= 0) ? difference : -difference; 
    if (difference >= static_cast<int16_t>(m_tolerance)) {
      m_value = value;
      return true;
    }
    return false;
  }

  uint8_t Battery::GetValue() const {
    return m_value;
  }

  // ----- Sensor (3D axes, no button) -----
  void Sensor::SetToleranceX(uint16_t tolerance) {
    m_xAxis.SetTolerance(tolerance);
  }

  void Sensor::SetToleranceY(uint16_t tolerance) {
    m_yAxis.SetTolerance(tolerance);
  }

  void Sensor::SetToleranceZ(uint16_t tolerance) {
    m_zAxis.SetTolerance(tolerance);
  }

  bool Sensor::SetValueX(int16_t valueX) {
    return m_xAxis.SetValue(valueX);
  }

  bool Sensor::SetValueY(int16_t valueY) {
    return m_yAxis.SetValue(valueY);
  }

  bool Sensor::SetValueZ(int16_t valueZ) {
    return m_zAxis.SetValue(valueZ);
  }

  int16_t Sensor::GetValueX() const {
    return m_xAxis.GetValue();
  }

  int16_t Sensor::GetValueY() const {
    return m_yAxis.GetValue();
  }

  int16_t Sensor::GetValueZ() const {
    return m_zAxis.GetValue();
  }

  Axis& Sensor::GetXAxis() {
    return m_xAxis;
  }

  const Axis& Sensor::GetXAxis() const {
    return m_xAxis;
  }

  Axis& Sensor::GetYAxis() {
    return m_yAxis;
  }

  const Axis& Sensor::GetYAxis() const {
    return m_yAxis;
  }

  Axis& Sensor::GetZAxis() {
    return m_zAxis;
  }

  const Axis& Sensor::GetZAxis() const {
    return m_zAxis;
  }
} // namespace GSB