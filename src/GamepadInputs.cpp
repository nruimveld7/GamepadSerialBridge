#include "GamepadInputs.h"
#include <stdlib.h> // abs()

namespace Gamepad {

// ----- Button -----
Button::Button()
  : m_pressed(false) {

}

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
Axis::Axis()
  : m_value(0), m_tolerance(1) {

}

void Axis::SetTolerance(int tolerance) {
  m_tolerance = tolerance;
}

bool Axis::SetValue(int value) {
  if (value == m_value) {
    return false;
  }
  if (abs(m_value - value) >= m_tolerance) {
    m_value = value;
    return true;
  }
  return false;
}

int Axis::GetValue() const {
  return m_value;
}

// ----- Trigger (1D axis + button) -----
Trigger::Trigger()
  : m_axis() {

}

void Trigger::SetTolerance(int tolerance) {
  m_axis.SetTolerance(tolerance);
}

bool Trigger::SetValue(int triggerVal) {
  return m_axis.SetValue(triggerVal);
}

int Trigger::GetValue() const {
  return m_axis.GetValue();
}

Axis& Trigger::GetAxis() {
  return m_axis;
}

const Axis& Trigger::GetAxis() const {
  return m_axis;
}

// ----- Joystick (2D axis + button) -----
Joystick::Joystick()
  : m_xAxis(), m_yAxis() {

}

void Joystick::SetToleranceX(int tolerance) {
  m_xAxis.SetTolerance(tolerance);
}

void Joystick::SetToleranceY(int tolerance) {
  m_yAxis.SetTolerance(tolerance);
}

bool Joystick::SetValueX(int value) {
  return m_xAxis.SetValue(value);
}

bool Joystick::SetValueY(int value) {
  return m_yAxis.SetValue(value);
}

int Joystick::GetValueX() const {
  return m_xAxis.GetValue();
}

int Joystick::GetValueY() const {
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

// ----- Sensor (3D axes, no button) -----
Sensor::Sensor()
  : m_xAxis(), m_yAxis(), m_zAxis() {

}

void Sensor::SetToleranceX(int tolerance) {
  m_xAxis.SetTolerance(tolerance);
}

void Sensor::SetToleranceY(int tolerance) {
  m_yAxis.SetTolerance(tolerance);
}

void Sensor::SetToleranceZ(int tolerance) {
  m_zAxis.SetTolerance(tolerance);
}

bool Sensor::SetValueX(int valueX) {
  return m_xAxis.SetValue(valueX);
}

bool Sensor::SetValueY(int valueY) {
  return m_yAxis.SetValue(valueY);
}

bool Sensor::SetValueZ(int valueZ) {
  return m_zAxis.SetValue(valueZ);
}

int Sensor::GetValueX() const {
  return m_xAxis.GetValue();
}

int Sensor::GetValueY() const {
  return m_yAxis.GetValue();
}

int Sensor::GetValueZ() const {
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

} // namespace Gamepad