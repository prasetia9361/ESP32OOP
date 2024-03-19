#include "eeprom.h"
#include "HTML.h"
#include <EEPROM.h>
#include <WebServer.h>
WebServer server(80);
// String ssidwifi;
// String passwifi;

eeprom::eeprom(){}

void eeprom::setup(int size){
    // this->size = size;
    EEPROM.begin(size);
    // eepromWifi();
}

// void eeprom::eepromWifi(int address, int max){
//     value = "";
//     for (int i = address; i < max; ++i) {
//         char data1 = EEPROM.read(i);
//         if (data1 != '\0') {
//             value += data1;
//         }
//     }
//     // for (int i = 32; i < 96; ++i) {
//     //     char data2 = EEPROM.read(i);
//     //     if (data2 != '\0') {
//     //          += data2;
//     //     }
//     // }

// }

String eeprom::eepromSsid(int addressSsid,int lengthSsid){
    valueSsid = "";
    for (int i = addressSsid; i < lengthSsid; ++i) {
        char data1 = EEPROM.read(i);
        if (data1 != '\0') {
            valueSsid += data1;
        }
    }
    return valueSsid;
}

String eeprom::eepromPass(int addressPass,int lengthPass){
    valuePass = "";
    for (int i = addressPass; i < lengthPass; ++i) {
        char data2 = EEPROM.read(i);
        if (data2 != '\0') {
            valuePass += data2;
        }
    }
    return valuePass;
}

// void eeprom::readEeprom(int addres,int length){
//     this->addres = addres;
//     this->length = length;
//     this->eepromWifi(addres,length);
// }

// void eeprom::writewifi(String xas, String yas){
//     // this->ssidNew = ssidNew;
//     // this->passNew = passNew;
//     if (xas.length() > 0 && yas.length() > 0) {
//         for (int i = 0; i < 124; ++i) {
//             EEPROM.write(i, 0);
//             EEPROM.commit();
//         }
//     }

//     for (int i = 0; i < xas.length(); ++i) {
//         EEPROM.write(i, xas[i]);
//     }
//     for (int i = 0; i < yas.length(); ++i) {
//         EEPROM.write(32 + i, yas[i]);
//     }
//     EEPROM.commit();
// }

// String eeprom::wifiValue() const{
//     return value;
// }

void eeprom::handleRoot() {
  server.send(200, "text/html", index_html);
}

void eeprom::handleFrom() {
  String ssidNew = server.arg("ssidNew");
  String passNew = server.arg("passNew");
  server.send(200, "text/html", sukses_html);
  delay(2000);
  // writewifi(ssidNew, passNew);
  // this->ssidNew = ssidNew;
  // this->passNew = passNew;
  if (ssidNew.length() > 0 && passNew.length() > 0) {
    for (int i = 0; i < 124; ++i) {
        EEPROM.write(i, 0);
        EEPROM.commit();
    }
  }

  for (int i = 0; i < ssidNew.length(); ++i) {
    EEPROM.write(i, ssidNew[i]);
  }
  for (int i = 0; i < passNew.length(); ++i) {
    EEPROM.write(32 + i, passNew[i]);
  }
  EEPROM.commit();
  Serial.println("setting wifi berhasil");
  ESP.restart();
}

void handleRootWrapper() {
    eeprom eepromInstance;  // Buat objek eeprom
    eepromInstance.handleRoot();  // Panggil metode handleRoot()
}

void handleFromWrapper() {
    eeprom eepromInstance;  // Buat objek eeprom
    eepromInstance.handleFrom();  // Panggil metode handleFrom()
}

void eeprom::settingWifi(){
    // eeprom eepromInstance;
    // server.on("/", std::bind(&eeprom::handleRoot, eepromInstance)); 
    // server.on("/action_page", std::bind(&eeprom::handleFrom, eepromInstance));
    server.on("/", handleRootWrapper);  // Panggil fungsi bebas yang memanggil handleRoot()
    server.on("/action_page", handleFromWrapper);  // Panggil fungsi bebas yang memanggil handleFrom()
    server.begin();
}

void eeprom::notConnect(){
    server.handleClient();
}

