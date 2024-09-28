// Incluye las bibliotecas necesarias
#include "I2Cdev.h"
#include "MPU6050.h"
#include <Wire.h>
// #include "ConfigSensor.h"
// #include "DataFetcher.h"
#include "globals.h"

#define BUTTON_OFFSET 2 //BLUE
#define BUTTON_START 4  //RED

// Crear una instancia del MPU6050
// MPU6050 mpu;
// ConfigSensor configSensor(mpu, MPU6050_ACCEL_FS_2, MPU6050_GYRO_FS_250);
// DataFetcher dataFetcher(mpu);

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

    // Inicializar el sensor MPU6050
    Serial.println("Configurando MPU6050...");
    sensorManager.initialize();
    delay(500);
    // Comprobar la conexión con el sensor
    if (!sensorManager.getMPU6050().testConnection()) {
        Serial.println("Error: No se pudo conectar con el MPU6050.");
        while (1); // Detener la ejecución
    } else {
        Serial.println("Conexión con el MPU6050 establecida.");
    }
    
    delay(2000); // Espera para estabilizar el sensor
}

void loop() {


    if (digitalRead(BUTTON_OFFSET) == HIGH)
    {

        Serial.print("Boton presionado");
        
        if(sensorManager.getConfigSensor()->setOffsets(5)) { 
            Serial.print("Offsets calibrados correctamente.");
        } else {
            Serial.print("Error durante la calibración de offsets.");
        }
        
    }

    if (digitalRead(BUTTON_START) == HIGH)
    {

        Serial.print("Start feching data");
        sensorManager.getDataFetcher()->fetchAndconverDataToJSON(10);
        Serial.print("Feching data finished");
        
    }

    // // Leer los datos del sensor MPU6050
    // sensorManager.getMPU6050().getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

    // // Convertir los valores a unidades físicas
    // float accX = ax / 16384.0;  // Conversión para ±2g
    // float accY = ay / 16384.0;
    // float accZ = az / 16384.0;

    // float gyroX = gx / 131.0;    // Conversión para ±250°/s
    // float gyroY = gy / 131.0;
    // float gyroZ = gz / 131.0;

    // // Mostrar los valores convertidos en el monitor serial
    // Serial.print("Acelerómetro: ");
    // Serial.print("X = "); Serial.print(accX, 2);
    // Serial.print(" g | Y = "); Serial.print(accY, 2);
    // Serial.print(" g | Z = "); Serial.print(accZ, 2); Serial.println(" g");

    // Serial.print("Giroscopio: ");
    // Serial.print("X = "); Serial.print(gyroX, 2);
    // Serial.print(" °/s | Y = "); Serial.print(gyroY, 2);
    // Serial.print(" °/s | Z = "); Serial.print(gyroZ, 2); Serial.println(" °/s");

    // Serial.println("---------------------------");
    
    // // Esperar 1 segundo antes de la próxima lectura
    // delay(1000);
}
