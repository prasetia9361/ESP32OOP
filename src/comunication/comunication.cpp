#include "comunication.h"
#include <WiFi.h>
#include <WiFiClient.h>
#include <SPI.h>
#include <LoRa.h>
#include <MQTT.h>
WiFiClient net;
MQTTClient client;

comunication::comunication(){}

void comunication::wifiAP(){
    state = true;
    WiFi.softAPdisconnect(state);
    WiFi.mode(WIFI_STA);
    Serial.println("menghubungkan ke mqtt");
    Serial.print("connecting");
}

void comunication::wifiConnect(String ssid, String pass){
    WiFi.begin(ssid, pass);
}

bool comunication::wifiStatus(){
    return WiFi.status() == WL_CONNECTED;
}

void comunication::wifiAPconnect(String ssid, String pass){
    WiFi.softAP(ssid, pass);
    IPAddress IP = WiFi.softAPIP();
    Serial.println("");
    Serial.print("IP Address: ");
    Serial.println(IP);
}

void comunication::mqttloop(){
    client.loop();
}

bool comunication::mqttlost(){
    return !client.connected();
}

void comunication::mqttconnect(String text){
    client.begin("broker.emqx.io", net);
    while (!client.connect("ESP32")) {
        Serial.print(".");
        delay(1000);
    }
    Serial.print("mqtt connected");
    client.subscribe(text);//String(chipId) + "/#"
}

void comunication::loraconnect(){
    LoRa.setPins(ss, rst, dio0);
    if (!LoRa.begin(433E6)) {
        Serial.println("Starting LoRa failed!");
        while (1)
        ;
    }
}

String comunication::getValue(String data, char separator, int index) {
  int found = 0;
  int strIndex[] = { 0, -1 };
  int maxIndex = data.length() - 1;
  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

void comunication::sendtemp(uint64_t Id) {
  client.publish(String(Id) + "/suhu", temp);//
  Serial.println("data suhu terkirim lewat" + String(Id) + "/suhu");
  Serial.println(temp);
  // 106151743883988/suhu
  // delay(100);
}

void comunication::sendlong(uint64_t Id) {
  client.publish(String(Id) + "/long", longitude);
  Serial.println("data longitude terkirim lewat" + String(Id) + "/long");
  Serial.println(longitude);
  // 106151743883988/long
  // delay(100);
}
void comunication::sendlat(uint64_t Id) {
  client.publish(String(Id) + "/lat", latitude);
  Serial.println("data latitude terkirim lewat" + String(Id) + "/lat");
  Serial.println(latitude);
  // 106151743883988/lat
  // delay(100);
}
void comunication::sendspeed(uint64_t Id) {
  client.publish(String(Id) + "/speed", speed);
  Serial.println("data speed terkirim lewat" + String(Id) + "/speed");
  Serial.println(speed);
  // 106151743883988/speed
  // delay(100);
}

void comunication::sendVolatge(uint64_t Id) {
  client.publish(String(Id) + "/vol", voltage);
  Serial.println("data speed terkirim lewat" + String(Id) + "/vol");
  Serial.println(voltage);
  // 106151743883988/vol
  // delay(100);
}

void comunication::sendCurrent(uint64_t Id) {
  client.publish(String(Id) + "/current", current);
  Serial.println("data speed terkirim lewat" + String(Id) + "/current");
  Serial.println(current);
  // 106151743883988/current
  // delay(100);
}

void comunication::onRecive(uint64_t numId){
    packetSize =  LoRa.parsePacket();
    if (packetSize == 0) return;
    memset(cadena, 0, sizeof(cadena));  //memset deletes the contents of the array "cadena" from position 0 to the end sizeof
    // read packet header bytes:
    int recipient = LoRa.read();       // recipient address
    byte sender = LoRa.read();         // sender address
    byte incomingMsgId = LoRa.read();  // incoming msg ID
    // byte incomingLength = LoRa.read();  // incoming msg length
    // read packet
    // received a packet
    // Serial.print("Received packet... ");
    while (LoRa.available()) {
        char dedos = (char)LoRa.read();
        Serial.print(dedos);
        cadena[posicion] = dedos;  //Read a character from the string "cadena" from "posicion", then read the next character with "posicion++"
        posicion++;
    }
    posicion = 0;
    String mkrwan = String(sender, HEX);
    String esp32 = String(recipient, HEX);
    Serial.println("Received from: 0x" + mkrwan);
    Serial.println("Sent to: 0x" + esp32);
    Serial.println("Message ID: " + String(incomingMsgId));
    Serial.println("RSSI: " + String(LoRa.packetRssi()));
    Serial.println("Snr: " + String(LoRa.packetSnr()));

    temp = getValue(cadena, ',', 1);
    latitude = getValue(cadena, ',', 2);
    longitude = getValue(cadena, ',', 3);
    speed = getValue(cadena, ',', 4);
    voltage = getValue(cadena,',',5);
    current = getValue(cadena,',',6);

    if (recipient == localAddress && sender == destination) {
        if (millis() - sending >= 1000) {
        sending = millis();
        sendtemp(numId);
        sendlong(numId);
        sendlat(numId);
        sendspeed(numId);
        sendVolatge(numId);
        sendCurrent(numId);
        }
    } else {
        Serial.println("tidak ada data");
        return;
    }
}