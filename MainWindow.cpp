#include "MainWindow.h"

#include <format>
#include <iostream>
#include <thread>
#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_sdlrenderer3.h"
#include <math.h>

#include "AudioGenerator.h"
# define PI  3.14159265358979323846
constexpr float FRAMERATE = 60.0f;
constexpr std::chrono::duration<double, std::milli> TARGET_FRAMETIME(1000.0 / FRAMERATE);


constexpr ImGuiKey keyMap[13] = {
    ImGuiKey_Q, ImGuiKey_Z, ImGuiKey_S, ImGuiKey_E, ImGuiKey_D, ImGuiKey_F,
    ImGuiKey_T, ImGuiKey_G, ImGuiKey_Y, ImGuiKey_H, ImGuiKey_U, ImGuiKey_J, ImGuiKey_K
};


constexpr float NOTE_FREQUENCIES[13] = {
    440.0f, // La
    466.16f, // La#
    493.88f, // Si
    523.25f, // Do
    554.37f, // Do#
    587.33f, // Ré
    622.25f, // Ré#
    659.26f, // Mi
    698.46f, // Fa
    739.99f, // Fa#
    783.99f, // Sol
    830.61f, // Sol#
    880.00f // La
};


void MainWindow::init() {
    // Setup SDL
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD)) {
        SDL_Log("Error: SDL_Init(): %s\n", SDL_GetError());
        return;
    }
    // Create window with SDL_Renderer graphics context
    Uint32 window_flags = SDL_WINDOW_HIDDEN | SDL_WINDOW_RESIZABLE;
    window = SDL_CreateWindow("", 800, 600, window_flags);
    if (nullptr == window) {
        SDL_Log("Error: SDL_CreateWindow(): %s\n", SDL_GetError());
        return;
    }
    renderer = SDL_CreateRenderer(window, nullptr);
    SDL_SetRenderVSync(renderer, 1);
    if (nullptr == renderer) {
        SDL_Log("Error: SDL_CreateRenderer(): %s\n", SDL_GetError());
        return;
    }
    SDL_SetWindowPosition(
        window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    SDL_ShowWindow(window);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void) io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

    // Setup DearImGui style
    ImGui::StyleColorsDark();
    ImGui::GetStyle().WindowRounding = 0.0f;

    // Setup Platform/Renderer backends
    ImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer3_Init(renderer);
}

void MainWindow::run() {
    const auto clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    bool done{false};
    while (!done) {
        auto frameStart = std::chrono::high_resolution_clock::now();

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            ImGui_ImplSDL3_ProcessEvent(&event);
            if (SDL_EVENT_QUIT == event.type)
                done = true;
            if ((SDL_EVENT_WINDOW_CLOSE_REQUESTED == event.type)
                && (SDL_GetWindowID(window) == event.window.windowID))
                done = true;
        }

        // Start the Dear ImGui frame
        ImGui_ImplSDLRenderer3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();


        // all the UI code description
        draw();

        // Rendering
        ImGui::Render();
        SDL_SetRenderDrawColorFloat(renderer,
                                    clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        SDL_RenderClear(renderer);
        ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer);
        SDL_RenderPresent(renderer);

        // Calculate time spent and sleep if needed
        auto frameEnd = std::chrono::high_resolution_clock::now();
        auto frameDuration = frameEnd - frameStart;
        if (frameDuration < TARGET_FRAMETIME) {
            std::this_thread::sleep_for(TARGET_FRAMETIME - frameDuration);
        }
    }

    // Cleanup
    ImGui_ImplSDLRenderer3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}


void MainWindow::handleKeyPress() const {
    for (int i = 0; i < 13; i++) {
        if (ImGui::IsKeyPressed(keyMap[i])) {
            playNote(i);
            break;
        }
    }
}

void MainWindow::handleKeyRelease() const {
    for (int i = 0; i < 13; i++) {
        if (ImGui::IsKeyReleased(keyMap[i])) {
            stopNote();
            break;
        }
    }
}


void MainWindow::playNote(unsigned int noteIndex) const {
    if (noteIndex >= 0 && noteIndex < 13) {
        Guard guard(shared.mtx);
        shared.params.activeFrequency = NOTE_FREQUENCIES[noteIndex];
        shared.params.noteOn = true;
    }
}

void MainWindow::stopNote() const {
    Guard guard(shared.mtx);
    shared.params.noteOn = false;
}

void MainWindow::draw() {
    ImGui::SetNextWindowPos(ImVec2(50, 40));
    ImGui::SetNextWindowSize(ImVec2(700, 500));
    ImGui::Begin("My Window", nullptr,
                 ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground |
                 ImGuiWindowFlags_NoTitleBar);

    handleKeyPress();
    handleKeyRelease();
    ImGui::Checkbox("OCS 1", &OCS1);

    //todo : maybe put this code with the switch in another function
    if (ImGui::BeginCombo("##combo", combo[current_item])) {
        for (int n = 0; n < song; n++) {
            const bool is_selected = (current_item == n);
            if (ImGui::Selectable(combo[n], is_selected)) {
                current_item = n;
            }
            if (is_selected) {
                ImGui::SetItemDefaultFocus();
            }
        }

        ImGui::EndCombo();
    }




    switch (current_item) {
        case 0: type = WaveType::SINE;
            break;
        case 1: type = WaveType::SQUARE;
            break;
        case 2: type = WaveType::SAW;
            break;
        default: type = WaveType::SINE;
    }

    // todo : Create a function for the sliders
    ImGui::SliderFloat("OSC1 Frequency Offset", &frequency, -5.0f, 5.0f);

    ImGui::Checkbox("OCS 2", &OCS2);

    ImGui::SliderFloat("Attack", &attack, 0.0f, 1.0f);

    ImGui::Spacing();
    ImGui::SliderFloat("Release", &release, 0.0f, 2.0f);

    ImGui::Spacing();
    ImGui::SliderFloat("Filter cutoff", &filter_cutoff, 20.0f, 20000.0f);

    ImGui::Spacing();
    ImGui::SliderFloat("Filter resonance", &filter_resonance, 20.0f, 20000.0f);

    ImGui::Spacing();
    ImGui::SliderFloat("Delay time", &delay_time, 0.0f, 1.0f);

    ImGui::Spacing();
    ImGui::SliderFloat("Delay mix", &delay_mix, 0.0f, 1.0f);



    for (unsigned int i = 0; i < 13; ++i) {
        if (ImGui::Button(std::to_string(i + 1).c_str(), ImVec2(40, 40))) {
        }

        if (ImGui::IsItemActive()) {
            playNote(i);
        } else if (ImGui::IsItemDeactivated()) {
            stopNote();
        }
        ImGui::SameLine();
    }

    // Lock the mutex to ensure thread-safe access to shared parameters
    // This prevents race conditions between the UI thread and the audio thread
    {
        Guard guard(shared.mtx);
        shared.params.osc1Enabled = OCS1;
        shared.params.osc2Enabled = OCS2;
        shared.params.osc1WaveType = type;
        shared.params.osc1FrequencyOffset = frequency;
        shared.params.attack = attack;
        shared.params.release = release;
        shared.params.filterCutoff = filter_cutoff;
        shared.params.filterResonance = filter_resonance;
        shared.params.delayTime = delay_time;
        shared.params.delayMix = delay_mix;
    }

    ImGui::End();
}
