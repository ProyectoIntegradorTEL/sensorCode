#include "ConfigSensor.h"

uint8_t ConfigSensor::accelRange;
uint8_t ConfigSensor::gyroRange;

ConfigSensor::ConfigSensor(MPU6050 &sensor, uint8_t accelRange, uint8_t gyroRange): 
sensor(sensor){

    ConfigSensor::accelRange = accelRange;
    ConfigSensor::gyroRange = gyroRange;

}

ConfigSensor::~ConfigSensor() {}


bool ConfigSensor::setSamplingFrequency(uint8_t DLPFMode, uint8_t rate){

    sensor.setDLPFMode(DLPFMode);
    sensor.setRate(rate);

    if (sensor.getDLPFMode() != DLPFMode || sensor.getRate() != rate) {
        Serial.print("Error while configuring sampling frequency.");
        return false;
    }

    Serial.print("Sampling frequency configured successfully.");
    return true;

}

bool ConfigSensor::setOffsets(int samples){


    try
    {
        sensor.CalibrateAccel(samples);
        sensor.CalibrateGyro(samples);

    }
    catch(const std::exception& e)
    {

        Serial.print("Error while configuring offsets.");
        return false;

    }
    
    Serial.print("Offsets configured successfully.");
    return true;

}


bool ConfigSensor::setScaleRange(){     

    //Define gyroscope and accelerometer ranges.    
    
    sensor.setFullScaleAccelRange(accelRange);    
    sensor.setFullScaleGyroRange(gyroRange);   

    if (sensor.getFullScaleAccelRange() != accelRange ||
        sensor.getFullScaleGyroRange() != gyroRange) {
        Serial.print("Error while configuring scale ranges.");
        return false;
    }

    Serial.print("Range scales configured successfully.");
    return true;

}


