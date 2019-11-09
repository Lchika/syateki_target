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
  if(color == clear){
    _head->clear();
  }else{
    std::array<int, 3> color_a = color_array(color);
    _head->setPixelColor(0, _head->Color(color_a[0], color_a[1], color_a[2]));
  }
  _head->show();
}

std::array<int, 3> SlideTarget::color_array(HeadColor color) const {
  std::map<HeadColor, std::array<int, 3>> dict{
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