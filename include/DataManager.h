#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <ArduinoJson.h>
#include "SPIFFS.h"

class DataManager {
public:
    // Inicializa el sistema de archivos SPIFFS
    bool begin();

    // Escribe un JsonDocument 
    void writeJsonToSPIFFS(JsonDocument doc);

    // Lee un JsonDocument desde SPIFFS y lo imprime en el Serial Monitor
    JsonDocument  readJsonFromSPIFFS();

private:
    const char* filename = "/data.json";  // Archivo donde se almacenarán los datos
};

#endif  // DATAMANAGER_H
