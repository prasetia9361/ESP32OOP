#include <Arduino.h>
// #include <WebServer.h>
#include "./eeprom/eeprom.h"
#include "./comunication/comunication.h"
// #include "HTML.h"
eeprom prom;
comunication com;
int valor;
// WebServer server(80);

#define ssid "Setting_WiFi"
#define pass "1qazxsw2"
// String ssidNew = "";
// String passNew;
String ssidWifi;
String passWifi;

uint64_t chipId;

byte msgCount = 0;
long lastSendTime = 0;     // last send time
int interval = 2000;       // interval between sends
int length = 512;

TaskHandle_t Task1;
TaskHandle_t Task2;
TaskHandle_t Task3;
TaskHandle_t Task4;

void wifiBegin(void* pvParameters);
void LoraBegin(void* pvParameters);
void mqttBegin(void* pvParameters);
void sending(void* pvParameters);
void setup() {
  Serial.begin(115200);
  Serial.println("LoRa Receiver");
  prom.setup(length);
  xTaskCreatePinnedToCore(
    wifiBegin,
    "Task1",
    1000,
    NULL,
    10,
    &Task1,
    0
  );

  // prom.setup(length);
  // ssidWifi = prom.eepromSsid(0, 20);
  // passWifi = prom.eepromPass(20, 50);
  // // client.begin("broker.emqx.io", net);
  // chipId = ESP.getEfuseMac();
  // Serial.println(ssidWifi);
  // Serial.println(passWifi);
  // com.wifiConnect(ssidWifi, passWifi);
  xTaskCreatePinnedToCore(
    LoraBegin,
    "Task2",
    1000,
    NULL,
    10,
    &Task2,
    1
  );
  // com.loraconnect();
  // Serial.println("LoRa Active");
  xTaskCreatePinnedToCore(
    wifiBegin,
    "Task3",
    1000,
    NULL,
    8,
    &Task3,
    0
  );
  // if (!com.wifiStatus()) {
  //   com.wifiAPconnect(ssid,pass);
  //   prom.settingWifi();
  //   // server.on("/", handleRoot);  // Routine untuk menghandle homepage
  //   // server.on("/action_page", handleForm);
  //   // server.begin();
  // } else if (com.wifiStatus()) {
  //   com.wifiAP();
  //   com.mqttconnect(String(chipId) + "/#");
  // }
}

void loop() {
  xTaskCreatePinnedToCore(
    sending,
    "Task4",
    1000,
    NULL,
    8,
    &Task4,
    1
  );
  // if (!com.wifiStatus()) {
  //   prom.notConnect();
  //   // server.handleClient();
  // } else {
  //   // timer.run();
  //   com.mqttloop();
  //   if (com.mqttlost()) {
  //       com.wifiAP();
  //       com.mqttconnect(String(chipId) + "/#");
  //   }
  //   com.onRecive(chipId);
  //   // onRecive(LoRa.parsePacket());
  // }
}

void wifiBegin(void* pvParameters){
  ssidWifi = prom.eepromSsid(0, 20);
  passWifi = prom.eepromPass(20, 50);
  // client.begin("broker.emqx.io", net);
  chipId = ESP.getEfuseMac();
  Serial.println(ssidWifi);
  Serial.println(passWifi);
  com.wifiConnect(ssidWifi, passWifi);
}

void LoraBegin(void* pvParameters){
  com.loraconnect();
  Serial.println("LoRa Active");
}

void mqttBegin(void* pvParameters){
   if (!com.wifiStatus()) {
    com.wifiAPconnect(ssid,pass);
    prom.settingWifi();
    // server.on("/", handleRoot);  // Routine untuk menghandle homepage
    // server.on("/action_page", handleForm);
    // server.begin();
  } else if (com.wifiStatus()) {
    com.wifiAP();
    com.mqttconnect(String(chipId) + "/#");
  }
}

void sending(void* pvParameters){
  if (!com.wifiStatus()) {
    prom.notConnect();
    // server.handleClient();
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