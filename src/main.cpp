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
long lastSendTime = 0; // last send time
int interval = 2000;   // interval between sends
int length = 512;

TaskHandle_t WifiTask;
TaskHandle_t LoraTask;
TaskHandle_t MqttTask;

void wifiTask(void *pvParameters);
void loraTask(void *pvParameters);
void mqttTask(void *pvParameters);
void sending(void *pvParameters);
void setup()
{
  Serial.begin(115200);
  Serial.println("LoRa Receiver");
  prom.setup(length);
  com.loraconnect();
  xTaskCreatePinnedToCore(
      wifiTask,
      "Task1",
      1000,
      NULL,
      10,
      &WifiTask,
      0);

  // prom.setup(length);
  // ssidWifi = prom.eepromSsid(0, 20);
  // passWifi = prom.eepromPass(20, 50);
  // // client.begin("broker.emqx.io", net);
  // chipId = ESP.getEfuseMac();
  // Serial.println(ssidWifi);
  // Serial.println(passWifi);
  // com.wifiConnect(ssidWifi, passWifi);
  // com.loraconnect();
  // Serial.println("LoRa Active");
  xTaskCreatePinnedToCore(
      mqttTask,
      "Task3",
      1000,
      NULL,
      8,
      &MqttTask,
      0);
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

void loop()
{

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

void wifiTask(void *pvParameters)
{
  ssidWifi = prom.eepromSsid(0, 20);
  passWifi = prom.eepromPass(20, 50);
  // client.begin("broker.emqx.io", net);
  chipId = ESP.getEfuseMac();
  Serial.println(ssidWifi);
  Serial.println(passWifi);

  while (true)
  {
    com.wifiConnect(ssidWifi, passWifi);
    delay(20);
  }
}

void mqttTask(void *pvParameters)
{
  if (!com.wifiStatus())
  {
    com.wifiAPconnect(ssid, pass);
    prom.settingWifi();
    // server.on("/", handleRoot);  // Routine untuk menghandle homepage
    // server.on("/action_page", handleForm);
    // server.begin();
  }
  else if (com.wifiStatus())
  {
    com.wifiAP();
    com.mqttconnect(String(chipId) + "/#");
  }
  while (true)
  {
    if (!com.wifiStatus())
    {
      prom.notConnect();
      // server.handleClient();
    }
    else
    {
      // timer.run();
      com.mqttloop();
      if (com.mqttlost())
      {
        com.wifiAP();
        com.mqttconnect(String(chipId) + "/#");
      }
      com.onRecive(chipId);
      // onRecive(LoRa.parsePacket());
    }
  }
}