// Buzzer.cpp
#include "Buzzer.h"

// Método para reproducir el sonido de inicio
void Buzzer::startSound() {
    // Reproducir una secuencia de tonos ascendentes para indicar inicio
    tone(PIN_BUZZER, 523, 500); // Nota C5
    delay(1000);
    tone(PIN_BUZZER, 587, 500); // Nota D5
    delay(1000);
    tone(PIN_BUZZER, 659, 500); // Nota E5
    delay(2000);
    noTone(PIN_BUZZER); // Detener el tono
}

// Método para reproducir el sonido de configuración exitosa
void Buzzer::configurationDoneSound() {
    // Reproducir un tono de confirmación para indicar configuración exitosa
    tone(PIN_BUZZER, 784, 500); // Nota G5
    delay(2000);
    noTone(PIN_BUZZER); // Detener el tono
}

// Método para reproducir el sonido de terminación
void Buzzer::finishedSound() {
    // Reproducir una secuencia de tonos descendentes para indicar terminación
    tone(PIN_BUZZER, 659, 500); // Nota E5
    delay(250);
    tone(PIN_BUZZER, 587, 500); // Nota D5
    delay(250);
    tone(PIN_BUZZER, 523, 500); // Nota C5
    delay(250);
    noTone(PIN_BUZZER); // Detener el tono
}
