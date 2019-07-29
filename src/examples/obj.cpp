#include <iostream>
#include <cmath>
#include <basicgl/Manager.hpp>

using namespace BasicGL;
using namespace std;

void rotate(ElementsList elements, WindowPtr window, float ellasedTime)
{
    ElementPtr mario = window->find("mario");
    mario->rotate( 0, ellasedTime );
}

int main(int argc, char *argv[])
{
    Manager::Init(argc, argv);
    Manager::CreateWindow("Obj", MODE_3D);
    Manager::SetAnimationFunction(rotate);

    ElementPtr mario = Manager::CreateElement(OBJECT, "mario");
    mario->obj.load("../src/examples/data/mario.obj");
    mario->scaleTo(3.0f);


    Manager::Show();
    return 0;
}