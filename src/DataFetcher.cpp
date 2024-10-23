#include "DataFetcher.h"
#include "ConfigSensor.h"

// Constructor
DataFetcher::DataFetcher(MPU6050 &sensorInstance)
    : sensor(sensorInstance)
{
}

// Destructor
DataFetcher::~DataFetcher(){
    sensorReadings.clear();
    doc.clear();
}

// Fetch data methods


bool DataFetcher::fetchData(int8_t duration){
    // Fetch data from the sensor
    SensorReading reading;
    unsigned long startTime = millis();
    unsigned long currentTime = millis();
    unsigned long elapsedTime = 0;
    while (elapsedTime < duration * 1000) {
        sensor.getMotion6(&reading.accX, &reading.accY, &reading.accZ, &reading.gyroX, &reading.gyroY, &reading.gyroZ);
        reading.timestamp = millis();
        addReading(reading);
        currentTime = millis();
        elapsedTime = currentTime - startTime;
        delay(100);
    }
    return true;
}

JsonDocument DataFetcher::fetchAndconverDataToJSON(int8_t duration){
    
    
    if (isEmpty()) {
        Serial.println("LinkedList is empty. Fetching data from sensor.");
        fetchData(duration);
        Serial.println("Fetching data  status = done.");
    }

    
    JsonObject readings = doc["readings"].to<JsonObject>(); 
    JsonArray accelArray = readings["accelerometer"].to<JsonArray>();
    JsonArray gyroArray = readings["gyroscope"].to<JsonArray>();

    std::array<float,2> sensitivity = getLSBSensitivity();

    while (!isEmpty()) {

        SensorReading readingRaw = sensorReadings.pop();
        JsonObject accelJSON = accelArray.add<JsonObject>();
        accelJSON["x"] = readingRaw.accX * sensitivity[0];
        accelJSON["y"] = readingRaw.accY * sensitivity[0];
        accelJSON["z"] = readingRaw.accZ * sensitivity[0];
        accelJSON["timestamp"] = readingRaw.timestamp / 1000.0;
        
        // Agregar lectura al arreglo de giroscopio
        JsonObject gyroJSON = gyroArray.add<JsonObject>();
        gyroJSON["x"] = readingRaw.gyroX * sensitivity[1];
        gyroJSON["y"] = readingRaw.gyroY * sensitivity[1];
        gyroJSON["z"] = readingRaw.gyroZ * sensitivity[1];
        gyroJSON["timestamp"] = readingRaw.timestamp / 1000.0;
        }
    String stringJson ; 


  
    return doc;
}

std::array<float,2> DataFetcher::getLSBSensitivity(){
    
    
    //LSB parameters
    
    float const bitsRange = 32768; 
    float const gravityValue = 9.81;

    //ConfigSensor variables
    
    uint8_t accelRange = ConfigSensor::getAccelRange();
    uint8_t gyroRange = ConfigSensor::getGyroRange();

    //Storages sensitivity values
    
    std::array<float,2> sensitivity;

    //Define accel mapping LSB sensitivity

    if (MPU6050_ACCEL_FS_2 == accelRange) {
        sensitivity[0] = (2 * gravityValue) / bitsRange;
    } else if (MPU6050_ACCEL_FS_4 == accelRange) {
        sensitivity[0] = (4 * gravityValue) / bitsRange;
    } else if (MPU6050_ACCEL_FS_8 == accelRange) {
        sensitivity[0] = (8 * gravityValue) / bitsRange;
    } else if (MPU6050_ACCEL_FS_16 == accelRange) {
        sensitivity[0] = (16 * gravityValue) / bitsRange;
    } else { 
        Serial.println("Error while getting accelerometer sensitivity LSB.");
    }  
    
    //Define gyro mapping LSB sensitivity

    if (MPU6050_GYRO_FS_250 == gyroRange) {
        sensitivity[1] = (250 * M_PI / 180) / bitsRange;
    } else if (MPU6050_GYRO_FS_500 == gyroRange) {
        sensitivity[1] = (500 * M_PI / 180) / bitsRange;
    } else if (MPU6050_GYRO_FS_1000 == gyroRange) {
        sensitivity[1] = (1000 * M_PI / 180) / bitsRange;
    } else if (MPU6050_GYRO_FS_2000 == gyroRange) {
        sensitivity[1] = (2000 * M_PI / 180) / bitsRange;
    } else {
        Serial.println("Error while getting gyroscope sensitivity LSB.");
    }

    return sensitivity;
    
}



//Linkedlist methods

void DataFetcher::addReading(const SensorReading& reading){
    
    sensorReadings.add(reading);

}

bool DataFetcher::getReading(SensorReading& reading){
    if (!(sensorReadings.size() == 0)) {
        reading = sensorReadings.remove(0);
        return true;
    }
    return false;
}

bool DataFetcher::isEmpty() {

    return sensorReadings.size() == 0 ? true : false; 

}

int DataFetcher::size() {
    return sensorReadings.size();
}

void DataFetcher::clearQueue(){
    sensorReadings.clear();
}
