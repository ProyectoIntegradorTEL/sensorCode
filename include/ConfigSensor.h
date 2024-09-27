#ifndef CONFIGSENSOR_H
#define CONFIGSENSOR_H

#include <Wire.h>
#include "I2Cdev.h"
#include "MPU6050.h"

class ConfigSensor{

    public: 
        
        ConfigSensor(MPU6050 &sensor);

        bool setSamplingFrequency(uint8_t DLPFMode, uint8_t rate);
        bool setOffsets(int samples);
        bool setScaleRange(uint8_t accelRange, uint8_t gyroRange); 

    private: 
        MPU6050 &sensor;
   
};

#endif
