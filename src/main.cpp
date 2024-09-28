// Incluye las bibliotecas necesarias
#include "I2Cdev.h"
#include "MPU6050.h"
#include <Wire.h>
#include "ConfigSensor.h"
#include "DataFetcher.h"
#define BUTTON_OFFSET 2 //BLUE
#define BUTTON_START 4  //RED

// Crear una instancia del MPU6050
MPU6050 mpu;
ConfigSensor configSensor(mpu, MPU6050_ACCEL_FS_2, MPU6050_GYRO_FS_250);
DataFetcher dataFetcher(mpu);

// Variables para almacenar los datos del sensor
int16_t ax, ay, az;
int16_t gx, gy, gz;

void setup() {
    
    // Initialize communication for 115200 baudios

    pinMode(BUTTON_OFFSET, INPUT);
    pinMode(BUTTON_START, INPUT);
    
    Serial.begin(115200);
    while (!Serial) {
        ; 
    }
    //Is important to wait for the serial monitor to open.
    delay(3000);

    // I2C connection
    Wire.begin();


    // Initiliaze MPU6050
    Serial.println("Inicializando MPU6050...");
    mpu.initialize();

    // Test MPU6050 connection
    
    if (mpu.testConnection()) {
        Serial.print("Conexión exitosa con MPU6050");
    } else {
        Serial.print("Fallo en la conexión con MPU6050");
        while (1) {

        }
    }

    // MPU6050 configuration

     if(configSensor.setScaleRange()) {
        Serial.print("Configuración de rangos de escala completa exitosa.");
    } else {
        Serial.print("Error en la configuración de rangos de escala completa.");
    }

    if(configSensor.setSamplingFrequency(0, 0)) {
        Serial.print("Frecuencia de muestreo configurada correctamente.");
    } else {
        Serial.print("Error al configurar la frecuencia de muestreo.");
    }

   

}

void loop() {


    if (digitalRead(BUTTON_OFFSET) == HIGH)
    {

        Serial.print("Boton presionado");
        
        if(configSensor.setOffsets(5)) { 
            Serial.print("Offsets calibrados correctamente.");
        } else {
            Serial.print("Error durante la calibración de offsets.");
        }
        
    }

    if (digitalRead(BUTTON_START) == HIGH)
    {

        Serial.print("Start feching data");
        dataFetcher.fetchAndconverDataToJSON(10);
        Serial.print("Feching data finished");
        
    }

}
