#include "TOF.h"
const uint8_t TOF::validPin[] = {42, 49};
bool TOF::sensorSetUp[] = {false, false};
bool TOF::vlxSetup = false;
const uint8_t TOF::sensorNumber = 2;
TOF::TOF(const uint8_t pinShut, const uint8_t address){
    if(!TOF::vlxSetup){
        Serial.println("ENTERED");
        for(int i = 0; i < TOF::sensorNumber;++i){
            uint8_t pin =  TOF::validPin[i];
            pinMode(pin, OUTPUT);
            digitalWrite(pin, LOW);        
        }
        delay(10);
    for(int i = 0; i < TOF::sensorNumber;++i){
        uint8_t pin = TOF::validPin[i];
        if(pin != pinShut){
            digitalWrite(pin, HIGH);
        }
    }
        TOF::vlxSetup = true;
    }

    if(pinShut == TOF::validPin[0]){
        TOF::sensorSetUp[0] = true;
    }else{
        TOF::sensorSetUp[1] = true;
    }
    
    for(int i = 0; i < TOF::sensorNumber; ++i){
        if(TOF::validPin[i] != pinShut && !TOF::sensorSetUp[i]){
            digitalWrite(TOF::validPin[i], LOW);
        }else{
            digitalWrite(TOF::validPin[i], HIGH);
            TOF::sensorSetUp[i] = true;
        }
    }
    
    if(address != VL53L0X_I2C_ADDR)
        lox.begin(address);
    else
        lox.begin();
    delay(100);

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