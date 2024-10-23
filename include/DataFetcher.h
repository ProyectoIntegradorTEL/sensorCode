#ifndef DATAFETCHER_H
#include <Wire.h>
#include "I2Cdev.h"
#include "MPU6050.h"
#include <ArduinoJson.h>
#include "SensorReading.h"
#include <LinkedList.h>
#include <array>



class DataFetcher{


    public:        
        
    DataFetcher(MPU6050 &sensor);
    ~DataFetcher(); 

    //Queue methods

    void addReading(const SensorReading& reading);
    bool getReading(SensorReading& reading);
    bool isEmpty() ;
    int size() ;
    void clearQueue();
    JsonDocument fetchAndconverDataToJSON(int8_t duration);
    bool fetchData(int8_t duration); 
    std::array<float, 2> getLSBSensitivity();

    private:
    MPU6050 &sensor;
    JsonDocument doc; 
    LinkedList<SensorReading> sensorReadings;

     
};

#endif