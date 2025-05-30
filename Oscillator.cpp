#include "Oscillator.h"
#include <cmath>
#include <iostream>
#include "AudioConstants.h"






Oscillator::Oscillator() : frequency(0.0f),
        frequencyOffset(0.0f),
        phase(0),
        enabled(false),
        waveType(WaveType::SINE)
{}




void Oscillator::setWaveform(WaveType waveform) {
    this->waveType = waveform;
}

void Oscillator::setFrequency(float frequency) {
    this->frequency = frequency;
}

void Oscillator::setFrequencyOffset(const float offset) {
    this->frequencyOffset = offset;
}

void Oscillator::setEnabled(const bool enabled) {
    this->enabled = enabled;
}


void Oscillator::fillBuffer(float *buffer, unsigned long framesPerBuffer) {
    if (!enabled) {
        return;
    }

    float currentFrequency = frequency + frequencyOffset;
    const float phaseStep = AudioConstants::TWO_PI * currentFrequency / AudioConstants::SAMPLE_RATE;

    for (unsigned long i = 0; i < framesPerBuffer; i++) {
        float sample = generateSample(phase);
        buffer[i * 2] += sample;
        buffer[i * 2 + 1] += sample;

        phase = std::fmod(phase + phaseStep, AudioConstants::TWO_PI);
    }
}


float Oscillator::generateSample(float phase) const{
    switch (waveType) {
        case WaveType::SINE:
            return generateSine(phase);

        case WaveType::SQUARE:
            return generateSquare(phase);

        case WaveType::SAW:
            return generateSaw(phase);
        default:
                return 0.0f;
    }

}


float Oscillator::generateSine(float phase) const {
    return AudioConstants::AMPLITUDE * std::cos(phase);

}

float Oscillator::generateSquare(float phase) const {
    return AudioConstants::AMPLITUDE * (std::cos(phase) >= 0.0f ? 1.0f : -1.0f);
}

float Oscillator::generateSaw(float phase) const {
    float normalizedPhase = phase / AudioConstants::TWO_PI;
    return AudioConstants::AMPLITUDE * (2.0f * normalizedPhase - 1.0f);
}
