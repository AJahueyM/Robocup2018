#include "TOF.h"
const uint8_t TOF::validPin[] = {42, 49};
bool TOF::vlxSetup = false;
const uint8_t TOF::sensorNumber = 2;
TOF::TOF(const uint8_t pinShut, const uint8_t address){
    if(TOF::vlxSetup){
        for(int i = 0; i < TOF::sensorNumber;++i){
            uint8_t pin =  TOF::validPin[i];
            pinMode(pin, OUTPUT);
            digitalWrite(pin, LOW);        
        }
        delay(10);
        TOF::vlxSetup = true;
    }
    bool validPin  = false;

    for(int i = 0; i < TOF::sensorNumber;++i){
        uint8_t pin = TOF::validPin[i];
        if(pin != pinShut){
            digitalWrite(pin, HIGH);
        }
    }
    delay(100);
    if(address != VL53L0X_I2C_ADDR)
        lox.begin(address);
    else
        lox.begin();
}

int TOF::getDistance(){
    if(millis() - lastReadTime > readRateMs){
        VL53L0X_RangingMeasurementData_t measure;
        lox.rangingTest(&measure, false); 
        if(measure.RangeStatus != 4)
            distance = measure.RangeMilliMeter / 10;
        else
            distance = 0;
        lastReadTime = millis();
    }
    return distance;
}