//
// Created by oussa on 10-04-25.
//

#ifndef DELAY_H
#define DELAY_H

#include <vector>

class Delay {
private:
    static constexpr int SAMPLE_RATE = 44100;


    // Ring buffer to store delayed samples (stereo)
    std::vector<float> delayBuffer;

    // Ring buffer parameters
    size_t bufferSize; // buffer size calculate in the constructor
    size_t writeIndex;  // Position where new samples are written
    size_t readIndex;   // Position where delayed samples are read

    // Delay parameters
    float delayTime;
    float delayMix;

    // Helper function to calculate delay in samples
    [[nodiscard]] size_t calculateDelaySamples() const;

    // Helper function to update read index based on delay time
    void updateReadIndex();

public:
    Delay();

    void setDelayTime(float seconds);
    void setDelayMix(float mix);

    void applyToBuffer(float* buffer, unsigned long framesPerBuffer);
};

#endif //DELAY_H