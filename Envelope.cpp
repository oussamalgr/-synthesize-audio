#include "Envelope.h"

#include <iostream>
constexpr int SAMPLE_RATE{44100};


// Documentation  of the code will be completed in a future update

Envelope::Envelope() : currentValue(0.0f),
                       attack(0.0f),
                       release(0.0f),
                       note_pressed(false) {
}

void Envelope::setAttack(float seconds) {
    // seconds the time of the attack
    // seconds * SAMPLE_RATE = total number of samples needed to reach the target (1.0 for attack)
    attack = seconds > 0.001f ? 1.0f / (seconds * SAMPLE_RATE) : 1.0f / (0.001f * SAMPLE_RATE);
}

void Envelope::setRelease(float seconds) {
    release = seconds > 0.001f ? 1.0f / (seconds * SAMPLE_RATE) : 1.0f / (0.001f * SAMPLE_RATE);
}

void Envelope::noteOn() {
    note_pressed = true;
}

void Envelope::noteOff() {
    note_pressed = false;
}

/**
 * Updates and returns the current value of the envelope.
 *
 * This function is called once per audio sample. It modifies the envelope's
 * current value depending on whether a note is pressed or released:
 *
 * - If a note is pressed (noteOn), the envelope value increases toward 1.0 (attack phase).
 * - If the note is released (noteOff), the envelope value decreases toward 0.0 (release phase).
 *
 * The rate of change is controlled by the 'attack' and 'release' increments,
 * which are computed based on time in seconds and the sample rate.
 *
 * @return The current envelope value (between 0.0 and 1.0).
 */
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
