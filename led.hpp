#ifndef _LED_HPP_
#define _LED_HPP_

#include <Arduino.h>

class Led {
private:
  uint8_t _pin = 0;

public:
  Led(uint8_t pin):_pin(pin){
    digitalWrite(pin, LOW);
    pinMode(pin, OUTPUT);
  };
  void blink(uint32_t time = 100, uint32_t count = 3);
  void on(void);
  void off(void);
};

#endif