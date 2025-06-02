#ifndef TESTINSTRUCT_MAINWINDOW_H
#define TESTINSTRUCT_MAINWINDOW_H
#include "../shared/SynthParams.h"
#include "../audio/Oscillator.h"
#include <SDL3/SDL.h>

class MainWindow {
public :

    void init();

    /**
     * @brief Constructs the MainWindow and initializes synthesis shared parameters.
     *
     * Initializes all synth-related values (delay, envelope, filter, oscillators, etc.)
     * and links the UI to shared parameters used by the audio engine.
     *
     * @param sharedParams Reference to shared synthesizer parameters.
    */
    MainWindow(SharedSynthParameters& sharedParams);
    void run();


private:

    const char *const combo[3] = {"SINE", "SQUARE", "SAW"}; // List of supported waveform types
    int current_type; //  Index of the currently selected wave type


    // Synthesizer parameters
    float delay_time;
    float delay_mix;
    float attack;
    float filter_cutoff;
    float filter_resonance;
    float release;
    float frequency;
    bool OSC1;
    bool OSC2;


    WaveType type; // Enum  => Saw,square,sin

    SharedSynthParameters& shared; // shared parameters between audio and ui threads

    /**
     * @brief Handles key press events and triggers a note if a mapped key is pressed.
     *
     * Iterates over the key map and checks if any assigned key is currently pressed.
     * If so, triggers the corresponding note.
    */
    void handleKeyPress() const;

    /**
     * @brief Handles key release events and stops the note if a mapped key is released.
     *
     * Iterates over the key map and checks if any assigned key was released.
     * If so, stops the currently playing note.
    */
    void handleKeyRelease() const;

    /**
     * @brief Triggers note playback based on the given note index.
     *
     * Sets the active frequency from the NOTE_FREQUENCIES array and enables noteOn.
     * Thread-safe via a mutex guard.
     *
     * @param noteIndex Index of the note to play (0 to number_of_notes - 1).
    */
    void playNote(unsigned int noteIndex) const;

    /**
     * @brief Stops the currently active note.
     *
     * Sets shared.noteOn to false. Thread-safe via a mutex guard.
    */
    void stopNote() const;

    /**
     * @brief Sets the waveform type based on the current UI selection index.
     *
     * Maps current_item to the corresponding WaveType (SINE, SQUARE, SAW).
     * Defaults to SINE if the selection index is invalid.
    */
    void setWaveTypeFromSelection();

    /**
     * Displays UI controls for synth parameters (oscillator, envelope, filter, delay).
    */
    void drawSynthParameters();

    void draw();
    SDL_Window *window{nullptr};
    SDL_Renderer *renderer{nullptr};
};

#endif //TESTINSTRUCT_MAINWINDOW_H
