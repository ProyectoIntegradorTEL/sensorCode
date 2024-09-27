#include "globals.h"
#include <Arduino.h>
#include "MPU6050.h"
#include <Wire.h>
#include "I2Cdev.h"
// Inicialización de la instancia única
SensorManager& sensorManager = SensorManager::getInstance();

