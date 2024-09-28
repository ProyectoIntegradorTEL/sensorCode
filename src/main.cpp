// Incluye las bibliotecas necesarias
#include "I2Cdev.h"
#include "MPU6050.h"
#include <Wire.h>
#include "globals.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include "DataManager.h"
#include "Secrets.h"
#include "Buzzer.h"

#define BUTTON_OFFSET 2 //BLUE
#define BUTTON_START 4  //RED
#define BUZZER_PIN 5 // Pin digital connected to buzzer


const char* ssid = WIFI_SSID;
const char* password = WIFI_PASSWORD;
DataManager dataManager;



// Variables para almacenar los datos del sensor
int16_t ax, ay, az;
int16_t gx, gy, gz;

void sendDataToServer(JsonDocument& doc) {
    if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;

        // Dirección IP del servidor y la ruta donde recibirás los datos
        http.begin("http://192.168.1.3:8080/sensor-data");

        // Establecer el tipo de contenido como JSON
        http.addHeader("Content-Type", "application/json");

        // Convertir el documento JSON a una cadena
        String jsonData;
        serializeJson(doc, jsonData);

        // Enviar la solicitud POST con los datos
        int httpResponseCode = http.POST(jsonData);

        // Verificar la respuesta del servidor
        if (httpResponseCode > 0) {
            String response = http.getString();
            Serial.println("Respuesta del servidor: " + response);
        } else {
            Serial.println("Error en la solicitud: " + String(httpResponseCode));
        }

        // Finalizar la conexión HTTP
        http.end();
    } else {
        Serial.println("Error de conexión WiFi");
    }
}


void setup() {
    
    // Initialize communication for 115200 baudios

    pinMode(BUTTON_OFFSET, INPUT);
    pinMode(BUTTON_START, INPUT);
    pinMode(BUZZER_PIN, OUTPUT); 
    Serial.begin(115200);
    while (!Serial) {
        ; 
    }
    //Is important to wait for the serial monitor to open.
    delay(3000);

    
    // Inicializar el sistema de archivos SPIFFS
    if (!dataManager.begin()) {
        Serial.println("Error al inicializar el sistema de archivos SPIFFS");
        return;
    }

    // WiFi connection setting
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Conectando a WiFi...");
    }

    Serial.println("Conectado a la red WiFi");

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

        // MPU6050 configuration

     if(sensorManager.getConfigSensor()->setScaleRange()) {
        Serial.print("Configuración de rangos de escala completa exitosa.");
    } else {
        Serial.print("Error en la configuración de rangos de escala completa.");
    }

    if(sensorManager.getConfigSensor()->setSamplingFrequency(0, 0)) {
        Serial.print("Frecuencia de muestreo configurada correctamente.");
    } else {
        Serial.print("Error al configurar la frecuencia de muestreo.");
    }

}

void loop() {

    
    if (digitalRead(BUTTON_OFFSET) == HIGH)
    {

        Serial.print("Start offset configuration");
        Buzzer::startSound();

        
        if(sensorManager.getConfigSensor()->setOffsets(5)) { 
            Buzzer::configurationDoneSound();
            Serial.print("Offsets calibrados correctamente.");
            Buzzer::finishedSound();
        } else {
            Serial.print("Error durante la calibración de offsets.");
        }
        
    }

    if (digitalRead(BUTTON_START) == HIGH)
    {

        Buzzer::startSound();

        Serial.println("Start feching data");
        JsonDocument doc = sensorManager.getDataFetcher()->fetchAndconverDataToJSON(1);
        Serial.println("Feching data finished");
        // dataManager.writeJsonToLittleFS(doc);
        Serial.println("Data writing to file: ");
        dataManager.writeJsonToSPIFFS(doc);
        Serial.println("Data sending to server: ");
        // sendDataToServer(doc);
        Serial.println("Data sent to server");
        JsonDocument docToSend = dataManager.readJsonFromSPIFFS();
        //sendDataToServer(docToSend);

        Buzzer::finishedSound();

    }

}



