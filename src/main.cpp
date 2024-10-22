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
#include <PubSubClient.h>

#define BUTTON_OFFSET 2 //BLUE
#define BUTTON_START 4  //RED
#define BUZZER_PIN 5 // Pin digital connected to buzzer


const char* ssid = WIFI_SSID;
const char* password = WIFI_PASSWORD;

WiFiClient wifiClient;
PubSubClient client(wifiClient);
const char* mqtt_server = "broker.emqx.io"; // Dirección del broker
const int mqtt_port = 1883; // Puerto del broker MQTT (TLS)
const char* mqtt_topic = "medition/Device"; // Tópico al cual se suscribe para recibir mensajes

DataManager dataManager;



// Variables para almacenar los datos del sensor
int16_t ax, ay, az;
int16_t gx, gy, gz;

void sendDataToServer(JsonDocument& doc) {
    if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;

        // Dirección IP del servidor y la ruta donde recibirás los datos
        http.begin("http://192.168.18.26:8080/evaluation/sensor");

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

void mqttCallback(char* topic, byte* payload, unsigned int length) {
    String message;
    for (int i = 0; i < length; i++) {
        message += (char)payload[i];
    }
    
    Serial.print("Mensaje recibido en el tópico: ");
    Serial.println(topic);
    Serial.print("Mensaje: ");
    Serial.println(message);
    
    // Si el mensaje es "start", iniciar la prueba
    if (message == "start") {
        Serial.println("Recibido comando 'start'. Iniciando prueba del sensor...");
        Buzzer::startSound();

        JsonDocument doc = sensorManager.getDataFetcher()->fetchAndconverDataToJSON(1);
        Serial.println("Feching data finished");

        // Escribir datos a SPIFFS
        dataManager.writeJsonToSPIFFS(doc);
        Serial.println("Data written to SPIFFS");

        // Leer los datos guardados
        JsonDocument docToSend = dataManager.readJsonFromSPIFFS();

        // Enviar datos al servidor si es necesario
        sendDataToServer(docToSend);

        Buzzer::finishedSound();
    }
}

void setupMQTT() {
    
    client.setServer(mqtt_server, mqtt_port);
    client.setCallback(mqttCallback);
}

void reconnectMQTT() {
    // Reintentar conexión en caso de desconexión
    while (!client.connected()) {
        Serial.print("Intentando conectar al broker MQTT...");
        if (client.connect("ESP32Client")) {
            Serial.println("Conectado al broker MQTT");
            client.subscribe(mqtt_topic); // Suscribirnos al tópico
        } else {
            Serial.print("Error de conexión MQTT, rc=");
            Serial.print(client.state());
            delay(5000);
        }
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

    // Configuración MQTT
    setupMQTT();    

}

void loop() {

    if (!client.connected()) {
        reconnectMQTT();
    }
    client.loop(); // Mantiene activa la conexión MQTT
    
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

    // if (digitalRead(BUTTON_START) == HIGH)
    // {

    //     Buzzer::startSound();

    //     Serial.println("Start feching data");
    //     JsonDocument doc = sensorManager.getDataFetcher()->fetchAndconverDataToJSON(1);
    //     Serial.println("Feching data finished");
    //     // dataManager.writeJsonToLittleFS(doc);
    //     Serial.println("Data writing to file: ");
    //     dataManager.writeJsonToSPIFFS(doc);
    //     Serial.println("Data sending to server: ");
    //     // sendDataToServer(doc);
    //     Serial.println("Data sent to server");
    //     JsonDocument docToSend = dataManager.readJsonFromSPIFFS();
    //     //sendDataToServer(docToSend);

    //     Buzzer::finishedSound();

    // }

}
