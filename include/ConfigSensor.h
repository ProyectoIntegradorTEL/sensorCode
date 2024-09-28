#ifndef CONFIGSENSOR_H
#define CONFIGSENSOR_H

#include <Wire.h>
#include "I2Cdev.h"
#include "MPU6050.h"

class ConfigSensor{

    public: 
        
        ConfigSensor(MPU6050 &sensor, uint8_t accelRange, uint8_t gyroRange );
        ~ConfigSensor();

        bool setSamplingFrequency(uint8_t DLPFMode, uint8_t rate);
        bool setOffsets(int samples);
        bool setScaleRange(); 

        static uint8_t getAccelRange() { return accelRange; }
        static uint8_t getGyroRange() { return gyroRange; }

        static uint8_t accelRange; 
        static uint8_t gyroRange;
      

    private: 
        MPU6050 &sensor ;
   
};

#endif
