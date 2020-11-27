#include <array>
#include <memory>
#include <utility>
#include <vector>
#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include "SimpleWebServer.h"
#include "debug.h"
#include "irReceiver.hpp"
#include "slideTarget.hpp"
#include "rotaryDipSwitch.hpp"
#include "ILI9341Logger.hpp"

static constexpr uint8_t EYE_LED_PIN_1CH = 27;
//static constexpr uint8_t HEAD_LED_PIN_1CH = 26;
static constexpr int ADDRESS_IR_RECV_MOD_1CH = 8;
static constexpr uint8_t EYE_LED_PIN_2CH = 25;
//static constexpr uint8_t HEAD_LED_PIN_2CH = 23;
static constexpr int ADDRESS_IR_RECV_MOD_2CH = 9;
static constexpr uint8_t EYE_LED_PIN_3CH = 19;
static constexpr int ADDRESS_IR_RECV_MOD_3CH = 10;
static constexpr uint8_t TFT_DC_PIN = 17;
static constexpr uint8_t TFT_CS_PIN = 15;
static constexpr uint8_t TFT_RST_PIN = 16;
static constexpr uint8_t TARGET_NUM = 3;

struct Target{
  std::unique_ptr<IrReceiver> irReceiver;
  std::unique_ptr<SlideTarget> slideTarget;
};

IPAddress ip(192, 168, 100, 200);        // for fixed IP Address
IPAddress gateway(192, 168, 100, 1);     //
IPAddress subnet(255, 255, 255, 0);      //
//SimpleWebServer server("target", "12345678", ip, IPAddress(255,255,255,0), 80);
std::unique_ptr<SimpleWebServer> server;
//std::vector<Target> targets;
Target targets[TARGET_NUM];
//IrReceiver _irReceiver(ADDRESS_IR_RECV_MOD);
//SlideTarget _target(EYE_LED_PIN, HEAD_LED_PIN);
SPIClass hspi(HSPI);
std::unique_ptr<ILI9341Logger> tft_logger;

void setup(){
  BeginDebugPrint();

  tft_logger.reset(new ILI9341Logger(&hspi, TFT_DC_PIN, TFT_CS_PIN, TFT_RST_PIN));
  
  Wire.begin();
  info("Wire begun");

  init_target_val();

  RotaryDipSwitch rotaryDipSwitch({34, 36, 39, 35});
  const IPAddress ip(192, 168, 100, 200 + rotaryDipSwitch.read());
  server.reset(new SimpleWebServer(ip, IPAddress(255, 255, 255, 0), 80));

  WiFi.mode(WIFI_AP_STA);
  set_server();
  
  if (!WiFi.config(ip, gateway, subnet)) {
    info("STA Failed to configure");
  }
  WiFi.begin("your-ssid", "your-password");
  unsigned int try_connect_count = 0;
  while(WiFi.status() != WL_CONNECTED){
    try_connect_count++;
    if(try_connect_count > 20) break;
    delay(500);
    info(".");
  }
  if (WiFi.status() == WL_CONNECTED) {  
    info("");
    info("WiFi connected.");
    info("IP address: ");
    info(WiFi.localIP().toString().c_str());
  } else {
    info("WiFi connect process time out.");
  }
  //server = new SimpleWebServer("target", "12345678", IPAddress(192,168,100,116), IPAddress(255,255,255,0), 80);
  //server = new SimpleWebServer("target", "12345678", WiFi.localIP(), IPAddress(255,255,255,0), 80);
  info("set_server end");
  info("setup end");
}

void loop(){
  server->handle_request();
  guide_recv_status();
  delay(100);
}

void handle_root(){
  info("-- GET /");
  byte response_num = 0;
  for(int target_id = 0; target_id < TARGET_NUM; target_id++){
    byte gun_num = check_and_handle_hit(target_id);
    if(gun_num != 0){
      response_num = gun_num;
    }
  }
  String return_html = "target=" + String(response_num);
  server->send_html(200, return_html);
  return;
}

byte check_and_handle_hit(unsigned int target_id){
  byte gun_num = targets[target_id].irReceiver->read();
  info("gun_num = " + String(gun_num));
  //String return_html = "target=" + String(gun_num);
  //server->send_html(200, return_html);
  if(gun_num > 0){
    targets[target_id].slideTarget->blink_eye(100, 3);
  }
  return gun_num;
}

void set_server(){
  server->add_handler("/", handle_root);
  server->begin();
}

/*
void blink_led(uint8_t pin, uint32_t blink_time, uint32_t blink_count){
  for(int i = 0; i < blink_count; i++){
    digitalWrite(pin, HIGH);
    delay(blink_time);

    digitalWrite(pin, LOW);
    delay(blink_time);
  }
}
*/

void guide_recv_status(){
  for(const auto& t : targets){
    byte target_num = t.irReceiver->read();
    //info("target_num = %d", target_num);
    if(target_num > 0){
      t.slideTarget->flash_eye(true);
      t.slideTarget->set_head_color(_head_color(target_num));
    }else{
      t.slideTarget->flash_eye(false);
      t.slideTarget->set_head_color(HeadColor::clear);
    }
  }
}

HeadColor _head_color(byte target_num){
  std::map<byte, HeadColor> dict{
    {1, HeadColor::red},
    //{2, HeadColor::blue},
    //{3, HeadColor::blue},
    {4, HeadColor::green}
  };
  try{
    return dict.at(target_num);
  }catch(std::out_of_range&){
    info("<exception> std::out_of_range: input = " + String(target_num));
  }
  return HeadColor::clear;
}

void init_target_val(void){
  info("initialize target variable start.");
  //std::pair<IrReceiver, SlideTarget> t1
  //  = std::make_pair(IrReceiver(ADDRESS_IR_RECV_MOD_1CH), SlideTarget(EYE_LED_PIN_1CH, HEAD_LED_PIN_1CH));
  //std::pair<IrReceiver, SlideTarget> t2
  //  = std::make_pair(IrReceiver(ADDRESS_IR_RECV_MOD_2CH), SlideTarget(EYE_LED_PIN_2CH, HEAD_LED_PIN_2CH));
  //info("target push_back");
  //targets.push_back(std::move(t1));
  //targets.push_back(std::move(t2));
  targets[0].irReceiver.reset(new IrReceiver(ADDRESS_IR_RECV_MOD_1CH));
  targets[0].slideTarget.reset(new SlideTarget(EYE_LED_PIN_1CH, 0, false));
  targets[1].irReceiver.reset(new IrReceiver(ADDRESS_IR_RECV_MOD_2CH));
  targets[1].slideTarget.reset(new SlideTarget(EYE_LED_PIN_2CH, 1, false));
  targets[2].irReceiver.reset(new IrReceiver(ADDRESS_IR_RECV_MOD_3CH));
  targets[2].slideTarget.reset(new SlideTarget(EYE_LED_PIN_3CH, 2, false));
  info("initialize target variable end.");
}

void info(const String& msg){
  DebugPrint(msg);
  tft_logger->info(msg);
}

void error(const String& msg){
  DebugPrint(msg);
  tft_logger->error(msg);
}