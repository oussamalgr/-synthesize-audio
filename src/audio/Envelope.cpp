#include "Envelope.h"
#include "AudioConstants.h"
#include <iostream>


// Documentation  of the code will be completed in a future update



Envelope::Envelope() : currentValue(0.0f),
                       attack(0.0f),
                       release(0.0f),
                       note_pressed(false) {
}



void Envelope::setAttack(float seconds) {
    // seconds the time of the attack
    // seconds * SAMPLE_RATE = total number of samples needed to reach the target (1.0 for attack)
    attack = seconds > 0.001f ? 1.0f / (seconds * AudioConstants::SAMPLE_RATE) : 1.0f / (0.001f * AudioConstants::SAMPLE_RATE);
}




void Envelope::setRelease(float seconds) {
    // Same logic as the setAttack method
    release = seconds > 0.001f ? 1.0f / (seconds * AudioConstants::SAMPLE_RATE) : 1.0f / (0.001f * AudioConstants::SAMPLE_RATE);
}



void Envelope::noteOn() {
    note_pressed = true;
}



void Envelope::noteOff() {
    note_pressed = false;
}


float Envelope::process() {
    if (note_pressed) {
        // If a note is currently held, ramp up towards the target value (1.0)
        currentValue += attack; // Increase envelope value using attack increment
        if (currentValue > 1.0f) currentValue = 1.0f; // Limit to the maximum value
    } else {
        // If the note is released, ramp down towards the target value (0.0)
        currentValue -= release; // Decrease envelope value using release increment
        if (currentValue < 0.0f) currentValue = 0.0f; // Limit to the minimum value
    }
    return currentValue;
}






void Envelope::applyToBuffer(float *buffer, unsigned long framesPerBuffer) {
    for (unsigned long i = 0; i < framesPerBuffer; i++) {
        float envelopeValue = process();
        // Multiply each sample by the envelope value to modulate its amplitude
        buffer[i * 2] *= envelopeValue;
        buffer[i * 2 + 1] *= envelopeValue;
    }
}
