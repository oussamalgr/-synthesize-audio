#ifndef OSCILLATOR_H
#define OSCILLATOR_H

#include <cmath>
#include <cstdint>


/**
 * Wave form : SINE, SQUARE, SAW
 */
enum class WaveType {
    SINE,
    SQUARE,
    SAW
};


/**
 * @class Oscillator
 * @brief A class representing the audio oscillator.
 *
 * The Oscillator class generates periodic audio waveforms such as sine, square,
 * and sawtooth waves.
 */
class Oscillator {
public:
    /**
     *  Constructor of the oscillator class
     *  initialize its attributes to default values
    */
    Oscillator();
    /**
     * Sets the waveform type of the oscillators.
     * @param waveform the waveform to use (e.g., SINE,SAW,SQUARE).
    */
    void setWaveform(WaveType waveform);
    /**
     * sets the base frequency of the oscillator.
     * @param frequency Frequency value to set.
    */
    void setFrequency(float frequency);
    /**
     * Sets the frequency offset added to the base frequency.
     * @param offset Frequency offset value.
    */
    void setFrequencyOffset(float offset);

    /**
     * Enables or disables the oscillator.
     * @param enabled True to enable, false to disable
    */
    void setEnabled(bool enabled);
    /**
     * @brief Fills the output buffer with oscillator samples for the given number of frames.
     * Adds the generated waveform samples to both left and right channels of the buffer.
     * The oscillator's phase is updated for continuous waveform generation.
     * Does nothing if the oscillator is disabled.
     * @param buffer
     *
     * @param framesPerBuffer
    */
    void fillBuffer(float* buffer, unsigned long framesPerBuffer);

private:
    float frequency;
    float frequencyOffset;
    float phase;

    bool enabled;

    /**
     * Generates a sine wave sample at the given phase.
     * @param phase Current phase .
     * @return Sine wave sample value .
    */
    [[nodiscard]] float generateSine(float phase) const;
    /**
     * Generates a square wave sample at the given phase.
     * @param phase Current phase.
     * @return Square wave sample value
    */
    [[nodiscard]] float generateSquare(float phase) const;
   /**
     * Generates a sawtooth wave sample at the given phase.
     * @param phase Current phase.
     * @return Sawtooth wave sample value scaled by amplitude.
    */
    [[nodiscard]] float generateSaw(float phase) const;

    WaveType waveType; // wave type form


    /**
     * Selects the waveform function according to the oscillator's waveType and returns
     * the corresponding sample value for the given phase.
     *
     * @param phase Current phase of the oscillator
     * @return Sample value for the waveform at the given phase
    */
    [[nodiscard]] float generateSample(float phase) const;


};

#endif // OSCILLATOR_H