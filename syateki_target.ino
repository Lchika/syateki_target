#include <Arduino.h>
#include <Wire.h>
#include "SimpleWebServer.h"
#include "tone.h"
#include "debug.h"
#include "irReceiver.hpp"
#include "slideTarget.hpp"

static constexpr uint8_t BUZZER_PIN = 13;
static constexpr uint8_t EYE_LED_PIN = 25;
static constexpr int ADDRESS_IR_RECV_MOD = 8;

IPAddress ip(192, 168, 100, 200);        // for fixed IP Address
IPAddress gateway(192, 168, 100, 1);     //
IPAddress subnet(255, 255, 255, 0);      //
SimpleWebServer server("target", "12345678", ip, IPAddress(255,255,255,0), 80);
//SimpleWebServer *server;
IrReceiver _irReceiver(ADDRESS_IR_RECV_MOD);
SlideTarget _target(EYE_LED_PIN);

void setup(){
  BeginDebugPrint();
  
  initialize_pin_mode();
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
  ledcSetup(1, 12000, 8);
  ledcAttachPin(BUZZER_PIN, 1);
  playmusic();
  DebugPrint("setup end");
}

void loop(){
  server.handle_request();
  guide_recv_status(_irReceiver, _target);
  delay(100);
}

void handle_root(){
  DebugPrint("-- GET /");
  byte target_num = _irReceiver.read();
  DebugPrint("target_num = %d", target_num);
  String return_html = "target=" + String(target_num);
  server.send_html(200, return_html);
  if(target_num == 1){
    blink_led(EYE_LED_PIN, 100, 3);
    playmusic();
  }
  return;
}

void set_server(){
  server.add_handler("/", handle_root);
  server.begin();
}

void playmusic(){
  ledcWriteTone(1,C4);
  delay(BEAT);
  ledcWriteTone(1,D4);
  delay(BEAT);
  ledcWriteTone(1,0);
}

void initialize_pin_mode(){
  digitalWrite(EYE_LED_PIN, LOW);
  pinMode(EYE_LED_PIN, OUTPUT);
}

void blink_led(uint8_t pin, uint32_t blink_time, uint32_t blink_count){
  for(int i = 0; i < blink_count; i++){
    digitalWrite(pin, HIGH);
    delay(blink_time);

    digitalWrite(pin, LOW);
    delay(blink_time);
  }
}

void guide_recv_status(const IrReceiver& irReceiver, const SlideTarget& target){
  if(irReceiver.read() != 0){
    target.flash_eye(true);
  }else{
    target.flash_eye(false);
  }
}