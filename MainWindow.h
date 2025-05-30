#ifndef TESTINSTRUCT_MAINWINDOW_H
#define TESTINSTRUCT_MAINWINDOW_H
#include "SynthParams.h"
#include <SDL3/SDL.h>

class MainWindow {
public :

    void init();
    MainWindow(SharedSynthParameters& sharedParams);
    void run();


private:
    // Some attributes will be modified or deleted later
    void playNote(unsigned int noteIndex) const;
    void stopNote() const;
    const char *const combo[3] = {"SINE", "SQUARE", "SAW"};
    int current_item;
    constexpr static int song{3};
    float delay_time;
    float delay_mix;
    float attack;
    float filter_cutoff;
    float filter_resonance;
    float release;
    float frequency;
    bool OSC1;
    bool OSC2;
    int number_of_notes;
    WaveType type;
    SharedSynthParameters& shared;

    void handleKeyPress() const;
    void handleKeyRelease() const;


    void draw();
    SDL_Window *window{nullptr};
    SDL_Renderer *renderer{nullptr};
};

#endif //TESTINSTRUCT_MAINWINDOW_H
