#ifndef SIMPLE_SYNTH_AUDIOGENERATOR_H
#define SIMPLE_SYNTH_AUDIOGENERATOR_H

#include "Envelope.h"
#include "Oscillator.h"
#include "portaudio.h"
#include "SynthParams.h"
#include "Filter.h"
#include "Delay.h"

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
    Filter filter;
    Oscillator OSC1;
    Oscillator OSC2;
    Envelope envelope;
    Delay delay;
    void processAudio(float* out,unsigned long frame_per_buffer);
    SharedSynthParameters& shared;
};


#endif //SIMPLE_SYNTH_AUDIOGENERATOR_H
