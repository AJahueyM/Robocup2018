#ifndef __TOF_H_
#define __TOF_H_
#include <Arduino.h>
#include <Adafruit_VL53L0X.h>

class TOF{
public:
    TOF(const uint8_t pinShut, const uint8_t address = VL53L0X_I2C_ADDR);
    const static uint8_t validPin[];
    const static uint8_t sensorNumber;
    static bool sensorSetUp[];
    static bool vlxSetup;
    int getDistance();
private:
    long lastReadTime = 0;
    const long readRateMs = 10;
    int distance = 0;
    Adafruit_VL53L0X lox;

};
#endif

