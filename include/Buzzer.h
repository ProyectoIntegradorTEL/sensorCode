// Buzzer.h
#ifndef BUZZER_H
#define BUZZER_H

#include <Arduino.h>

class Buzzer {
public:
    // Pin digital conectado al buzzer
    static const uint8_t PIN_BUZZER = 5;

    // Métodos estáticos para reproducir sonidos específicos
    static void startSound();
    static void configurationDoneSound();
    static void finishedSound();
};

#endif // BUZZER_H
