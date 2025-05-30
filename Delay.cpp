//
// Created by oussa on 10-04-25.
//

#include "Delay.h"
#include <algorithm>
#include <iostream>

#include "AudioConstants.h"

constexpr float MIN_DELAY_TIME = 0.1f;  // 0.1 seconds minimum
constexpr float MAX_DELAY_TIME = 2.0f;  // 2 seconds maximum


Delay::Delay() : writeIndex(0),
                 readIndex(0),
                 delayTime(0.1f),
                 delayMix(0.0f) {

    // Multiplying MAX_DELAY_TIME * SAMPLE_RATE gives the maximum number of mono samples required to store a delay of MAX_DELAY_TIME seconds.
    bufferSize = static_cast<size_t>(MAX_DELAY_TIME * AudioConstants::SAMPLE_RATE);
    // delaybuffer is a vector that contains the audio samples stored to apply the delay.
    delayBuffer.resize(bufferSize * 2, 0.0f);  // init with 0.0f and *2 because L and R stereo

    // Initialize read index based on default delay time
    updateReadIndex();
}

/**
 *
 * This function updates the delay time of the delay effect, ensuring
 * the value stays within the allowed minimum and maximum range.
 * It then recalculates the read index based on the new delay time.
 *
 * @param seconds The desired delay time in seconds.
 */
void Delay::setDelayTime(float seconds) {
    // Clamp delay time to valid range
    delayTime = std::clamp(seconds, MIN_DELAY_TIME, MAX_DELAY_TIME);
    updateReadIndex();
}

/**
 * @brief Sets how much delayed sound is mixed with the original sound.
 *
 * The mix value controls the balance between dry (original) and wet (delayed) sound.
 * 0.0 means no delay effect, 1.0 means only delayed sound.
 * The value is limited to stay between 0.0 and 1.0.
 *
 * @param mix The delay mix level, from 0.0 to 1.0.
 */
void Delay::setDelayMix(float mix) {
    // Clamp mix to 0.0 - 1.0 range
    delayMix = std::clamp(mix, 0.0f, 1.0f);
}


/**
 * Calculate the delay samples corresponding to the delayTime
 * @return the delay samples
 */
size_t Delay::calculateDelaySamples() const {
    return delayTime * AudioConstants::SAMPLE_RATE;
}


/**
 * @brief Calculates and updates the readIndex in the circular delay buffer.
 *
 * The readIndex points to the position in the buffer where the delayed audio sample
 * should be read. It is always set behind the writeIndex by the number of samples
 * that correspond to the current delay time.
 *
 * If the writeIndex is greater than or equal to the delaySamples, readIndex is simply
 * writeIndex minus delaySamples. Otherwise, the readIndex wraps around to the end
 * of the circular buffer to maintain the correct delay distance.
 */
void Delay::updateReadIndex() {

    // number of samples corresponding to the desired delay time
    size_t delaySamples = calculateDelaySamples();

    // if write index >= to delay samples, we can step back in the buffer (without exceeding index 0)
    if (writeIndex >= delaySamples) {
        readIndex = writeIndex - delaySamples;
    } else {
        // Wrap around to the end of the buffer
        readIndex = bufferSize - (delaySamples - writeIndex);
    }
}


/**
 * Applies the delay effect to a stereo audio buffer.
 *
 * @param buffer Pointer to the audio buffer (interleaved stereo: L, R, L, R...).
 * @param framesPerBuffer Number of audio frames in the buffer.
 *
 * For each frame, the function mixes the delayed signal with the current input
 * and stores the result back into the buffer. It also updates the delay buffer
 * and manages read/write indices in a circular manner.
 */
void Delay::applyToBuffer(float* buffer, unsigned long framesPerBuffer) {
    // Early return if delay mix is 0 (no effect)
    if (delayMix <= 0.0f) {
        return;
    }

    for (unsigned long i = 0; i < framesPerBuffer; i++) {
        // Process left channel
        size_t leftReadIdx = readIndex * 2;
        size_t leftWriteIdx = writeIndex * 2;

        float leftInput = buffer[i * 2];
        float leftDelayed = delayBuffer[leftReadIdx];

        // Apply delay effect: out = out + delay_mix * buffer[index_read]
        buffer[i * 2] = leftInput + delayMix * leftDelayed;

        // Store the output in the delay buffer for future use
        delayBuffer[leftWriteIdx] = buffer[i * 2];

        // Process right channel
        size_t rightReadIdx = readIndex * 2 + 1;
        size_t rightWriteIdx = writeIndex * 2 + 1;

        float rightInput = buffer[i * 2 + 1];
        float rightDelayed = delayBuffer[rightReadIdx];

        // Apply delay effect
        buffer[i * 2 + 1] = rightInput + delayMix * rightDelayed;

        // Store the output in the delay buffer
        delayBuffer[rightWriteIdx] = buffer[i * 2 + 1];

        // Increment indices (circular buffer)
        writeIndex = (writeIndex + 1) % bufferSize;
        readIndex = (readIndex + 1) % bufferSize;
    }
}