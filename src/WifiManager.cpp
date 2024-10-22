#include <WiFi.h>

class WiFiManager {
private:
    const char* ssid;
    const char* password;

public:
    WiFiManager(const char* ssid, const char* password) : ssid(ssid), password(password) {}

    void connect() {
        WiFi.begin(ssid, password);
        while (WiFi.status() != WL_CONNECTED) {
            delay(1000);
            Serial.println("Connecting to WiFi...");
        }
        Serial.println("Connected to WiFi!");
    }

    void reconnect() {
        if (WiFi.status() != WL_CONNECTED) {
            connect();
        }
    }
};
