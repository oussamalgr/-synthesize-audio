//
// Created by oussa on 10-04-25.
//

#include "Filter.h"
#include "AudioConstants.h"
#include <iostream>
#include <math.h>
#include <ostream>

Filter::Filter() : cutoff(20000.0f),
                   resonance(0.0f),
                   a0(0), a1(0), a2(0), b1(0),
                   b2(0), x1L(0), x2L(0), y1L(0), y2L(0),
                   x1R(0), x2R(0), y1R(0), y2R(0) {
    // init the internal parameters of the filter
    calculateInternParams();
}

void Filter::setCutoff(float frequency) {

    if (frequency != cutoff) // Avoid unnecessary calculations
    {
        this->cutoff = frequency;
        // Recalculate filter coefficients when the cutoff frequency changes
        calculateInternParams();
    }
}

void Filter::setResonance(float value) {
    if (resonance != value) {
        this->resonance = value;
        // Recalculate filter coefficients when the resonance frequency changes

        calculateInternParams();
    }
}


void Filter::calculateInternParams() {
    float res = resonance;
    if (res >= 0.99f) res = 0.99f;
    float q = 0.5 / (1.0 - res);
    float omega = 2.0 * M_PI * cutoff / AudioConstants::SAMPLE_RATE;
    float alpha = std::sin(omega) / (2.0 * q);
    float cosw = std::cos(omega);
    float norm = 1.0 / (1.0 + alpha);
    float one_minus_cosw = 1.0 - cosw;
    a0 = one_minus_cosw * 0.5 * norm;
    a1 = one_minus_cosw * norm;
    a2 = one_minus_cosw * 0.5 * norm;
    b1 = -2.0 * cosw * norm;
    b2 = (1.0 - alpha) * norm;
}


void Filter::applyToBuffer(float *buffer, unsigned long framesPerBuffer) {

    for (unsigned long i = 0; i < framesPerBuffer; i++) {
        // Process Left Channel
        float inputL = buffer[i * 2]; // Get left channel input sample
        float outputL = a0 * inputL + a1 * x1L + a2 * x2L - b1 * y1L - b2 * y2L; // Apply filter equation

        // Update history for left channel
        x2L = x1L;
        x1L = inputL;
        y2L = y1L;
        y1L = outputL;
        buffer[i * 2] = outputL;  // Store filtered left sample

        //Process Right Channel
        float inputR = buffer[i * 2 + 1]; // Get right channel input sample
        float outputR = a0 * inputR + a1 * x1R + a2 * x2R - b1 * y1R - b2 * y2R; // Apply filter equation

        // Update history for right channel
        x2R = x1R;
        x1R = inputR;
        y2R = y1R;
        y1R = outputR;

        buffer[i * 2 + 1] = outputR;// Store filtered right sample
    }
}
