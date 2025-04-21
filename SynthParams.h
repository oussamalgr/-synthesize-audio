

#ifndef SIMPLE_SYNTH_SYNTHPARAMETERS_H
#define SIMPLE_SYNTH_SYNTHPARAMETERS_H

#include <mutex>
#include "Oscillator.h"

using Guard = std::lock_guard<std::mutex>;

struct SharedSynthParameters;

struct SynthPOD {
    // Parameters for oscillators
    bool osc1Enabled{false};
    bool osc2Enabled{false};
    WaveType osc1WaveType{WaveType::SINE};
    float osc1FrequencyOffset{0.0f};

    // Parameters for Envelope
    float attack{0.01f};
    float release{0.5f};

    // Parameters for filter
    float filterCutoff{20.0f};
    float filterResonance{0.0f};

    // Parameters for delay
    float delayTime{0.1f};
    float delayMix{0.0f};

    // Parameters active note
    float activeFrequency{0.0f};
    bool noteOn{false};

    SynthPOD &operator=(const SynthPOD &other);

    SynthPOD &operator=(const SharedSynthParameters &other);
};

struct SharedSynthParameters {
    mutable std::mutex mtx;
    SynthPOD params;
    SharedSynthParameters() = default;
    SharedSynthParameters &operator=(const SynthPOD &other);
};


#endif // SIMPLE_SYNTH_SYNTHPARAMETERS_H
