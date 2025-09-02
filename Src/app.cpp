#include "app.h"
int main(int argc, char** argv) {
    Eng::Engine engine("Window!", {1920, 1080});

    engine.addObject(
        {0.75f,  0.0, 2.0f},// position
        {2.0f, 2.0f, 2.0f},// scale
        {0, 0, -DEG45}// rotation
    , 0);
    engine.addObject(
        {-0.75f,  0.0, 2.0f},// position
        {2.0f, 2.0f, 2.0f},// scale
        {0, 0, DEG45}// rotation
    , 1);

    engine.start();
    engine.loop();
    return 0;
}