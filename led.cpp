#include "led.hpp"

void Led::blink(uint32_t time, uint32_t count){
  for(int i = 0; i < count; i++){
    digitalWrite(_pin, HIGH);
    delay(time);
    digitalWrite(_pin, LOW);
    delay(time);
  }
}

void Led::on(void){
  digitalWrite(_pin, HIGH);
}

void Led::off(void){
  digitalWrite(_pin, LOW);
}