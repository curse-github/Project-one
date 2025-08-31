#include "app.h"

int main(int argc, char** argv) {
    Eng::Engine engine("Window!", {1920, 1080});

    engine.addModel({
        {{-0.5,  0.5}, {1.0f, 0.0f, 0.0f}},
        {{-0.5, -0.5}, {0.0f, 1.0f, 0.0f}},
        {{ 0.5, -0.5}, {0.0f, 0.0f, 1.0f}}
    });
    engine.addModel({
        {{ 0.5, -0.5}, {0.0f, 0.0f, 1.0f}},
        {{ 0.5,  0.5}, {1.0f, 1.0f, 1.0f}},
        {{-0.5,  0.5}, {1.0f, 0.0f, 0.0f}}
    });

    engine.start();
    engine.loop();
    return 0;
}