#include "slideTarget.hpp"

void SlideTarget::flash_eye(bool is_on) const {
  if(is_on){
    _eye->on();
  }else{
    _eye->off();
  }
}

void SlideTarget::blink_eye(uint32_t time, uint32_t count) const {
  _eye->blink(time, count);
}