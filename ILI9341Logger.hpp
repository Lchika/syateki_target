/**
 * @file ILI9341Logger.hpp
 * @brief ログTFT表示用クラスヘッダ
 */

#ifndef ILI9341LOGGER_HPP
#define ILI9341LOGGER_HPP

#include <memory>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <Arduino.h>

/**
 * @class ILI9341Logger
 * @brief ログTFT表示用クラス
 */
class ILI9341Logger {
private:
  std::unique_ptr<Adafruit_ILI9341> _tft;
  uint8_t _line = 0;
  void resetScreenIfNeed(uint8_t msg_line_num = 0)
  {
    _line++;
    if(_line > 30){
      _tft->fillScreen(ILI9341_BLACK);
      _tft->setCursor(0, 0);
      _line = msg_line_num;
    }
  }

public:
  ILI9341Logger(SPIClass *spi, int8_t dc, int8_t cs, int8_t rst)
  :_tft(new Adafruit_ILI9341(spi, dc, cs, rst))
  {
    _tft->begin();
    _tft->fillScreen(ILI9341_BLACK);
    _tft->setTextColor(ILI9341_WHITE);
    _tft->setTextSize(1);
    _tft->setRotation(1);
  }
  void info(const String& msg)
  {
    resetScreenIfNeed(1);
    _tft->setTextColor(ILI9341_WHITE);
    _tft->println(msg);
  }
  void error(const String& msg)
  {
    info(msg);
  }
};

#endif