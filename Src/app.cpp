#include "app.h"

int main(int argc, char** argv) {
    Eng::Engine engine("Vulkan Window Name!", {1920, 1080});
    engine.start();
    engine.loop();
    return 0;
}