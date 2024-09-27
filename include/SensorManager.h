#ifndef SENSORMANAGER_H
#define SENSORMANAGER_H

#include "MPU6050.h"
#include <Wire.h>
#include "I2Cdev.h"
#include "ConfigSensor.h"

class SensorManager {
private:

    MPU6050 mpu;
    ConfigSensor* configSensor;
    
    SensorManager() {}

public:
    
    static SensorManager& getInstance() {
        static SensorManager instance; 
        return instance;
    }

    // Prohibimos la copia y la asignación de la clase
    SensorManager(SensorManager const&) = delete;
    void operator=(SensorManager const&) = delete;


    // Función para inicializar el sensor
    void initialize() {
        Wire.begin();
        delay(500);
        mpu.initialize();
        delay(500);
        if (configSensor == nullptr) {
        configSensor = new ConfigSensor(mpu);
        }
    }

    ~SensorManager() {
    delete configSensor; // Libera la memoria al destruir el objeto
    }

    // Método para acceder directamente al objeto MPU6050
    MPU6050& getMPU6050() {
        return mpu;
    }
};

#endif
