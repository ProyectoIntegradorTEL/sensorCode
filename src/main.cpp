#include "globals.h"

int16_t ax, ay, az;
int16_t gx, gy, gz;

void setup() {
    
    Serial.begin(115200);
    while (!Serial) {
        ; // Espera a que el puerto serie esté listo
    }
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

    // Calibrar el acelerómetro y el giroscopio si es necesario
    // sensorManager.calibrateAccelerometer(6);
    // sensorManager.calibrateGyro(6);
    // sensorManager.printOffsets();
    
    delay(2000); // Espera para estabilizar el sensor
}

void loop() {
    // Leer los datos del sensor MPU6050
    sensorManager.getMPU6050().getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

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
