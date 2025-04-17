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

    // Définit la fréquence de l'oscillateur
    void setWaveform(WaveType waveform);
    void setFrequency(float frequency);
    void setFrequencyOffset(float offset);
    void setEnabled(bool enabled);


    void fillBuffer(float* buffer, unsigned long framesPerBuffer, uint32_t sampleRate);


private:

    float frequency = 440.0f;

    float frequencyOffset =0.0f;
    float phase = 0.0f;

    bool enabled;
    static constexpr float AMPLITUDE = 0.5f;
    static constexpr float TWO_PI = 2.0f * std::numbers::pi_v<float>;


    float generateSine(float phase) const;
    float generateSquare(float phase) const;
    float generateSaw(float phase) const;
    WaveType waveType;

    float generateSample(float phase) const;


};

#endif // OSCILLATOR_H