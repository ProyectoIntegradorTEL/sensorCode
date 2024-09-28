#include "DataManager.h"
#include <Arduino.h>

// Inicializa LittleFS y retorna true si tuvo éxito, false en caso de error
bool DataManager::begin() {
   if (!SPIFFS.begin(true)) {
        Serial.println("Error al montar SPIFFS");
        return false;
    }
    return true;
}

// Escribe un JsonDocument en el archivo /data.json en LittleFS
void DataManager::writeJsonToSPIFFS(JsonDocument doc) {
    
    // Abrir el archivo en modo escritura. Para adjuntar datos al archivo, usar FILE_APPEND en lugar de FILE_WRITE
    File file = SPIFFS.open("/data.json", FILE_WRITE);
    if (!file) {
        Serial.println("Error al abrir el archivo para escribir");
        return;
    }

    // Serializar el objeto JSON y escribirlo en el archivo
    if (serializeJson(doc, file) == 0) {
        Serial.println("Error al serializar el JSON y escribirlo en el archivo");
    } else {
        Serial.println("JSON escrito correctamente en /data.json");
    }

    // Cerrar el archivo
    file.close();
}

JsonDocument  DataManager::readJsonFromSPIFFS() {
    // Abrir el archivo en modo lectura
    JsonDocument doc;
    File file = SPIFFS.open("/data.json", FILE_READ);
    if (!file) {
        Serial.println("Error al abrir el archivo para leer");
        return doc; // Retorna un documento JSON vacío en caso de error
    }

    // Crear un buffer para leer el contenido del archivo
    DeserializationError error = deserializeJson(doc, file);
    if (error) {
        Serial.print(F("Error al deserializar el archivo JSON: "));
        Serial.println(error.c_str());
        return doc; // Retorna un documento JSON vacío en caso de error
    }

    // Imprimir el contenido del JSON en el Serial Monitor
    serializeJsonPretty(doc, Serial);
    return doc;
    // Cerrar el archivo
    file.close();
}