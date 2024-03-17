#include <Arduino.h>

class comunication{
    private:
     String text;
     bool state = false;
     int message;
     byte localAddress = 0xBB;  // address of this device
     byte destination = 0xFF;
     int ss = 5;
     int rst = 14;
     int dio0 = 2;
     unsigned long sending;
     int packetSize;
     char cadena[40]; 
     byte posicion = 0;
     String temp;
     String longitude;
     String latitude;
     String speed;
     String voltage;
     String current;
    public:
     comunication();
     void wifiAP();
     void wifiConnect(String ssid, String pass);
     bool wifiStatus();
     void wifiAPconnect(String ssid, String pass);
     void mqttconnect(String text);
     void mqttloop();
     bool mqttlost();
     void loraconnect();
     String getValue(String data, char separator, int index);
     void sendtemp(uint64_t Id);
     void sendlong(uint64_t Id);
     void sendlat(uint64_t Id);
     void sendspeed(uint64_t Id);
     void sendVolatge(uint64_t Id);
     void sendCurrent(uint64_t Id);
     void onRecive(uint64_t numId);
};