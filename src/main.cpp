#include <Arduino.h>
#include <WebServer.h>
#include "./eeprom/eeprom.h"
#include "./comunication/comunication.h"
#include "HTML.h"
eeprom prom;
comunication com;
int valor;
WebServer server(80);

String ssid = "Setting_WiFi";
String pass = "1qazxsw2";
String ssidNew = "";
String passNew;
String ssidwifi;
String passwifi;

uint64_t chipId;

byte msgCount = 0;
long lastSendTime = 0;     // last send time
int interval = 2000;       // interval between sends
int length = 512;

void handleRoot() {
  server.send(200, "text/html", index_html);
}
void handleForm() {
  ssidNew = server.arg("ssidNew");
  passNew = server.arg("passNew");
  server.send(200, "text/html", sukses_html);
  delay(2000);
  prom.writewifi(ssidNew, passNew);
  Serial.println("setting wifi berhasil");
  ESP.restart();
}

void setup() {
  Serial.begin(115200);
  Serial.println("LoRa Receiver");
  prom.setup(length);
  prom.readeeprom(0,32);
  ssidwifi = prom.wifiValue();
  prom.readeeprom(32,96);
  passwifi = prom.wifiValue();
  // client.begin("broker.emqx.io", net);
  chipId = ESP.getEfuseMac();
  com.loraconnect();
  Serial.println(ssidwifi);
  Serial.println(passwifi);
  com.wifiConnect(ssidwifi, passwifi);
  Serial.println("LoRa Active");
  if (!com.wifiStatus()) {
    com.wifiAPconnect(ssid,pass);
    server.on("/", handleRoot);  // Routine untuk menghandle homepage
    server.on("/action_page", handleForm);
    server.begin();
  } else if (com.wifiStatus()) {
    com.wifiAP();
    com.mqttconnect(String(chipId) + "/#");
  }
}

void loop() {

  if (!com.wifiStatus()) {
    server.handleClient();
  } else {
    // timer.run();
    com.mqttloop();
    if (com.mqttlost()) {
        com.wifiAP();
        com.mqttconnect(String(chipId) + "/#");
    }
    com.onRecive(chipId);
    // onRecive(LoRa.parsePacket());
  }
}