#ifndef OSCILLATOR_H
#define OSCILLATOR_H

#include <cmath>
#include <cstdint>
#include <numbers>

enum class WaveType {
    SINE,
    SQUARE,
    SAW
};

class Oscillator {
public:
    Oscillator();

    void setWaveform(WaveType waveform);
    void setFrequency(float frequency);
    void setFrequencyOffset(float offset);
    void setEnabled(bool enabled);
    void fillBuffer(float* buffer, unsigned long framesPerBuffer);

private:
    float frequency;
    float frequencyOffset;
    float phase;

    bool enabled;


    [[nodiscard]] float generateSine(float phase) const;
    [[nodiscard]] float generateSquare(float phase) const;
    [[nodiscard]] float generateSaw(float phase) const;
    WaveType waveType;

    [[nodiscard]] float generateSample(float phase) const;


};

#endif // OSCILLATOR_H