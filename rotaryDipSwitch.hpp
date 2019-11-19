/**
 * @file rotaryDipSwitch.hpp
 * @brief ロータリーDIPスイッチ用クラスヘッダ
 */

#ifndef ROTARY_DIP_SWITCH_HPP
#define ROTARY_DIP_SWITCH_HPP

#include <array>
#include <Arduino.h>

/**
 * @class RotaryDipSwitch
 * @brief ロータリーDIPスイッチ用クラス
 */

class RotaryDipSwitch {
private:
  const std::array<uint8_t, 4> _pins{};

public:
  //! 引数のpinsは{1桁目のピン, 2桁目のピン, 3桁目のピン, 4桁目のピン}という風に指定する
  RotaryDipSwitch(std::array<uint8_t, 4> pins):_pins(pins){
    for(uint8_t pin : pins){
      pinMode(pin, INPUT);
    }
  }
  uint8_t read(void) const{
    return static_cast<uint8_t>(digitalRead(_pins[0]) | (digitalRead(_pins[1]) << 1) | 
              (digitalRead(_pins[2]) << 2) | (digitalRead(_pins[3]) << 3));
  }
};

#endif