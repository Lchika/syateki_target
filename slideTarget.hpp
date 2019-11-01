/**
 * @file slideTarget.hpp
 * @brief 横移動まと用クラスヘッダ
 */

#ifndef SLIDE_TARGET_HPP
#define SLIDE_TARGET_HPP

#include <memory>
#include <Arduino.h>
#include "led.hpp"

/**
 * @class SlideTarget
 * @brief 横移動まと用クラス
 */
class SlideTarget {
private:
  std::unique_ptr<Led> _eye;

public:
  SlideTarget(uint8_t pin_eye_led):_eye(new Led(pin_eye_led)){}
  void flash_eye(bool is_on) const;
  void blink_eye(uint32_t time = 100, uint32_t count = 3) const;
};

#endif