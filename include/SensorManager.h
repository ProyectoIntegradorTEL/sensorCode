#ifndef SENSORMANAGER_H
#define SENSORMANAGER_H

#include "MPU6050.h"
#include "I2Cdev.h"
#include "ConfigSensor.h"
#include "DataFetcher.h"

class SensorManager {
private:

    MPU6050 mpu;
    ConfigSensor* configSensor;
    DataFetcher* dataFetcher;
    // Constructor privado para que no se pueda instanciar desde fuera
    SensorManager() {}

public:
    // Función para obtener la única instancia de SensorManager
    static SensorManager& getInstance() {
        static SensorManager instance; // Variable estática que contiene la instancia única
        return instance;
    }

    // Prohibimos la copia y la asignación de la clase
    SensorManager(SensorManager const&) = delete;
    void operator=(SensorManager const&) = delete;


    // Función para inicializar el sensor
    void initialize() {
        delay(500);
        mpu.initialize();
        delay(500);
        if (configSensor == nullptr) {
            configSensor = new ConfigSensor(mpu, MPU6050_ACCEL_FS_2, MPU6050_GYRO_FS_250);
        }
        if (dataFetcher == nullptr) {
            dataFetcher = new DataFetcher(mpu);
        }
    }

    ~SensorManager() {
    delete configSensor; // Libera la memoria al destruir el objeto
    }

    // Método para acceder directamente al objeto MPU6050
    MPU6050& getMPU6050() {
        return mpu;
    }

    ConfigSensor* getConfigSensor(){
        return configSensor;
    }

    DataFetcher* getDataFetcher(){
        return dataFetcher; 
    }
};

#endif
