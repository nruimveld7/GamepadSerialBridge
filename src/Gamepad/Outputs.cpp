#include "Gamepad/Outputs.h"

namespace GSB {
  // ----- Rumble -----
  void Rumble::Set(uint8_t force, uint8_t duration) {
    m_force = force;
    m_duration = duration;
  }
  
  uint8_t Rumble::GetForce() const {
    return m_force;
  }

  uint8_t Rumble::GetDuration() const {
    return m_duration;
  }

  // ----- Led -----
  bool Led::Set(bool illuminated) {
    if (m_illuminated == illuminated) {
      return false;
    }
    m_illuminated = illuminated;
    return true;
  }

  void Led::Toggle() {
    m_illuminated = !m_illuminated;
  }

  bool Led::GetIlluminated() const {
    return m_illuminated;
  }

  // ----- PlayerLed -----
  PlayerLed::PlayerLed(PlayerLedID id) : m_id(id) {

  }

  void PlayerLed::SetID(PlayerLedID id) {
    m_id = id;
  }

  PlayerLedID PlayerLed::GetID() const {
    return m_id;
  }

  bool PlayerLed::SetPlayer(uint8_t playerBitmask) {
    const uint8_t bit = PlayerLedIndex(m_id);
    if (bit >= PlayerLedCount() || bit >= 8) {
      return Set(false);
    }
    const bool illuminate = (playerBitmask & (uint8_t(1) << bit)) != 0;
    return Set(illuminate);
  }

  // ----- ColorLed -----
  bool ColorLed::SetColor(uint8_t red, uint8_t green, uint8_t blue) {
    Color color{red, green, blue};
    const bool illuminated = (red | green | blue) != 0;
    const bool colorChange = (m_color.red != color.red) || (m_color.green != color.green) || (m_color.blue != color.blue);
    const bool stateChange = (m_illuminated != illuminated);
    if (colorChange) {
      m_color = color;
    }
    if (stateChange) {
      m_illuminated = illuminated;
    }
    return colorChange || stateChange;
  }

  bool ColorLed::SetColor(Color color) {
    return SetColor(color.red, color.green, color.blue);
  }

  Color ColorLed::GetColor() const {
    return m_color;
  }
} // namespace GSB