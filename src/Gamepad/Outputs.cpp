#include "Outputs.h"

namespace GSB {
  // ----- Rumble -----
  Rumble::Rumble() : m_force(0), m_duration(0) {

  }

  bool Rumble::TurnOn(uint8_t force, uint8_t duration) {
    m_force = force;
    m_duration = duration;
    return true;
  }

  void Rumble::TurnOff() {
    m_force = 0;
    m_duration = 0;
    return true;
  }

  bool 
  // ----- LED -----
  LED::LED() : m_illuminated(false) {

  }

  bool LED::TurnOn() {
    if(m_illuminated) {
      return false;
    }
    m_illuminated = true;
    return true;
  }

  void LED::TurnOff() {
    if(!m_illuminated) {
      return false;
    }
    m_illuminated = false;
    return true;
  }

  // ----- ColorLED -----
  ColorLED::ColorLED() : m_red(0), m_green(0), m_blue(0), LED() {

  }

  bool ColorLED::SetColor(uint8_t red, uint8_t green, uint8_t blue) {

  }



} // namespace GSB