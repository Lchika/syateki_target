#include "ht16k33LED.hpp"

// 複数定義しないようにstatic変数はここで実体を定義
bool Ht16k33led::_initialized = false;

Ht16k33led::Ht16k33led(uint8_t address, uint8_t id, bool do_wire_begin)
:_address(address)
,_id(id)
{
  if(do_wire_begin){
    Wire.begin();
  }
}

void Ht16k33led::init(){
  if(_initialized){
    return;
  }
  // システムオシレータON
  Wire.beginTransmission(_address);
  Wire.write(0x21);
  Wire.endTransmission();
  // 点滅OFF
  Wire.beginTransmission(_address);
  Wire.write(0x81);
  Wire.endTransmission();
  // 明るさ1(0-15)
  Wire.beginTransmission(_address);
  Wire.write(0xE1);
  Wire.endTransmission();
  // 全消灯
  Wire.beginTransmission(_address);
  Wire.write(0x00);
  for(int i = 0; i < 8; i++){
    Wire.write(0x00);
    Wire.write(0x00);
  }
  Wire.endTransmission();
  _initialized = true;
}

void Ht16k33led::write_rgb(uint8_t r, uint8_t g, uint8_t b){
  uint16_t row = 0x0000 | (r << (_id * 3)) | (g << (_id * 3 + 1)) | (b << (_id * 3 + 2));
  Wire.beginTransmission(_address);
  Wire.write(_id * 2);
  Wire.write(static_cast<uint8_t>(row & 0x00FF));
  Wire.write((row >> 8));
  Wire.endTransmission();
}

void Ht16k33led::write_row(uint8_t com, uint8_t row1, uint8_t row2){
  Wire.beginTransmission(_address);
  Wire.write(com);
  Wire.write(row1);
  Wire.write(row2);
  Wire.endTransmission();
}

void Ht16k33led::clear(){
  write_rgb(0, 0, 0);
}
