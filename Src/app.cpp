#include "app.h"
int main(int argc, char** argv) {
    Eng::Engine engine("Window!", {1920, 1080});

    engine.addObject(
        {0.0f,  0.0, 2.5f},// position
        {0.25f, 0.25f, 0.25f},// scale
        {DEG45, DEG45, DEG90}// rotation
    );

    engine.start();
    engine.loop();
    return 0;
}