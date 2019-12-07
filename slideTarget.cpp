#include <map>
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

void SlideTarget::set_head_color(HeadColor color) const {
  if(_head == NULL){
    return;
  }
  if(color == clear){
    _head->clear();
  }else{
    std::array<uint8_t, 3> color_a = color_array(color);
    _head->write_rgb(color_a[0], color_a[1], color_a[2]);
  }
}

std::array<uint8_t, 3> SlideTarget::color_array(HeadColor color) const {
  std::map<HeadColor, std::array<uint8_t, 3>> dict{
    {clear, {0, 0, 0}},
    {red, {1, 0, 0}},
    {green, {0, 1, 0}},
    {blue, {0, 0, 1}}
  };
  try {
    return dict.at(color);
  }
  catch(std::out_of_range&) {
  }
  return dict.at(clear);
}