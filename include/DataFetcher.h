#ifndef DATAFETCHER_H
#include <Wire.h>
#include "I2Cdev.h"
#include "MPU6050.h"
#include <ArduinoJson.h>


class DataFetcher{


    public:        
    DataFetcher(MPU6050 &sensor);

    private:
    MPU6050 &sensor;
    const size_t JSON_BUFFER_SIZE = 512;
    DynamicJsonDocument doc; 
    
};





#endif