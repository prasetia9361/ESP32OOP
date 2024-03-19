#include <Arduino.h>

class eeprom {
    private:
     String valueSsid;
     String valuePass;
    public:
     eeprom();
     void setup(int size);
    //  void eepromWifi(int address, int max);
     String eepromSsid(int addressSsid,int lengthSsid);
     String eepromPass(int addressPass,int lengthPass);
    //  void readSSid(int addressSsid, int lengthSsid);
    //  void readPass(int addressPass, int lengthPass);
    //  void writewifi(String xas, String yas);
    //  String wifiValue() const;
     void handleRoot();
     void handleFrom();
     void settingWifi();
     void notConnect();
};