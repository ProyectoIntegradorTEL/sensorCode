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
#define MQTT_MAX_PACKET_SIZE 4096
#include <MQTT.h>

#define BUTTON_OFFSET 2 // BLUE
#define BUTTON_START 4  // RED
#define BUZZER_PIN 5    // Pin digital conectado al buzzer

const char* ssid = WIFI_SSID;
const char* password = WIFI_PASSWORD;
bool shouldPublishTestMessage = false;
String messageTosend = "";

WiFiClient net;
MQTTClient client(4096); // Tamaño del buffer

const char* mqtt_server = "broker.emqx.io"; // Dirección del broker
const int mqtt_port = 1883;                 // Puerto del broker MQTT

const char* mqtt_topicSendResult = "sender/Device082621";   // Tópico al cual se envían los mensajes
const char* mqtt_topicTriggerESP = "trigger/Device082621";  // Tópico al cual se suscribe para recibir mensajes

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

void printFreeMemory() {
    Serial.print("Memoria libre: ");
    Serial.print(ESP.getFreeHeap());
    Serial.println(" bytes");
}

void connectToMqtt() {
    // Reintentar conexión en caso de desconexión
    while (!client.connected()) {
        Serial.print("Intentando conectar al broker MQTT...");
        if (client.connect("ZfKevinZf12IcesiMQTT")) {
            Serial.println("Conectado al broker MQTT");
            // Suscribirse al tópico con QoS 1
            client.subscribe(mqtt_topicTriggerESP, 1);
        } else {
            Serial.print("Error de conexión MQTT, rc=");
            Serial.println(client.lastError());
            delay(5000);
        }
    }
}

void sendLargeMessage(const String& largeMessage) {
    const size_t fragmentSize = 1000;
    size_t totalLength = largeMessage.length();
    size_t fragmentCount = (totalLength + fragmentSize - 1) / fragmentSize;

    for (size_t i = 0; i < fragmentCount; i++) {
        size_t start = i * fragmentSize;
        size_t len = fragmentSize;

        if (start + len > totalLength) {
            len = totalLength - start;
        }

        String fragment = largeMessage.substring(start, start + len);

        int retryCount = 0;
        const int maxRetries = 3;
        bool success = false;

        while (retryCount < maxRetries && !success) {
            if (!client.connected()) {
                connectToMqtt();
            }

            // Publicar el fragmento con QoS 1
            if (client.publish(mqtt_topicSendResult, fragment, false, 1)) {
                Serial.print("Fragmento ");
                Serial.print(i);
                Serial.println(" enviado correctamente.");
                // Serial.println(fragment); // Puedes comentar esta línea si el fragmento es muy grande
                success = true;
            } else {
                Serial.print("Error al enviar el fragmento ");
                Serial.print(i);
                Serial.print(" Error: ");
                Serial.println(client.lastError());
                retryCount++;
            }
        }

        if (!success) {
            Serial.print("Fallo al enviar el fragmento ");
            Serial.println(i);
            break;
        }

        delay(200);
    }
}

void messageReceived(String &topic, String &payload) {
    Serial.print("Mensaje recibido en el tópico: ");
    Serial.println(topic);
    Serial.print("Mensaje: ");
    Serial.println(payload);

    // Si el mensaje es "start", iniciar la prueba
    if (payload == "start") {
        // Imprimir condiciones iniciales
        Serial.println("Recibido comando 'start'. Iniciando prueba del sensor...");

        Serial.print("Memoria libre 1: ");
        Serial.print(ESP.getFreeHeap());
        Serial.println(" bytes");

        Buzzer::startSound();

        // Obtener datos y guardar en documento JSON
        JsonDocument doc = sensorManager.getDataFetcher()->fetchAndconverDataToJSON(5);

        Serial.println("Feching data finished");

        // Almacenar información del documento JSON en messageTosend
        serializeJson(doc, messageTosend);

        Serial.println("Mensaje desde messageReceived");
        // Serial.println(messageTosend); // Puedes comentar esta línea si el mensaje es muy grande

        size_t messageSize = messageTosend.length();
        Serial.print("Tamaño del mensaje JSON: ");
        Serial.println(messageSize);

        Serial.print("Memoria libre 2: ");
        Serial.print(ESP.getFreeHeap());
        Serial.println(" bytes");

        shouldPublishTestMessage = true;

        Buzzer::finishedSound();
    }
}

void setup() {
    // Inicializar comunicación a 115200 baudios
    pinMode(BUTTON_OFFSET, INPUT);
    pinMode(BUTTON_START, INPUT);
    pinMode(BUZZER_PIN, OUTPUT);
    Serial.begin(115200);
    while (!Serial) {
        ;
    }
    // Es importante esperar a que se abra el monitor serial
    delay(3000);

    // Inicializar el sistema de archivos SPIFFS
    if (!dataManager.begin()) {
        Serial.println("Error al inicializar el sistema de archivos SPIFFS");
        return;
    }

    // Configuración de conexión WiFi
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Conectando a WiFi...");
    }

    Serial.println("Conectado a la red WiFi");

    // Conexión I2C
    Wire.begin();

    // Inicializar el sensor MPU6050
    Serial.println("Configurando MPU6050...");
    sensorManager.initialize();
    delay(500);
    // Comprobar la conexión con el sensor
    if (!sensorManager.getMPU6050().testConnection()) {
        Serial.println("Error: No se pudo conectar con el MPU6050.");
        while (1)
            ; // Detener la ejecución
    } else {
        Serial.println("Conexión con el MPU6050 establecida.");
    }

    delay(2000); // Espera para estabilizar el sensor

    // Configuración del MPU6050

    if (sensorManager.getConfigSensor()->setScaleRange()) {
        Serial.print("Configuración de rangos de escala completa exitosa.");
    } else {
        Serial.print("Error en la configuración de rangos de escala completa.");
    }

    if (sensorManager.getConfigSensor()->setSamplingFrequency(4, 0)) {
        Serial.print("Frecuencia de muestreo configurada correctamente.");
    } else {
        Serial.print("Error al configurar la frecuencia de muestreo.");
    }

    // Configuración MQTT
    client.begin(mqtt_server, mqtt_port, net);
    client.onMessage(messageReceived);

    connectToMqtt();
}

void loop() {
    client.loop();

    if (!client.connected()) {
        connectToMqtt();
    }

    if (digitalRead(BUTTON_OFFSET) == HIGH) {
        Serial.print("Start offset configuration");
        Buzzer::startSound();

        if (sensorManager.getConfigSensor()->setOffsets(5)) {
            Buzzer::configurationDoneSound();
            Serial.print("Offsets calibrados correctamente.");
            Buzzer::finishedSound();
        } else {
            Serial.print("Error durante la calibración de offsets.");
        }
    }

    if (shouldPublishTestMessage) {
        sendLargeMessage(messageTosend);

        String messageFinish = ".";

        // Publicar mensaje de finalización con QoS 1
        if (client.publish(mqtt_topicSendResult, messageFinish, false, 1)) {
            Serial.println("Finished sent.");
        } else {
            Serial.print("Error al enviar el finished ");
            Serial.print(" Error: ");
            Serial.println(client.lastError());
        }

        shouldPublishTestMessage = false; // Restablecer la bandera
    }
}
