#include "Envelope.h"

#include <iostream>
constexpr int SAMPLE_RATE{44100};



// Documentation  of the code will be completed in a future update

Envelope::Envelope() : 

    currentValue(0.0f),
    targetValue(0.0f), 
    attack(0.0f),
    release(0.0f),
    note_pressed(false) {}

void Envelope::setAttack(float seconds) {
    attack = seconds > 0.0f ? 1.0f / (seconds * SAMPLE_RATE) : 1.0f;
}

void Envelope::setRelease(float seconds) {
    release = seconds > 0.0f ? 1.0f / (seconds * SAMPLE_RATE) : 1.0f;
}

void Envelope::noteOn() {
    note_pressed = true;
    targetValue = 1.0f;
}

void Envelope::noteOff() {
    note_pressed = false;
    targetValue = 0.0f;
}

float Envelope::process() {
    if (note_pressed) {
        if (currentValue < targetValue) {
            currentValue += attack;
            if (currentValue > targetValue) currentValue = targetValue;
        }
    } else {
        if (currentValue > targetValue) {
            currentValue -= release;
            if (currentValue < targetValue) currentValue = targetValue;

        }
    }
    return currentValue;
}

void Envelope::applyToBuffer(float* buffer, unsigned long framesPerBuffer) {
    for (unsigned long i = 0; i < framesPerBuffer; i++) {
        float envelopeValue = process();
        // Multiply each sample by the envelope value to modulate its amplitude
        buffer[i * 2] *= envelopeValue;
        buffer[i * 2 + 1] *= envelopeValue;
    }
}