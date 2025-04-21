#include "MainWindow.h"
#include "AudioGenerator.h"

int main() {
    SharedSynthParameters shared;

    MainWindow ui(shared);
    AudioGenerator ag(shared);
    ag.init();
    ui.init();
    ui.run();

    return 0;
}
