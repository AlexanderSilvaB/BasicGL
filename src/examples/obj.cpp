#include <iostream>
#include <cmath>
#include <basicgl/Manager.hpp>

using namespace BasicGL;
using namespace std;

void rotate(ElementsList elements, WindowPtr window, float ellasedTime)
{
    ElementPtr earth = window->find("earth");
    earth->rotate( 0, 0.5f * ellasedTime );
}

int main(int argc, char *argv[])
{
    Manager::Init(argc, argv);
    Manager::CreateWindow("Obj", MODE_3D);
    Manager::SetAnimationFunction(rotate);

    ElementPtr earth = Manager::CreateElement(OBJECT, "earth");
    earth->obj.load("../src/examples/data/earth_ball.obj");
    earth->obj.flipYZ = false;
    earth->scaleTo(3.0f);

    Manager::Show();
    return 0;
}