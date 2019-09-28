#include <Arduino.h>
#include <Wire.h>
#include "SimpleWebServer.h"
#include "tone.h"
#include "debug.h"

static constexpr uint8_t BUZZER_PIN = 13;
static constexpr uint8_t EYE_LED_PIN = 25;
static constexpr int ADDRESS_IR_RECV_MOD = 8;

IPAddress ip(192, 168, 100, 125);        // for fixed IP Address
IPAddress gateway(192, 168, 100, 1);     //
IPAddress subnet(255, 255, 255, 0);      //
SimpleWebServer server("target", "12345678", ip, IPAddress(255,255,255,0), 80);
//SimpleWebServer *server;

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
  guide_recv_status(EYE_LED_PIN);
  delay(100);
}

void handle_root(){
  DebugPrint("-- GET /");
  byte target_num = get_target_num();
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

byte get_target_num(){
  Wire.requestFrom(ADDRESS_IR_RECV_MOD, 1);
  byte target_num = 0;
  while (Wire.available()) {
    target_num = Wire.read();
    DebugPrint("# Wire.read = %d", target_num);
  }
  return target_num;
}

void guide_recv_status(uint8_t led_pin){
  if(get_target_num() != 0){
    digitalWrite(led_pin, HIGH);
  }else{
    digitalWrite(led_pin, LOW);
  }
}