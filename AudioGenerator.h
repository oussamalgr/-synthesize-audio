#ifndef SIMPLE_SYNTH_AUDIOGENERATOR_H
#define SIMPLE_SYNTH_AUDIOGENERATOR_H

#include "Envelope.h"
#include "Oscillator.h"
#include "portaudio.h"
#include "SynthParams.h"

class AudioGenerator {
public:
    void init();
    AudioGenerator(SharedSynthParameters& sharedParams);
private:
    static int audioCallback( const void *inputBuffer, void *outputBuffer,
                              unsigned long framesPerBuffer,
                              const PaStreamCallbackTimeInfo* timeInfo,
                              PaStreamCallbackFlags statusFlags,
                              void *userData );

    double currentTimeInSeconds {0.0};
    void applyEffects(float* mixBuffer,unsigned long frame_per_buffer,const SynthPOD& params);
    static float phase;
    float delay_time{0.1f};
    float delay_mix{0.0f};
    float attack{0.0f};
    float filter_cutoff{20.0f};
    float filter_resonance{0.0f};
    float release{0.0f};
    float frequency{0.0f};
    int current_item = 0;
    Oscillator OSC1;
    Oscillator OSC2;
    Envelope envelope;
    void processAudio(float* out,unsigned long frame_per_buffer);
    SharedSynthParameters& shared;



};


#endif //SIMPLE_SYNTH_AUDIOGENERATOR_H
