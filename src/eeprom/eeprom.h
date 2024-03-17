#include <Arduino.h>

class eeprom {
    private:
    String value;
    int addres;
    int length;
    public:
     eeprom();
     void setup(int size);
     void eepromWifi(int address, int max);
     void readeeprom(int addres, int length);
     void writewifi(String xas, String yas);
     String wifiValue() const;
};