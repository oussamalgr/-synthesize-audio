//
// Created by oussa on 27-05-25.
//


#ifndef AUDIOCONSTANTS_H
#define AUDIOCONSTANTS_H

#include <numbers>

namespace AudioConstants {

    // Audio system parameters
    constexpr int SAMPLE_RATE = 44100;
    constexpr int FRAMES_PER_BUFFER = 256;

    // Audio processing constants
    constexpr float AMPLITUDE = 0.5f;
    constexpr float TWO_PI = 2.0f * std::numbers::pi_v<float>;
    constexpr float PI = std::numbers::pi_v<float>;


}


#endif //AUDIOCONSTANTS_H
