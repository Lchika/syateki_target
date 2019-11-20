#include <map>
#include "slideTarget.hpp"

RgbColor black(0);

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
    _head->ClearTo(black);
  }else{
    std::array<uint8_t, 3> color_a = color_array(color);
    RgbColor rgb_color(color_a[0], color_a[1], color_a[2]);
    _head->SetPixelColor(0, rgb_color);
  }
  _head->Show();
}

std::array<uint8_t, 3> SlideTarget::color_array(HeadColor color) const {
  std::map<HeadColor, std::array<uint8_t, 3>> dict{
    {clear, {0, 0, 0}},
    {red, {150, 0, 0}},
    {green, {0, 150, 0}},
    {blue, {0, 0, 150}}
  };
  try {
    return dict.at(color);
  }
  catch(std::out_of_range&) {
  }
  return dict.at(clear);
}