#include <Arduino.h>
#include <Wire.h>
#include "SimpleWebServer.h"

IPAddress ip(192, 168, 100, 125);        // for fixed IP Address
IPAddress gateway(192, 168, 100, 1);     //
IPAddress subnet(255, 255, 255, 0);      //
SimpleWebServer server("target", "12345678", ip, IPAddress(255,255,255,0), 80);
//SimpleWebServer *server;

void setup(){
  Serial.begin(115200);
  
  WiFi.mode(WIFI_AP_STA);
  set_server();
  
  if (!WiFi.config(ip, gateway, subnet)) {
    Serial.println("STA Failed to configure");
  }
  WiFi.begin("your-SSID", "your-PASSWORD");
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  if (WiFi.status() == WL_CONNECTED) {  
    Serial.println("");
    Serial.println("WiFi connected.");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("WiFi connect process time out.");
  }
  //server = new SimpleWebServer("target", "12345678", IPAddress(192,168,100,116), IPAddress(255,255,255,0), 80);
  //server = new SimpleWebServer("target", "12345678", WiFi.localIP(), IPAddress(255,255,255,0), 80);
  Serial.println("set_server end");
  Wire.begin();
  Serial.println("setup end");
}

void loop(){
  server.handle_request();
  delay(100);
}

void handle_root(){
  Serial.println("-- GET /");
  Wire.requestFrom(8, 1);
  while (Wire.available()) {
    byte b = Wire.read();
    Serial.print("# Wire.read = ");
    Serial.println(b);
    if(b == 1){
      server.send_html(200, "target=1");
      return;
    }
  }
  server.send_html(200, "target=0");
  return;
}

void set_server(){
  server.add_handler("/", handle_root);
  server.begin();
}