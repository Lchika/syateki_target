#include <Wire.h>
#include "irReceiver.hpp"

byte IrReceiver::read() const {
  Wire.requestFrom(_i2c_address, static_cast<uint8_t>(1));
  byte read_data = 0;
  while(Wire.available()){
    read_data = Wire.read();
  }
  return read_data;
}