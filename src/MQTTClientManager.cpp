#include <PubSubClient.h>
#include <WiFiClientSecure.h> //Usar cliente seguro

class MQTTClientManager {
private:
    WiFiClientSecure espClient;
    PubSubClient mqttClient;
    const char* broker ;
    const int port ;
    const char* topic ;

    static void callback(char* topic, byte* message, unsigned int length) {
        Serial.print("Message arrived on topic: ");
        Serial.print(topic);
        Serial.print(". Message: ");
        String messageTemp;
        for (int i = 0; i < length; i++) {
            Serial.print((char)message[i]);
            messageTemp += (char)message[i];
        }
        Serial.println();
        if (messageTemp == "start") {
            Serial.println("Starting the test...");
            // Iniciar la prueba
            // TODO: llamar la función que inicia la prueba
        }
    }

public:
    MQTTClientManager(const char* broker, int port, const char* topic) 
        : broker(broker), port(port), topic(topic), mqttClient(espClient) {}

    void connect() {
        mqttClient.setServer(broker, port);
        mqttClient.setCallback(callback);
        espClient.setInsecure(); //Usar conexión insegura
        
        while (!mqttClient.connected()) {
            Serial.println("Connecting to MQTT...");
            if (mqttClient.connect("ESP32Client")) {
                Serial.println("Connected to MQTT broker!");
                mqttClient.subscribe(topic);
            } else {
                Serial.print("Failed to connect, rc=");
                Serial.print(mqttClient.state());
                delay(5000);
            }
        }
    }

    void loop() {
        mqttClient.loop();
    }
};
