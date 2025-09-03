#include "app.h"
int main(int argc, char** argv) {
    Eng::Engine engine("Window!", {1920, 1080});

    engine.addObject(// vase 1
        {0.75f, 0.0f, 0.0f},// position
        {2.0f, 2.0f, 2.0f},// scale
        {0.0f, 0.0f, 0.0f}// rotation
    , 0);
    engine.addObject(// vase 2
        {-0.75f, 0.0f, 0.0f},// position
        {2.0f, 2.0f, 2.0f},// scale
        {0.0f, 0.0f, 0.0f}// rotation
    , 1);
    engine.addObject(// monkey
        {0.0f, -0.5f, 2.5f},// position
        {0.75f, 0.75f, 0.75f},// scale
        {0.0f, 0.0f, 0.0f}// rotation
    , 2);
    engine.addObject(// floor
        {0.0f, 0.5f, 1.5f},// position
        {3.0f, 3.0f, 3.0f},// scale
        {0.0f, 0.0f, 0.0f}// rotation
    , 3);

    engine.start();
    engine.run();
    return 0;
}