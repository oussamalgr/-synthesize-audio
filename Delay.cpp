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

    // Compute the maximum number of mono samples needed for the longest delay time.
    bufferSize = static_cast<size_t>(MAX_DELAY_TIME * AudioConstants::SAMPLE_RATE);

    // Initialize the delay buffer with zeros.
    // Size is doubled to accommodate interleaved stereo audio (Left and Right channels)
    delayBuffer.resize(bufferSize * 2, 0.0f);

    // Set the initial read index based on the current delay time.
    updateReadIndex();
}


void Delay::setDelayTime(float seconds) {
    // Clamp delay time to valid range
    delayTime = std::clamp(seconds, MIN_DELAY_TIME, MAX_DELAY_TIME);
    // Update the read index to reflect the new delay time in the circular buffer.
    updateReadIndex();
}


void Delay::setDelayMix(float mix) {
    // Clamp the input mix value to the valid range [0.0, 1.0].
    delayMix = std::clamp(mix, 0.0f, 1.0f);
}



size_t Delay::calculateDelaySamples() const {
    // Calculate and return the number of samples corresponding to the current delay time.
    // This converts the delay time from seconds to samples based on the sample rate.
    return delayTime * AudioConstants::SAMPLE_RATE;
}



void Delay::updateReadIndex() {

    // Calculate the number of samples that correspond to the current delay time
    size_t delaySamples = calculateDelaySamples();

    // If the write index is ahead of or equal to the delaySamples,
    // move the read index backwards by delaySamples without going below zero
    if (writeIndex >= delaySamples) {
        readIndex = writeIndex - delaySamples;
    } else {
        // If writeIndex is less than delaySamples, wrap the read index
        // to the end of the circular buffer to maintain the correct delay offset.
        readIndex = bufferSize - (delaySamples - writeIndex);
    }
}


void Delay::applyToBuffer(float* buffer, unsigned long framesPerBuffer) {
    // If delay mix is zero or less, skip processing  (no effect applied)
    if (delayMix <= 0.0f) {
        return;
    }

    for (unsigned long i = 0; i < framesPerBuffer; i++) {
        // Calculate indices for left channel in the circular delay buffer
        size_t leftReadIdx = readIndex * 2;
        size_t leftWriteIdx = writeIndex * 2;

        // Get the current input sample and the delayed sample for the left channel
        float leftInput = buffer[i * 2];
        float leftDelayed = delayBuffer[leftReadIdx];

        // Mix the delayed signal with the original input according to delayMix
        buffer[i * 2] = leftInput + delayMix * leftDelayed;

        // Store the processed sample back into the delay buffer for future reads
        delayBuffer[leftWriteIdx] = buffer[i * 2];

        // Repeat the same process for the right channel
        size_t rightReadIdx = readIndex * 2 + 1;
        size_t rightWriteIdx = writeIndex * 2 + 1;

        float rightInput = buffer[i * 2 + 1];
        float rightDelayed = delayBuffer[rightReadIdx];

        buffer[i * 2 + 1] = rightInput + delayMix * rightDelayed;

        delayBuffer[rightWriteIdx] = buffer[i * 2 + 1];

        // Advance the circular buffer indices, wrapping around the buffer size
        writeIndex = (writeIndex + 1) % bufferSize;
        readIndex = (readIndex + 1) % bufferSize;
    }
}