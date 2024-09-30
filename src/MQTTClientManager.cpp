#include <WiFiClientSecure.h>
#include <PubSubClient.h>

// Definir la clase MQTTClientManager
class MQTTClientManager {
public:
    MQTTClientManager(const char* broker, int port, const char* topic);
    void connect();
    void loop();
    void setCallback(MQTT_CALLBACK_SIGNATURE);

private:
    const char* broker;
    int port;
    const char* topic;
    WiFiClientSecure espClient;
    PubSubClient client;
};

// Implementación del constructor
MQTTClientManager::MQTTClientManager(const char* broker, int port, const char* topic)
    : broker(broker), port(port), topic(topic), client(espClient) {
    client.setServer(broker, port);  // Establece el servidor MQTT
}

// Implementación del método connect
void MQTTClientManager::connect() {
    while (!client.connected()) {
        Serial.print("Conectando al broker MQTT...");
        if (client.connect("ESP32Client")) { // Conectar al broker MQTT
            Serial.println("Conectado al broker MQTT");
            client.subscribe(topic);  // Suscribirse al topic
        } else {
            Serial.print("Error de conexión, estado: ");
            Serial.println(client.state()); // Imprimir el estado de la conexión
            delay(2000);  // Espera y reintenta la conexión
        }
    }
}

// Implementación del método loop (mantiene la conexión viva)
void MQTTClientManager::loop() {
    client.loop();
}

// Implementación del método setCallback (establece el callback de mensajes)
void MQTTClientManager::setCallback(MQTT_CALLBACK_SIGNATURE) {
    client.setCallback(callback);
}
