#include <Arduino.h>
#include <BluepadGPIOBridge.h>

HardwareSerial* processorSerial = &Serial;
HardwareSerial* receiverSerial = &Serial2;
int resetPin = 35;
int numGamepads = 4;

UartConfig uartConfig {
  BaudRate::BAUD_115200,
  DataBits::DATA_8,
  StopBits::STOP_1,
  ParityBits::PARITY_NONE,
};

BluepadGPIOBridgeConfig config {
  uartConfig,
  processorSerial,
  receiverSerial,
  resetPin,
  numGamepads,
};

BluepadGPIOBridge bluepadBridge(config);

void setup() {
  processorSerial->begin(config.uartConfig.GetBaudRate());
  if(bluepadBridge.Setup())
  {
    AttachHandlers();
  }else
  {
    processorSerial->println(F("Bluepad Bridge Failed Initialization"));
  }
}

void loop() {
  ProcessSerialInput();
  bluepadBridge.Loop();
}

void ProcessSerialInput() {
  if(processorSerial->available()) {
    String input = processorSerial->readStringUntil('\n');
    if(input == "RESET\r") {
      bluepadBridge.Reset();
    }
  }
}

void AttachHandlers() {
  bluepadBridge.SetButtonOnPress(&ButtonPressed);
  bluepadBridge.SetButtonOnRelease(&ButtonReleased);
  bluepadBridge.SetTriggerOnChange(&TriggerChanged);
  bluepadBridge.SetJoystickOnChange(&JoystickChanged);
  bluepadBridge.SetSensorOnChange(&SensorChanged);
}

void ButtonPressed(int gamepadIndex, Gamepad::ButtonID buttonID) {
  if(!processorSerial) {
    return;
  }
  processorSerial->print("Gamepad ");
  processorSerial->print(gamepadIndex);
  processorSerial->print(" Button Pressed: ");
  switch(buttonID)
  {
    case Gamepad::Xbox::DPad::UP:
      processorSerial->println("^");
      break;
    case Gamepad::Xbox::DPad::DOWN:
      processorSerial->println("v");
      break;
    case Gamepad::Xbox::DPad::RIGHT:
      processorSerial->println(">");
      break;
    case Gamepad::Xbox::DPad::LEFT:
      processorSerial->println("<");
      break;
    case Gamepad::Xbox::Buttons::A:
      processorSerial->println("A");
      break;
    case Gamepad::Xbox::Buttons::B:
      processorSerial->println("B");
      break;
    case Gamepad::Xbox::Buttons::X:
      processorSerial->println("X");
      break;
    case Gamepad::Xbox::Buttons::Y:
      processorSerial->println("Y");
      break;
    case Gamepad::Xbox::Buttons::LB:
      processorSerial->println("L Bumper");
      break;
    case Gamepad::Xbox::Buttons::RB:
      processorSerial->println("R Bump");
      break;
    case Gamepad::Xbox::Buttons::LT:
      processorSerial->println("L Trigger");
      break;
    case Gamepad::Xbox::Buttons::RT:
      processorSerial->println("R Trigger");
      break;
    case Gamepad::Xbox::Buttons::LS:
      processorSerial->println("L Joystick");
      break;
    case Gamepad::Xbox::Buttons::RS:
      processorSerial->println("R Joystick");
      break;
    case Gamepad::Xbox::Buttons::XBOX:
      processorSerial->println("Xbox");
      break;
    case Gamepad::Xbox::Buttons::VIEW:
      processorSerial->println("View");
      break;
    case Gamepad::Xbox::Buttons::MENU:
      processorSerial->println("Menu");
      break;
    case Gamepad::Xbox::Buttons::SHARE:
      processorSerial->println("Share");
      break;
    default:
      processorSerial->println("Unhandled Input...");
      break;
  }
  processorSerial->println("");
}

void ButtonReleased(int gamepadIndex, Gamepad::ButtonID buttonID) {
  if(!processorSerial) {
    return;
  }
  processorSerial->print("Gamepad ");
  processorSerial->print(gamepadIndex);
  processorSerial->print(" Button Released: ");
  switch(buttonID)
  {
    case Gamepad::Xbox::DPad::UP:
      processorSerial->println("^");
      break;
    case Gamepad::Xbox::DPad::DOWN:
      processorSerial->println("v");
      break;
    case Gamepad::Xbox::DPad::RIGHT:
      processorSerial->println(">");
      break;
    case Gamepad::Xbox::DPad::LEFT:
      processorSerial->println("<");
      break;
    case Gamepad::Xbox::Buttons::A:
      processorSerial->println("A");
      break;
    case Gamepad::Xbox::Buttons::B:
      processorSerial->println("B");
      break;
    case Gamepad::Xbox::Buttons::X:
      processorSerial->println("X");
      break;
    case Gamepad::Xbox::Buttons::Y:
      processorSerial->println("Y");
      break;
    case Gamepad::Xbox::Buttons::LB:
      processorSerial->println("L Bumper");
      break;
    case Gamepad::Xbox::Buttons::RB:
      processorSerial->println("R Bump");
      break;
    case Gamepad::Xbox::Buttons::LT:
      processorSerial->println("L Trigger");
      break;
    case Gamepad::Xbox::Buttons::RT:
      processorSerial->println("R Trigger");
      break;
    case Gamepad::Xbox::Buttons::LS:
      processorSerial->println("L Joystick");
      break;
    case Gamepad::Xbox::Buttons::RS:
      processorSerial->println("R Joystick");
      break;
    case Gamepad::Xbox::Buttons::XBOX:
      processorSerial->println("Xbox");
      break;
    case Gamepad::Xbox::Buttons::VIEW:
      processorSerial->println("View");
      break;
    case Gamepad::Xbox::Buttons::MENU:
      processorSerial->println("Menu");
      break;
    case Gamepad::Xbox::Buttons::SHARE:
      processorSerial->println("Share");
      break;
    default:
      processorSerial->println("Unhandled Input...");
      break;
  }
  processorSerial->println("");
}

void TriggerChanged(int gamepadIndex, Gamepad::TriggerID triggerID, int newVal) {
  if(!processorSerial) {
    return;
  }
  processorSerial->print("Gamepad ");
  processorSerial->print(gamepadIndex);
  processorSerial->print(" Trigger Changed: ");
  switch(triggerID)
  {
    case Gamepad::Xbox::Triggers::LT:
      processorSerial->print("Left - ");
      processorSerial->println(newVal);
      break;
    case Gamepad::Xbox::Triggers::RT:
      processorSerial->print("Right - ");
      processorSerial->println(newVal);
      break;
    default:
      processorSerial->println("Unhandled Input...");
      break;
  }
  processorSerial->println("");
}

void JoystickChanged(int gamepadIndex, Gamepad::JoystickID joystickID, int xVal, int yVal) {
  if(!processorSerial) {
    return;
  }
  processorSerial->print("Gamepad ");
  processorSerial->print(gamepadIndex);
  processorSerial->print(" Joystick Changed: ");
  switch(joystickID)
  {
    case Gamepad::Xbox::Joysticks::LS:
      processorSerial->print("Left - (");
      processorSerial->print(xVal);
      processorSerial->print(", ");
      processorSerial->print(yVal);
      processorSerial->println(")");
      break;
    case Gamepad::Xbox::Joysticks::RS:
      processorSerial->print("Right - (");
      processorSerial->print(xVal);
      processorSerial->print(", ");
      processorSerial->print(yVal);
      processorSerial->println(")");
      break;
    default:
      processorSerial->println("Unhandled Input...");
      break;
  }
  processorSerial->println("");
}

void SensorChanged(int gamepadIndex, Gamepad::SensorID sensorID, int xVal, int yVal, int zVal) {
  if(!processorSerial) {
    return;
  }
  processorSerial->print("Gamepad ");
  processorSerial->print(gamepadIndex);
  processorSerial->print(" Sensor Changed: ");
  switch(sensorID)
  {
    case Gamepad::SensorID::SENSOR_1:
      processorSerial->print("Gyroscope? - (");
      processorSerial->print(xVal);
      processorSerial->print(", ");
      processorSerial->print(yVal);
      processorSerial->print(", ");
      processorSerial->print(zVal);
      processorSerial->println(")");
      break;
    case Gamepad::SensorID::SENSOR_2:
      processorSerial->print("Accelerometer? - (");
      processorSerial->print(xVal);
      processorSerial->print(", ");
      processorSerial->print(yVal);
      processorSerial->print(", ");
      processorSerial->print(zVal);
      processorSerial->println(")");
      break;
    default:
      processorSerial->println("Unhandled Input...");
      break;
  }
  processorSerial->println("");
}