#ifndef SIMPLE_SYNTH_AUDIOGENERATOR_H
#define SIMPLE_SYNTH_AUDIOGENERATOR_H

#include "Envelope.h"
#include "Oscillator.h"
#include "portaudio.h"
#include "../shared/SynthParams.h"
#include "Filter.h"
#include "Delay.h"

/**
 * @class AudioGenerator
 * @brief Generates audio signals by combining oscillators and applying effects.
 *
 * This class handles audio synthesis including oscillator setup, envelope shaping,
 * filtering, and delay effects.
 *
 * The audioCallback method is repeatedly called  to fill audio buffers.
 */
class AudioGenerator {
public:
    void init();
    /**
     * Constructs the AudioGenerator with shared synthesis parameters
     * @param sharedParams  Reference to shared parameters used for thread-safe communication.
    */
    AudioGenerator(SharedSynthParameters& sharedParams);
private:
    static int audioCallback( const void *inputBuffer, void *outputBuffer,
                              unsigned long framesPerBuffer,
                              const PaStreamCallbackTimeInfo* timeInfo,
                              PaStreamCallbackFlags statusFlags,
                              void *userData );


    /**
     * @brief applies envelope, filter, and delay effects to the audio buffer.
     *
     * Modifies the input buffer (mixBuffer) by applying amplitude envelope, filtering and delay
     * based on the provided synthesis parameters
     *
     *
     * @param mixBuffer audio buffer to process.
     * @param frame_per_buffer Number of audio frames in the buffer.
     * @param params current synthesis parameters to control the effects.
    */
    void applyEffects(float* mixBuffer,unsigned long frame_per_buffer,const SynthPOD& params);
    Filter filter; // filter
    Oscillator OSC1; // Oscillator 1
    Oscillator OSC2; // Oscillator 2
    Envelope envelope; // Envelope
    Delay delay; // delay

    /**
     * @brief Fill the output buffer with audio from oscillators and applies effects.
     *
     * Generates audio using enabled oscillators, applies envelope, filter, and delay
     * then scales the output control overall volume.
     *
     *
     * @param out output buffer to fill with audio samples.
     * @param frame_per_buffer The number of audio frames to process.
    */
    void processAudio(float* out,unsigned long frame_per_buffer);
    SharedSynthParameters& shared; // shared params between ui and audio threads
};


#endif //SIMPLE_SYNTH_AUDIOGENERATOR_H
