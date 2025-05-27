//
// Created by oussa on 10-04-25.
//

#include "Filter.h"

#include <iostream>
#include <math.h>
#include <ostream>
constexpr int SAMPLE_RATE{44100};

Filter::Filter() : cutoff(20000.0f),
                   resonance(0.0f),
                   a0(0), a1(0), a2(0), b1(0),
                   b2(0), x1L(0), x2L(0), y1L(0), y2L(0),
                   x1R(0), x2R(0), y1R(0), y2R(0) {
    calculateInternParams();
}

void Filter::setCutoff(float frequency) {
    if (frequency != cutoff) {
        this->cutoff = frequency;
        calculateInternParams();
    }
}

void Filter::setResonance(float value) {
    if (resonance != value) {
        this->resonance = value;
        calculateInternParams();
    }
}


void Filter::calculateInternParams() {
    float res = resonance;
    if (res >= 0.99f) res = 0.99f;
    float q = 0.5 / (1.0 - res);
    float omega = 2.0 * M_PI * cutoff / SAMPLE_RATE;
    float alpha = std::sin(omega) / (2.0 * q);
    float cosw = std::cos(omega);
    float norm = 1.0 / (1.0 + alpha);

    a0 = (1.0 - cosw) * 0.5 * norm;
    a1 = (1.0 - cosw) * norm;
    a2 = (1.0 - cosw) * 0.5 * norm;
    b1 = -2.0 * cosw * norm;
    b2 = (1.0 - alpha) * norm;
}


void Filter::applyToBuffer(float *buffer, unsigned long framesPerBuffer) {
    for (unsigned long i = 0; i < framesPerBuffer; i++) {
        float inputL = buffer[i * 2];
        float outputL = a0 * inputL + a1 * x1L + a2 * x2L - b1 * y1L - b2 * y2L;
        x2L = x1L;
        x1L = inputL;
        y2L = y1L;
        y1L = outputL;
        buffer[i * 2] = outputL;


        float inputR = buffer[i * 2 + 1];
        float outputR = a0 * inputR + a1 * x1R + a2 * x2R - b1 * y1R - b2 * y2R;
        x2R = x1R;
        x1R = inputR;
        y2R = y1R;
        y1R = outputR;

        buffer[i * 2 + 1] = outputR;
    }
}
