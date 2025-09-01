#include "app.h"
#define DEG45 0.78539816339f
#define DEG90 1.5707963268f
#define DEG180 3.14159265359f
int main(int argc, char** argv) {
    Eng::Engine engine("Window!", {1920, 1080});

    engine.addObject({ 0.0f,  0.0, 0.8f}, {1.0f, 1.0f, 1.0f}, DEG45);
    engine.addObject({-0.5f, -0.5f, 0.2f}, {0.125f, 0.125f, 1.0f}, DEG45);
    engine.addObject({-0.5f, -0.5f, 0.4f}, {0.250f, 0.250f, 1.0f}, DEG45);
    engine.addObject({-0.5f, -0.5f, 0.6f}, {0.375f, 0.375f, 1.0f}, DEG45);

    engine.start();
    engine.loop();
    return 0;
}