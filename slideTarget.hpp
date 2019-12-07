/**
 * @file slideTarget.hpp
 * @brief 横移動まと用クラスヘッダ
 */

#ifndef SLIDE_TARGET_HPP
#define SLIDE_TARGET_HPP

#include <memory>
#include <array>
#include <Arduino.h>
#include "led.hpp"
#include "ht16k33LED.hpp"

/**
 * @class SlideTarget
 * @brief 横移動まと用クラス
 */

enum HeadColor{
  clear = (0),
  red,
  green,
  blue
};

class SlideTarget {
private:
  uint8_t _target_id = 0;
  std::unique_ptr<Led> _eye;
  std::unique_ptr<Ht16k33led> _head;
  std::array<uint8_t, 3> color_array(HeadColor color) const;

public:
  SlideTarget(){}
  SlideTarget(uint8_t pin_eye_led)
    :_eye(new Led(pin_eye_led)){}
  //! target_idは0始まりで指定する
  SlideTarget(uint8_t pin_eye_led, uint8_t target_id, bool do_wire_begin = true)
    :_eye(new Led(pin_eye_led))
    ,_head(new Ht16k33led(0x70, target_id, do_wire_begin))
  {
    _head->init();
    _head->clear();
  }
  ~SlideTarget(){}
  void flash_eye(bool is_on) const;
  void blink_eye(uint32_t time = 100, uint32_t count = 3) const;
  void set_head_color(HeadColor color) const;
};

#endif