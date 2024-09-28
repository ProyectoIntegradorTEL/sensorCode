#ifndef SENSORREADING_H
#define SENSORREADING_H
#include <cstdint>

struct SensorReading {

    unsigned long timestamp;
    int16_t accX, accY, accZ, gyroX, gyroY, gyroZ;
};

#endif