
#ifndef FILTER_H
#define FILTER_H

/**
 * @class Filter
 * @brief A stereo biquad low-pass filter with cutoff and resonance control.
 *
 * This class implements a basic digital low-pass biquad filter, commonly used in audio processing
 * to attenuate high frequencies. It processes stereo audio buffers using internal state variables
 * and applies a filter based on the cutoff frequency and resonance.
 */
class Filter {
private:
    float cutoff;
    float resonance;
    double a0, a1, a2, b1, b2;     // Filter coefficients
    float x1L, x2L, y1L, y2L; // input and ouput samples for left channel
    float x1R, x2R, y1R, y2R; // input and ouput samples for right channel
    /**
     * @brief Recalculates internal filter coefficients based on the current
     * cutoff frequency and resonance values.
    */
    void calculateInternParams();


public:
    /**
      * @brief Default constructor.
      *
      * Initializes the filter with a default cutoff of 20000 Hz
      * and 0.0 resonance. Internal filter coefficients and states are also initialized.
    */
    Filter();

    /**
     * @brief Sets the filter's cutoff frequency.
     *
     * This function recalculates the internal filter coefficients if the cutoff has changed.
     *
     * @param frequency Cutoff frequency.
    */
    void setCutoff(float frequency);

    /**
     * @brief Sets the filter's resonance (Q factor).
     *
     * The resonance controls the sharpness of the frequency cutoff.
     * A value near 1.0 results in a resonant peak. Values >= 0.99 are clamped.
     *
     * @param value Resonance amount in the range [0.0, 1.0].
    */
    void setResonance(float value);



    /**
     * @brief Applies the filter to a stereo audio buffer.
     *
     * Processes the buffer in-place using the current filter coefficients and history.
     * This implementation assumes the buffer is stereo interleaved (L, R, L, R...).
     *
     * @param buffer Pointer to the audio buffer.
     * @param framesPerBuffer Number of frames to process (each frame contains 2 samples).
    */
    void applyToBuffer(float *buffer, unsigned long framesPerBuffer);
};


#endif //FILTER_H
