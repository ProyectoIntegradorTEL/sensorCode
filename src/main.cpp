// Incluye las bibliotecas necesarias
#include "I2Cdev.h"
#include "MPU6050.h"
#include <Wire.h>
#include "ConfigSensor.h"
// Crear una instancia del MPU6050
MPU6050 mpu;
ConfigSensor configSensor(mpu, MPU6050_ACCEL_FS_2, MPU6050_GYRO_FS_250);

// Variables para almacenar los datos del sensor
int16_t ax, ay, az;
int16_t gx, gy, gz;

void setup() {
    // Iniciar la comunicación serial a 115200 baudios
    Serial.begin(115200);
    while (!Serial) {
        ; // Espera a que el puerto serie esté listo
    }

    // Iniciar la comunicación I2C
    Wire.begin();


    // Inicializar el MPU6050
    Serial.println("Inicializando MPU6050...");
    mpu.initialize();

    // Test MPU6050 connection
    
    if (mpu.testConnection()) {
        Serial.println("Conexión exitosa con MPU6050");
    } else {
        Serial.println("Fallo en la conexión con MPU6050");
        while (1) {

        }
    }


    // MPU6050 configuration

     if(configSensor.setScaleRange()) {
        Serial.println("Configuración de rangos de escala completa exitosa.");
    } else {
        Serial.println("Error en la configuración de rangos de escala completa.");
    }

    if(configSensor.setSamplingFrequency(0, 0)) {
        Serial.println("Frecuencia de muestreo configurada correctamente.");
    } else {
        Serial.println("Error al configurar la frecuencia de muestreo.");
    }

    if(configSensor.setOffsets(5)) { 
        Serial.println("Offsets calibrados correctamente.");
    } else {
        Serial.println("Error durante la calibración de offsets.");
    }

}

void loop() {
    // Leer los datos de aceleración y giroscopio
    mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

    // Convertir los valores a unidades físicas
    float accX = ax / 16384.0;  // Conversión para ±2g
    float accY = ay / 16384.0;
    float accZ = az / 16384.0;

    float gyroX = gx / 131.0;    // Conversión para ±250°/s
    float gyroY = gy / 131.0;
    float gyroZ = gz / 131.0;

    // Mostrar los valores convertidos en el monitor serial
    Serial.print("Acelerómetro: ");
    Serial.print("X = "); Serial.print(accX, 2);
    Serial.print(" g | Y = "); Serial.print(accY, 2);
    Serial.print(" g | Z = "); Serial.print(accZ, 2); Serial.println(" g");

    Serial.print("Giroscopio: ");
    Serial.print("X = "); Serial.print(gyroX, 2);
    Serial.print(" °/s | Y = "); Serial.print(gyroY, 2);
    Serial.print(" °/s | Z = "); Serial.print(gyroZ, 2); Serial.println(" °/s");

    Serial.println("---------------------------");
    
    // Esperar 1 segundo antes de la próxima lectura
    delay(1000);
}
