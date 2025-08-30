#include "app.h"

int main(int argc, char** argv) {
    Eng::Engine engine("Window!", {1920, 1080});
    engine.start();
    engine.loop();
    return 0;
}