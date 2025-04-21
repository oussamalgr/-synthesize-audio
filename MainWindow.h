#ifndef TESTINSTRUCT_MAINWINDOW_H
#define TESTINSTRUCT_MAINWINDOW_H
#include "SynthParams.h"
#include <SDL3/SDL.h>

class MainWindow {
public :

    void init();
    MainWindow(SharedSynthParameters& sharedParams) : shared(sharedParams) {}
    void run();


private:
    // Some attributes will be modified or deleted later
    void playNote(unsigned int noteIndex) const;
    void stopNote() const;
    const char *const combo[3] = {"SINE", "SQUARE", "SAW"};
    int current_item{0};
    constexpr static int NBTN{14};
    constexpr static int song{3};
    float delay_time{0.1f};
    float delay_mix{0.0f};
    float attack{0.0f};
    float filter_cutoff{20.0f};
    float filter_resonance{0.0f};
    float release{0.0f};
    float frequency{0.0f};
    bool OCS1= false;
    bool OCS2 = false;
    WaveType type;
    SharedSynthParameters& shared;

    void handleKeyPress() const;
    void handleKeyRelease() const;


    void draw();
    SDL_Window *window{nullptr};
    SDL_Renderer *renderer{nullptr};
};

#endif //TESTINSTRUCT_MAINWINDOW_H
