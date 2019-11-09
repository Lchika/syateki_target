/**
 * @file slideTarget.hpp
 * @brief 横移動まと用クラスヘッダ
 */

#ifndef SLIDE_TARGET_HPP
#define SLIDE_TARGET_HPP

#include <memory>
#include <array>
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
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
  std::unique_ptr<Adafruit_NeoPixel> _head;
  std::array<int, 3> color_array(HeadColor color) const;

public:
  SlideTarget(uint8_t pin_eye_led, uint8_t pin_head_led)
    :_eye(new Led(pin_eye_led))
    ,_head(new Adafruit_NeoPixel(1, pin_head_led, NEO_RGB + NEO_KHZ400)){
      digitalWrite(pin_head_led, LOW);
      pinMode(pin_head_led, OUTPUT);
      _head->begin();
      _head->setBrightness(30);
      _head->clear();
      _head->show();
    }
  void flash_eye(bool is_on) const;
  void blink_eye(uint32_t time = 100, uint32_t count = 3) const;
  void set_head_color(HeadColor color) const;
};

#endif