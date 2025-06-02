#include <iostream>
#include "AudioGenerator.h"
#include <cmath>
#include "AudioConstants.h"


AudioGenerator::AudioGenerator(SharedSynthParameters &sharedParams): shared(sharedParams) {
}


void AudioGenerator::init() {
    PaError errorInit = Pa_Initialize();


    if (errorInit != paNoError) {
        std::cerr << "PortAudio error in Pa_Initialize(): "
                << Pa_GetErrorText(errorInit) << std::endl;
        return;
    }

    PaError errorStream;
    PaStream *stream;

    errorStream = Pa_OpenDefaultStream(&stream,
                                       0,
                                       2,
                                       paFloat32,
                                       AudioConstants::SAMPLE_RATE,
                                       AudioConstants::FRAMES_PER_BUFFER,
                                       audioCallback,
                                       this);
    errorStream = Pa_StartStream(stream);
    if (errorStream != paNoError) {
        std::cerr << "PortAudio error in Pa_StartStream(): "
                << Pa_GetErrorText(errorStream) << std::endl;
    }
}


int AudioGenerator::audioCallback(const void *inputBuffer,
                                  void *outputBuffer,
                                  unsigned long framesPerBuffer,
                                  const PaStreamCallbackTimeInfo *timeInfo,
                                  PaStreamCallbackFlags statusFlags,
                                  void *userData) {

    // convert void pointer to proper types : outputBuffer to float array and userData to AudioGenerator instance
    auto *out = reinterpret_cast<float *>(outputBuffer);
    // Convert userData from void* to AudioGenerator* to access class members in the static callback
    auto *audioGen = static_cast<AudioGenerator *>(userData);

    // The 'out' buffer is reused on each audioCallback call.
    // If not cleared, it may contain leftover values from previous use.
    // Resetting it avoids noise, artifacts, or unintended sound.
    for (unsigned long i = 0; i < framesPerBuffer * 2; ++i) {
        out[i] = 0.0f;
    }
    // Delegates audio signal generation to this class by calling processAudio
    audioGen->processAudio(out, framesPerBuffer);


    return 0;
}


void AudioGenerator::processAudio(float *out, unsigned long frame_per_buffer) {


    // Copy shared parameters into a local SynthPOD variable.
    // Avoids direct access to shared data during processing.
    // Thread-safe => See SynthParams struct for details.
    SynthPOD localParams = shared;

    //Params for Oscillator 1
    OSC1.setWaveform(localParams.osc1WaveType);
    OSC1.setFrequency(localParams.activeFrequency);
    OSC1.setFrequencyOffset(localParams.osc1FrequencyOffset);
    OSC1.setEnabled(localParams.osc1Enabled);

    // params for Oscillator 2
    OSC2.setWaveform(WaveType::SAW);
    OSC2.setFrequency(localParams.activeFrequency);
    OSC2.setEnabled(localParams.osc2Enabled);

    //Enabling Oscillator 1
    if (localParams.osc1Enabled) {
        OSC1.fillBuffer(out, frame_per_buffer);
    }

    //Enabling Oscillator 2
    if (localParams.osc2Enabled) {
        OSC2.fillBuffer(out, frame_per_buffer);
    }

    // Apply effects to the buffer
    applyEffects(out, frame_per_buffer, localParams);


    // Apply global output gain to reduce the overall volume of the signal.
    // Helps prevent clipping and keeps the output at a controlled level.
    for (unsigned long i = 0; i < frame_per_buffer * 2; ++i) {
        out[i] *= 0.2f;
    }
}



void AudioGenerator::applyEffects(float *mixBuffer, unsigned long frame_per_buffer, const SynthPOD &params) {

    if (params.noteOn) {
        envelope.noteOn();
    } else {
        envelope.noteOff();
    }
    //Envelope effects
    envelope.setAttack(params.attack);
    envelope.setRelease(params.release);
    envelope.applyToBuffer(mixBuffer, frame_per_buffer);

    //filter effects
    filter.setCutoff(params.filterCutoff);
    filter.setResonance(params.filterResonance);
    filter.applyToBuffer(mixBuffer, frame_per_buffer);

    // delay effects
    delay.setDelayMix(params.delayMix);
    delay.setDelayTime(params.delayTime);
    delay.applyToBuffer(mixBuffer, frame_per_buffer);
}
