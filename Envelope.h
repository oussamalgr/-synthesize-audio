#ifndef ENVELOPE_H
#define ENVELOPE_H


/**
 * @class Envelope
 * @brief Implements a simple attack-release envelope generator.
 *
 * The Envelope class provides amplitude modulation for audio signals.
 */
class Envelope {
private:
    float currentValue; // Current envelope value
    float attack; // attack increment per sample
    float release; // release decrement per sample
    bool note_pressed; // indicating  if a note is currently pressed

public:
    /**
     * @brief Default constructor.
     *
     * Initializes the envelope with zero attack and release times,
     * and sets the initial value to 0.
    */
    Envelope();


    /**
     * @brief Sets the attack time.
     *
     * Defines how fast the envelope reaches its maximum value (1.0) when a note is pressed.
     * A shorter time results in a quicker rise in amplitude.
     *
     * @param seconds Time in seconds for the attack phase.
    */
    void setAttack(float seconds);

    /**
      * @brief Sets the release time.
      *
      * Defines how fast the envelope drops to zero when a note is released.
      * A shorter time results in a quicker fall-off.
      *
      * @param seconds Time in seconds for the release phase.
    */
    void setRelease(float seconds);


    /**
      * @brief Triggers the start of the envelope (attack phase).
      *
      * Should be called when a note is pressed or activated.
    */
    void noteOn();
    /**
      * @brief Triggers the release phase of the envelope.
      *
      * Should be called when a note is released.
    */
    void noteOff();



    /**
     * @brief Processes the envelope for one sample.
     *
     * Updates the internal envelope value depending on whether a note is pressed or released.
     * The value increases toward 1.0 during attack and decreases toward 0.0 during release.
     *
     * @return The current envelope value between 0.0 and 1.0.
    */
    float process();


    /**
     * @brief Applies the envelope to an audio buffer.
     *
     * Multiplies each frame in a stereo audio buffer by the current envelope value,
     * effectively modulating the amplitude of the audio signal over time.
     *
     * @param buffer Pointer to the stereo audio buffer (L/R).
     * @param framesPerBuffer Number of audio frames to process.
    */
    void applyToBuffer(float *buffer, unsigned long framesPerBuffer);
};

#endif // ENVELOPE_H
