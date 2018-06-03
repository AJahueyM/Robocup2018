#include "TOF.h"
const uint8_t TOF::validPin[2] = {42, 49};
bool TOF::vlxSetup = false;

TOF::TOF(const uint8_t pinShut, const uint8_t address){
    if(TOF::vlxSetup){
        for(int i = 0; i < 2;++i){
            uint8_t pin =  TOF::validPin[i];
            pinMode(pin, OUTPUT);
            digitalWrite(pin, LOW);        
        }
        delay(10);
        for(int i = 0; i < 2;++i){
            uint8_t pin =  TOF::validPin[i];           
            digitalWrite(pin, HIGH);        
        }   
        TOF::vlxSetup = true;
    }
    bool validPin  = false;

    for(int i = 0; i < 2;++i){
        uint8_t pin = TOF::validPin[i];
        if(pin != pinShut){
            digitalWrite(pin, HIGH);
        }
    }

    if(address != VL53L0X_I2C_ADDR)
        lox.begin(address);
    else
        lox.begin();
}

uint8_t TOF::getDistance(){
    if(millis() - lastReadTime > readRateMs){
        VL53L0X_RangingMeasurementData_t measure;
        lox.rangingTest(&measure, false); 
        distance = measure.RangeMilliMeter / 1000;
        lastReadTime = millis();
    }
    return distance;
}