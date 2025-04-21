#include "SynthParams.h"

SynthPOD& SynthPOD::operator=(const SynthPOD& other) {
    osc1Enabled = other.osc1Enabled;
    osc2Enabled = other.osc2Enabled;
    osc1WaveType = other.osc1WaveType;
    osc1FrequencyOffset = other.osc1FrequencyOffset;
    attack = other.attack;
    release = other.release;
    filterCutoff = other.filterCutoff;
    filterResonance = other.filterResonance;
    delayTime = other.delayTime;
    delayMix = other.delayMix;
    activeFrequency = other.activeFrequency;
    noteOn = other.noteOn;
    return *this;
}

SynthPOD& SynthPOD::operator=(const SharedSynthParameters& other) {
    Guard guard(other.mtx);
    *this = other.params;
    return *this;
}

SharedSynthParameters& SharedSynthParameters::operator=(const SynthPOD& other) {
    Guard guard(mtx);
    params = other;
    return *this;
}