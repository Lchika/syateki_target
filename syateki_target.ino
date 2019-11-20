#include <array>
#include <memory>
#include <utility>
#include <vector>
#include <Arduino.h>
#include <Wire.h>
#include "SimpleWebServer.h"
#include "debug.h"
#include "irReceiver.hpp"
#include "slideTarget.hpp"
#include "rotaryDipSwitch.hpp"

static constexpr uint8_t EYE_LED_PIN_1CH = 27;
static constexpr uint8_t HEAD_LED_PIN_1CH = 26;
static constexpr int ADDRESS_IR_RECV_MOD_1CH = 8;
static constexpr uint8_t EYE_LED_PIN_2CH = 25;
static constexpr uint8_t HEAD_LED_PIN_2CH = 23;
static constexpr int ADDRESS_IR_RECV_MOD_2CH = 9;

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
Target targets[2];
//IrReceiver _irReceiver(ADDRESS_IR_RECV_MOD);
//SlideTarget _target(EYE_LED_PIN, HEAD_LED_PIN);

void setup(){
  BeginDebugPrint();

  init_target_val();

  RotaryDipSwitch rotaryDipSwitch({34, 36, 39, 35});
  const IPAddress ip(192, 168, 100, 200 + rotaryDipSwitch.read());
  server.reset(new SimpleWebServer(ip, IPAddress(255, 255, 255, 0), 80));

  WiFi.mode(WIFI_AP_STA);
  set_server();
  
  if (!WiFi.config(ip, gateway, subnet)) {
    DebugPrint("STA Failed to configure");
  }
  WiFi.begin("ROBOCON-AP1", "20190216-rc");
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    DebugPrint(".");
  }
  if (WiFi.status() == WL_CONNECTED) {  
    DebugPrint("");
    DebugPrint("WiFi connected.");
    DebugPrint("IP address: ");
    DebugPrint(WiFi.localIP().toString().c_str());
  } else {
    DebugPrint("WiFi connect process time out.");
  }
  //server = new SimpleWebServer("target", "12345678", IPAddress(192,168,100,116), IPAddress(255,255,255,0), 80);
  //server = new SimpleWebServer("target", "12345678", WiFi.localIP(), IPAddress(255,255,255,0), 80);
  DebugPrint("set_server end");
  Wire.begin();
  DebugPrint("setup end");
}

void loop(){
  server->handle_request();
  guide_recv_status();
  delay(100);
}

void handle_root(){
  DebugPrint("-- GET /");
  byte target_num = targets[0].irReceiver->read();
  //byte target_num = 0;
  DebugPrint("target_num = %d", target_num);
  String return_html = "target=" + String(target_num);
  server->send_html(200, return_html);
  if(target_num == 1){
    blink_led(EYE_LED_PIN_1CH, 100, 3);
  }
  return;
}

void set_server(){
  server->add_handler("/", handle_root);
  server->begin();
}

void blink_led(uint8_t pin, uint32_t blink_time, uint32_t blink_count){
  for(int i = 0; i < blink_count; i++){
    digitalWrite(pin, HIGH);
    delay(blink_time);

    digitalWrite(pin, LOW);
    delay(blink_time);
  }
}

void guide_recv_status(){
  for(const auto& t : targets){
    byte target_num = t.irReceiver->read();
    //DebugPrint("target_num = %d", target_num);
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
    {2, HeadColor::green},
    {3, HeadColor::blue}
  };
  try{
    return dict.at(target_num);
  }catch(std::out_of_range&){
    DebugPrint("<exception> std::out_of_range: input = %d", target_num);
  }
  return HeadColor::clear;
}

void init_target_val(void){
  DebugPrint("initialize target variable start.");
  //std::pair<IrReceiver, SlideTarget> t1
  //  = std::make_pair(IrReceiver(ADDRESS_IR_RECV_MOD_1CH), SlideTarget(EYE_LED_PIN_1CH, HEAD_LED_PIN_1CH));
  //std::pair<IrReceiver, SlideTarget> t2
  //  = std::make_pair(IrReceiver(ADDRESS_IR_RECV_MOD_2CH), SlideTarget(EYE_LED_PIN_2CH, HEAD_LED_PIN_2CH));
  //DebugPrint("target push_back");
  //targets.push_back(std::move(t1));
  //targets.push_back(std::move(t2));
  targets[0].irReceiver.reset(new IrReceiver(ADDRESS_IR_RECV_MOD_1CH));
  targets[0].slideTarget.reset(new SlideTarget(EYE_LED_PIN_1CH, HEAD_LED_PIN_1CH));
  targets[1].irReceiver.reset(new IrReceiver(ADDRESS_IR_RECV_MOD_2CH));
  targets[1].slideTarget.reset(new SlideTarget(EYE_LED_PIN_2CH));
  DebugPrint("initialize target variable end.");
}