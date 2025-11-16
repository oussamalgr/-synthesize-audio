# Software Synthesizer

A real-time audio synthesizer built in C++ with a graphical user interface. This project implements a polyphonic synthesizer with dual oscillators, envelope shaping, filtering, and delay effects.

## Features

### Oscillators
- **Dual Oscillator Architecture**: Two independent oscillators (OSC1 and OSC2)
- **Waveform Types**: Sine, Square, and Sawtooth waves
- **Frequency Control**: Adjustable frequency offset for OSC1 (±5 Hz)
- **Independent Enable/Disable**: Toggle each oscillator on or off

### Audio Effects
- **ADSR Envelope**: Attack and Release controls for amplitude shaping
- **Low-Pass Filter**: Biquad filter with adjustable cutoff (20 Hz - 20 kHz) and resonance
- **Stereo Delay**: Echo effect with adjustable delay time (0.1 - 2 seconds) and wet/dry mix

### User Interface
- **ImGui-based GUI**: Clean, responsive interface built with Dear ImGui and SDL3
- **12-Note Keyboard**: Play notes using computer keyboard or on-screen buttons
- **Real-Time Parameter Control**: Adjust all synthesis parameters with sliders

## Technical Architecture

### Audio Processing
- **Sample Rate**: 44.1 kHz
- **Buffer Size**: 256 frames per buffer
- **Audio Library**: PortAudio for cross-platform audio I/O
- **Stereo Processing**: Full stereo signal path

### Thread Safety
- **Dual-Thread Architecture**: Separate UI and audio threads
- **Mutex Protection**: Thread-safe parameter updates using `SharedSynthParameters`
- **Lock-Free Reads**: Audio thread copies parameters once per buffer

## Keyboard Mapping

The synthesizer maps 12 notes to keyboard keys:

| Key | Note | Frequency (Hz) |
|-----|------|----------------|
| Q   | 0    | 220.00         |
| Z   | 1    | 233.08         |
| S   | 2    | 246.94         |
| E   | 3    | 261.63         |
| D   | 4    | 277.18         |
| F   | 5    | 293.66         |
| T   | 6    | 311.13         |
| G   | 7    | 329.63         |
| Y   | 8    | 349.23         |
| H   | 9    | 369.99         |
| U   | 10   | 392.00         |
| J   | 11   | 415.30         |

## Project Structure

```
├── audio/
│   ├── AudioGenerator.cpp/h    # Main audio processing engine
│   ├── Oscillator.cpp/h        # Waveform generation
│   ├── Envelope.cpp/h          # ADSR envelope processor
│   ├── Filter.cpp/h            # Biquad low-pass filter
│   ├── Delay.cpp/h             # Stereo delay effect
│   └── AudioConstants.h        # Audio system constants
├── ui/
│   ├── MainWindow.cpp/h        # GUI and user input handling
└── shared/
    └── SynthParams.cpp/h       # Thread-safe parameter sharing
```

## Dependencies

- **PortAudio**: Cross-platform audio I/O library
- **SDL3**: Window management and rendering
- **Dear ImGui**: Immediate mode GUI framework
- **C++20**: Modern C++ standard

## Building

### Prerequisites
```bash
# Install dependencies (example for Ubuntu/Debian)
sudo apt-get install libportaudio2 libportaudio-dev
sudo apt-get install libsdl3-dev

# Or using vcpkg
vcpkg install portaudio sdl3 imgui[sdl3-renderer-binding]
```

### Compilation
```bash
mkdir build
cd build
cmake ..
make
```

## Usage

1. Launch the application
2. Enable oscillators using the "OSC 1" and "OSC 2" checkboxes
3. Select waveform type for OSC1 from the dropdown menu
4. Adjust synthesis parameters using the sliders
5. Play notes using:
   - Keyboard keys (Q, Z, S, E, D, F, T, G, Y, H, U, J)
   - On-screen numbered buttons (1-12)

## Audio Signal Flow

```
OSC1 (Sine/Square/Saw) ─┐
                        ├─> Mix ─> Envelope ─> Filter ─> Delay ─> Output
OSC2 (Sawtooth)        ─┘
```

## Implementation Highlights

### Thread-Safe Parameter Updates
```cpp
// UI Thread
{
    Guard guard(shared.mtx);
    shared.filterCutoff = filter_cutoff;
    shared.attack = attack;
}

// Audio Thread
SynthPOD localParams = shared;  // Thread-safe copy
```

### Circular Buffer Delay
The delay effect uses a circular buffer with separate read/write indices, supporting delay times up to 2 seconds with adjustable wet/dry mix.

### Biquad Filter
Implements a standard biquad low-pass filter with resonance control, processing stereo audio with independent state variables for each channel.

## Author

Oussamalgr

- Dear ImGui by Omar Cornut
- SDL development team
