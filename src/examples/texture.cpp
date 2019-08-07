#include <iostream>
#include <cmath>
#include <basicgl/Manager.hpp>

using namespace BasicGL;
using namespace std;

int main(int argc, char *argv[])
{
    Manager::Init(argc, argv);
    Manager::Create("Texture", MODE_2D);
    Manager::SetBackground(13, 40, 53);

    ElementPtr circle1 = Manager::CreateElement(CIRCLE);
    circle1->translate(-0.25f, 0.25f);
    circle1->texture.load("../src/examples/data/mario.bmp");
    circle1->applyColors = false;

    ElementPtr circle2 = Manager::CreateElement(CIRCLE);
    circle2->translate(0.25f, -0.25f);
    circle2->texture.load("../src/examples/data/mario.png");
    circle2->applyColors = false;

    Manager::Capture("texture.png");

    Manager::Show();
    Manager::Destroy();
    
    return 0;
}