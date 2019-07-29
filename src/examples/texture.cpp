#include <iostream>
#include <cmath>
#include <basicgl/Manager.hpp>

using namespace BasicGL;
using namespace std;

int main(int argc, char *argv[])
{
    Manager::Init(argc, argv);
    Manager::CreateWindow("Texture", MODE_2D);
    Manager::SetBackground(13, 40, 53);

    ElementPtr rect = Manager::CreateElement(CIRCLE);
    rect->texture.load("../src/examples/data/mario.bmp");
    rect->applyColors = false;

    Manager::Save("texture.png");

    Manager::Show();
    Manager::Destroy();
    
    return 0;
}