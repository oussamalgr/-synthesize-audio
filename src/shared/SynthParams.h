

#ifndef SIMPLE_SYNTH_SYNTHPARAMETERS_H
#define SIMPLE_SYNTH_SYNTHPARAMETERS_H

#include <mutex>
#include "../audio/Oscillator.h"

using Guard = std::lock_guard<std::mutex>;

struct SharedSynthParameters;

/**
 * @struct SynthPOD
 * Plain Old Data structure holding synthesizer parameters.
 * Contains simple  oscillator, envelope, filter, delay, and note state parameters.
 */
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
    float filterCutoff{20000.0f};
    float filterResonance{0.0f};

    // Parameters for delay
    float delayTime{0.1f};
    float delayMix{0.0f};

    // Parameters active note
    float activeFrequency{0.0f};
    bool noteOn{false};

    /**
     * Assignment operator to copy parameters from a thread-safe SharedSynthParameters instance.
     * Locks the mutex of the source object during copy.
    */
    SynthPOD &operator=(const SharedSynthParameters &other);
};


/**
 * @struct SharedSynthParameters
 * Extends SynthPOD by adding a mutex to protect access to parameters across threads.
 * Provides assignment operator from SynthPOD to safely update shared parameters.
*/
struct SharedSynthParameters: SynthPOD {
    mutable std::mutex mtx;
    /**
     * Assignment operator to copy parameters from a SynthPOD instance.
     * Thread-safe update, locks this object's mutex during assignment.
    */
    SharedSynthParameters &operator=(const SynthPOD &other);
};


#endif // SIMPLE_SYNTH_SYNTHPARAMETERS_H
