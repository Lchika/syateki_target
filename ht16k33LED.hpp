/**
 * @brief ht16K33 フルカラーLED制御クラス
 */

#ifndef HT16K33LED_HPP
#define HT16K33LED_HPP

#include <array>
#include <Wire.h>
#include <Arduino.h>

/**
 * @class Ht16k33led
 * @brief
 */
class Ht16k33led {
private:
  static bool _initialized;
  uint8_t _address = 0;
  uint8_t _id = 0;

public:
  //! idは0始まりで指定する
  Ht16k33led(uint8_t address, uint8_t id, bool do_wire_begin = true);
  void init();
  //! r, g, bは0x00(OFF)か0x01(ON)で指定する
  void write_rgb(uint8_t r, uint8_t g, uint8_t b);
  void write_row(uint8_t com, uint8_t row1, uint8_t row2);
  void clear();
};

#endif