/**
 * @file slideTarget.hpp
 * @brief 横移動まと用クラスヘッダ
 */

#ifndef SLIDE_TARGET_HPP
#define SLIDE_TARGET_HPP

#include <memory>
#include <array>
#include <Arduino.h>
#include <NeoPixelBus.h>
#include "led.hpp"

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
  std::unique_ptr<Led> _eye;
  std::unique_ptr<NeoPixelBus<NeoRgbFeature, Neo800KbpsMethod>> _head;
  std::array<uint8_t, 3> color_array(HeadColor color) const;

public:
  SlideTarget(){}
  SlideTarget(uint8_t pin_eye_led)
    :_eye(new Led(pin_eye_led)){}
  SlideTarget(uint8_t pin_eye_led, uint8_t pin_head_led)
    :_eye(new Led(pin_eye_led))
    ,_head(new NeoPixelBus<NeoRgbFeature, Neo800KbpsMethod>(1, pin_head_led))
  {
    _head->Begin();
    _head->Show();
  }
  ~SlideTarget(){}
  void flash_eye(bool is_on) const;
  void blink_eye(uint32_t time = 100, uint32_t count = 3) const;
  void set_head_color(HeadColor color) const;
};

#endif