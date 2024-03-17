#include "eeprom.h"
#include <EEPROM.h>
// String ssidwifi;
// String passwifi;

eeprom::eeprom(){}

void eeprom::setup(int size){
    // this->size = size;
    EEPROM.begin(size);
    // eepromWifi();
}

void eeprom::eepromWifi(int address, int max){
    value = "";
    for (int i = address; i < max; ++i) {
        char data1 = EEPROM.read(i);
        if (data1 != '\0') {
            value += data1;
        }
    }
    // for (int i = 32; i < 96; ++i) {
    //     char data2 = EEPROM.read(i);
    //     if (data2 != '\0') {
    //          += data2;
    //     }
    // }

}

void eeprom::readeeprom(int addres,int length){
    this->addres = addres;
    this->length = length;
    this->eepromWifi(addres,length);
}

void eeprom::writewifi(String xas, String yas){
    // this->ssidNew = ssidNew;
    // this->passNew = passNew;
    if (xas.length() > 0 && yas.length() > 0) {
        for (int i = 0; i < 124; ++i) {
            EEPROM.write(i, 0);
            EEPROM.commit();
        }
    }

    for (int i = 0; i < xas.length(); ++i) {
        EEPROM.write(i, xas[i]);
    }
    for (int i = 0; i < yas.length(); ++i) {
        EEPROM.write(32 + i, yas[i]);
    }
    EEPROM.commit();
}

String eeprom::wifiValue() const{
    return value;
}

