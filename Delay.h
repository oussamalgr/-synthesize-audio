//
// Created by oussa on 10-04-25.
//

#ifndef DELAY_H
#define DELAY_H

#include <vector>
/**
 * @class Delay
 * @brief A stereo delay effect processor with adjustable time and mix.
 *
 * This class implements a basic stereo delay (echo) effect using a circular buffer.
 *
 * The delay is applied in-place on interleaved stereo buffers (L, R).
*/
class Delay {
private:

    // Ring buffer to store delayed samples (stereo)
    std::vector<float> delayBuffer;

    // Ring buffer parameters
    size_t bufferSize; // buffer size calculate in the constructor
    size_t writeIndex;  // Position where new samples are written
    size_t readIndex;   // Position where delayed samples are read

    // Delay parameters
    float delayTime;
    float delayMix;

    /**
     * @brief Calculates the number of samples corresponding to the current delay time.
     *
     * @return The number of delay samples.
    */
    [[nodiscard]] size_t calculateDelaySamples() const;

    /**
     * @brief Updates the read index based on the current write index and delay time.
     *
     * This ensures correct circular buffer behavior for accessing delayed samples.
    */
    void updateReadIndex();

public:
    /**
     * @brief Constructs a Delay object and initializes internal buffers.
     *
     * Sets default delay time and mix, allocates buffer space, and sets up read/write indices.
    */
    Delay();
    /**
     * @brief Sets the delay time in seconds.
     *
     * Ensures the delay time is within the allowed range [0.1, 2.0] seconds
     * and updates the internal read index accordingly.
     *
     * @param seconds The desired delay duration in seconds.
    */
    void setDelayTime(float seconds);

    /**
     * @brief Sets the wet/dry mix for the delay effect.
     *
     * A value of 0.0 means fully dry (no delay), and 1.0 means fully wet (only delayed signal).
     *
     * @param mix A float between 0.0 and 1.0.
    */
    void setDelayMix(float mix);


    /**
     * @brief Applies the delay effect to a stereo audio buffer.
     *
     * For each stereo frame, the delayed signal is read from the buffer and mixed with the input signal.
     * The result is then written back to the delay buffer for future use.
     *
     * This function uses a circular buffer approach with separate read/write indices.
     * It modifies the buffer in-place.
     *
     * @param buffer Pointer to the interleaved stereo audio buffer (L, R).
     * @param framesPerBuffer The number of stereo frames to process.
    */
    void applyToBuffer(float* buffer, unsigned long framesPerBuffer);
};

#endif //DELAY_H