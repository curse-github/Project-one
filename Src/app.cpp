#include "app.h"
int main(int argc, char** argv) {
    Eng::Engine engine("Window!", {1920, 1080});
    engine.addObject(// window thing
        {1.5f, 0.0f, 0.0f},// position
        {0.5f, 0.5f, 0.5f},// scale
        {DEG90, DEG45, 0.0f}// rotation
    , "Resources/Models/Quad.obj", "Resources/Models/suzanne.mtl", "Transparent");

    engine.addObject(// window thing
        {-1.5f, 0.0f, 0.0f},// position
        {0.5f, 0.5f, 0.5f},// scale
        {DEG90, -DEG45, 0.0f}// rotation
    , "Resources/Models/Quad.obj", "Resources/Models/suzanne.mtl", "Plate");
    engine.addObject(// monkey1
        {-1.75f, -0.5f, 2.25f},// position
        {1.0f, 0.75f, 0.75f},// scale
        {0.0f, -DEG45, 0.0f}// rotation
    , "Resources/Models/suzanne.obj", "Resources/Models/suzanne.mtl", "White");
    engine.addObject(// monkey2
        {0.0f, -0.5f, 2.75f},// position
        {0.75f, 1.0f, 0.75f},// scale
        {0.0f, 0.0f, 0.0f}// rotation
    , "Resources/Models/suzanne.obj", "Resources/Models/suzanne.mtl", "White");
    engine.addObject(// monkey3
        {1.75f, -0.5f, 2.25f},// position
        {1.0f, 0.75f, 0.75f},// scale
        {0.0f, DEG45, 0.0f}// rotation
    , "Resources/Models/suzanne.obj", "Resources/Models/suzanne.mtl", "White");

    engine.addObject(// floor
        {0.0f, 0.5f, 1.5f},// position
        {3.0f, 3.0f, 3.0f},// scale
        {0.0f, 0.0f, 0.0f}// rotation
    , "Resources/Models/Quad.obj", "Resources/Models/suzanne.mtl", "Floor");
    
    const vec3 base(0.0f, -0.5f, 0.0f);
    const vec3 mult(1.5f, 0.1333f, -1.5f);
    const float speedXZ = 1.25f;// revolutions per second
    const float speedY = 2.0f;// revolutions per second
    engine.addLight(
        {base.x+mult.x*cos(DEG360/7*0), base.y, base.z+mult.z*sin(DEG360/7*0)},// position
        0.0666f,// size
        {1.0f, 0.0f, 0.0f},// color = red
        0.666f// intensity
    );
    engine.addLight(
        {base.x+mult.x*cos(DEG360/7*1), base.y, base.z+mult.z*sin(DEG360/7*1)},// position
        0.0666f,// size
        {1.0f, 0.5f, 0.0f},// color = orange
        0.666f// intensity
    );
    engine.addLight(
        {base.x+mult.x*cos(DEG360/7*2), base.y, base.z+mult.z*sin(DEG360/7*2)},// position
        0.0666f,// size
        {1.0f, 1.0f, 0.0f},// color = yellow
        0.5f// intensity
    );
    engine.addLight(
        {base.x+mult.x*cos(DEG360/7*3), base.y, base.z+mult.z*sin(DEG360/7*3)},// position
        0.0666f,// size
        {0.0f, 1.0f, 0.0f},// color = green
        0.666f// intensity
    );
    engine.addLight(
        {base.x+mult.x*cos(DEG360/7*4), base.y, base.z+mult.z*sin(DEG360/7*4)},// position
        0.0666f,// size
        {0.0f, 0.0f, 1.0f},// color = blue
        1.0f// intensity
    );
    engine.addLight(
        {base.x+mult.x*cos(DEG360/7*5), base.y, base.z+mult.z*sin(DEG360/7*5)},// position
        0.0666f,// size
        {0.6f, 0.0f, 1.0f},// color = pink
        0.5f// intensity
    );
    engine.addLight(
        {base.x+mult.x*cos(DEG360/7*6), base.y, base.z+mult.z*sin(DEG360/7*6)},// position
        0.0666f,// size
        {1.0f, 0.0f, 1.0f},// color = pink
        0.666f// intensity
    );

    engine.start();
    engine.run();
    return 0;
}