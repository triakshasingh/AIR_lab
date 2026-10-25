#include <Wire.h>

const uint8_t imuAddress = 0x6A;
const uint8_t magnetometerAddress = 0x1C;
const uint8_t whoAmIRegister = 0x0F;
const uint8_t expectedImuId = 0x6C;
const uint8_t expectedMagnetometerId = 0x3D;

bool readRegister(uint8_t deviceAddress, uint8_t registerAddress, uint8_t &registerValue) {
  Wire.beginTransmission(deviceAddress);
  Wire.write(registerAddress);
  if (Wire.endTransmission(false) != 0) {
    return false;
  }
  if (Wire.requestFrom(deviceAddress, (size_t)1) != 1) {
    return false;
  }
  registerValue = Wire.read();
  return true;
}

void scanBus() {
  Serial.println("Scanning I2C bus...");
  int devicesFound = 0;
  for (uint8_t address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    if (Wire.endTransmission() == 0) {
      Serial.print("  device at 0x");
      Serial.println(address, HEX);
      devicesFound++;
    }
  }
  if (devicesFound == 0) {
    Serial.println("  nothing found, check wiring");
  }
}

void checkWhoAmI(const char *sensorName, uint8_t deviceAddress, uint8_t expectedId) {
  uint8_t receivedId = 0;
  Serial.print(sensorName);
  if (!readRegister(deviceAddress, whoAmIRegister, receivedId)) {
    Serial.println(": no response");
  } else if (receivedId == expectedId) {
    Serial.print(": OK (WHO_AM_I = 0x");
    Serial.print(receivedId, HEX);
    Serial.println(")");
  } else {
    Serial.print(": unexpected WHO_AM_I 0x");
    Serial.println(receivedId, HEX);
  }
}

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    delay(10);
  }
  Wire.begin();

  scanBus();
  checkWhoAmI("LSM6DSOX", imuAddress, expectedImuId);
  checkWhoAmI("LIS3MDL", magnetometerAddress, expectedMagnetometerId);
}

void loop() {
}