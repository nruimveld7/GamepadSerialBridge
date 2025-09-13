#include <Bluepad32.h>

#define ATMEGATX 6
#define ATMEGARX 7

struct TxState {
  // Sticks
  int16_t leftStickX, leftStickY;
  int16_t rightStickX, rightStickY;

  // Buttons & hats
  uint16_t mainButtons;  // controller->buttons()
  uint8_t  dpadBits;     // controller->dpad()
  uint8_t  miscButtons;  // controller->miscButtons()

  // Triggers
  uint16_t brakeValue;     // controller->brake()
  uint16_t throttleValue;  // controller->throttle()

  // Sensors
  int16_t gyroX, gyroY, gyroZ;
  int16_t accelX, accelY, accelZ;

  bool initialized;
};

static TxState s_txStates[BP32_MAX_GAMEPADS];
ControllerPtr controllers[BP32_MAX_GAMEPADS];
bool debug = false;

void setup() {
  Serial1.begin(115200, SERIAL_8N1, ATMEGATX, ATMEGARX);
  Serial.begin(115200);

  Serial.printf("Firmware: %s\n", BP32.firmwareVersion());
  const uint8_t* addr = BP32.localBdAddress();
  Serial.printf("BD Addr: %2X:%2X:%2X:%2X:%2X:%2X\n", addr[0], addr[1], addr[2], addr[3], addr[4], addr[5]);

  BP32.setup(&OnConnect, &OnDisconnected);
  BP32.forgetBluetoothKeys();
  BP32.enableVirtualDevice(false);
}

void loop() {
  BP32.update();
  for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
    ControllerPtr controller = controllers[i];
    if (controller && controller->isConnected() && controller->isGamepad()) {
      ProcessGamepad(controller);
    }
  }
  delay(50);
}

void OnConnect(ControllerPtr controller) {
  bool foundEmptySlot = false;
  for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
    if (controllers[i] == nullptr) {
      Serial.print(F("CALLBACK: Controller is connected, index="));
      Serial.println(i);
      Serial1.print(F("CONNECTED: "));
      Serial1.println(i);

      ControllerProperties properties = controller->getProperties();
      Serial.printf("Controller model: %s, VID=0x%04x, PID=0x%04x\n",
                    controller->getModelName().c_str(), properties.vendor_id, properties.product_id);

      controllers[i] = controller;
      foundEmptySlot = true;
      break;
    }
  }
  if (!foundEmptySlot) {
    Serial.println(F("CALLBACK: Controller connected, but could not found empty slot"));
    Serial1.println(F("OUT OF SLOTS"));
  }
}

void OnDisconnected(ControllerPtr controller) {
  bool foundController = false;
  for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
    if (controllers[i] == controller) {
      Serial.print(F("CALLBACK: Controller disconnected from index="));
      Serial.println(i);
      Serial1.print(F("DISCONNECT: "));
      Serial1.println(i);
      controllers[i] = nullptr;
      foundController = true;
      break;
    }
  }
  if (!foundController) {
    Serial.println(F("CALLBACK: Controller disconnected, but not found in controllers"));
    Serial1.println(F("DISCONNECT: NOT FOUND"));
  }
}

static inline bool StateChanged(const TxState& s, ControllerPtr c) {
  return
    // Sticks
    s.leftStickX   != c->axisX()  ||
    s.leftStickY   != c->axisY()  ||
    s.rightStickX  != c->axisRX() ||
    s.rightStickY  != c->axisRY() ||

    // Buttons & hats
    s.mainButtons  != c->buttons()     ||
    s.dpadBits     != c->dpad()        ||
    s.miscButtons  != c->miscButtons() ||

    // Triggers
    s.brakeValue     != c->brake()     ||
    s.throttleValue  != c->throttle()  ||

    // Sensors
    s.gyroX   != c->gyroX()   ||
    s.gyroY   != c->gyroY()   ||
    s.gyroZ   != c->gyroZ()   ||
    s.accelX  != c->accelX()  ||
    s.accelY  != c->accelY()  ||
    s.accelZ  != c->accelZ();
}

static inline void TakeSnapshot(TxState& s, ControllerPtr c) {
  // Sticks
  s.leftStickX  = c->axisX();   s.leftStickY  = c->axisY();
  s.rightStickX = c->axisRX();  s.rightStickY = c->axisRY();

  // Buttons & hats
  s.mainButtons = c->buttons();
  s.dpadBits    = c->dpad();
  s.miscButtons = c->miscButtons();

  // Triggers
  s.brakeValue    = c->brake();
  s.throttleValue = c->throttle();

  // Sensors
  s.gyroX  = c->gyroX();   s.gyroY  = c->gyroY();   s.gyroZ  = c->gyroZ();
  s.accelX = c->accelX();  s.accelY = c->accelY();  s.accelZ = c->accelZ();

  s.initialized = true;
}

void ProcessGamepad(ControllerPtr controller) {
  const int index = controller->index();
  TxState& state = s_txStates[index];

  if (!state.initialized || StateChanged(state, controller)) {
    // 17 fields -> give generous room to avoid truncation
    char update[160];
    const int length = snprintf(
      update, sizeof(update),
      "DATA|%d|%u|%u|%d|%d|%d|%d|%d|%d|%u|%d|%d|%d|%d|%d|%d\n",
      index,
      controller->dpad(),
      controller->buttons(),
      controller->axisX(), controller->axisY(),
      controller->axisRX(), controller->axisRY(),
      controller->brake(), controller->throttle(),
      controller->miscButtons(),
      // Sensor 1: Gyroscope
      controller->gyroX(), controller->gyroY(), controller->gyroZ(),
      // Sensor 2: Accelerometer
      controller->accelX(), controller->accelY(), controller->accelZ()
    );

    if (length > 0 && static_cast<size_t>(length) < sizeof(update)) {
      Serial1.write(reinterpret_cast<const uint8_t*>(update), static_cast<size_t>(length));
    } else {
      // Optional: debug when truncated or error
      if (debug) {
        Serial.println(F("WARN: UART frame truncated or snprintf error"));
      }
    }

    TakeSnapshot(state, controller);
  }
}
