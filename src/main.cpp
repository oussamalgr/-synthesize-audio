#include "audio/AudioGenerator.h"
#include "shared/SynthParams.h"
#include "ui/MainWindow.h"

int main() {
    SharedSynthParameters shared;

    MainWindow ui(shared);
    AudioGenerator ag(shared);
    ag.init();
    ui.init();
    ui.run();

    return 0;
}
