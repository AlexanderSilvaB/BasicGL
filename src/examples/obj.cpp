#include <iostream>
#include <cmath>
#include <basicgl/Manager.hpp>

using namespace BasicGL;
using namespace std;

void mouseHandler(Mouse mouse, WindowPtr window)
{
    ElementPtr obj = window->elements[0];
    if(mouse.move)
    {
        obj->rotate( mouse.dy, mouse.dx );
    }
    if(mouse.scroll != 0)
        obj->scale(mouse.scroll * 0.02f);
}

int main(int argc, char *argv[])
{
    Manager::Init(argc, argv);
    Manager::CreateWindow("Obj", MODE_3D);
    Manager::SetBackground(13, 40, 53);
    Manager::SetMouseFunction(mouseHandler);

    ElementPtr cornell = Manager::CreateElement(OBJECT);
    cornell->obj.load("../src/examples/data/mario.obj");
    cornell->scaleTo(2.0f);

    Manager::Show();
    return 0;
}