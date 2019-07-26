#include <basicgl/Manager.hpp>
#include <cmath>

using namespace BasicGL;

void rotate(ElementsList elements, WindowPtr window, float ellasedTime)
{
    ElementPtr teapot = elements[0];
    teapot->rotate( 0, 2 * M_PI * ellasedTime );
}

int main(int argc, char *argv[])
{
    Manager::Init(argc, argv);

    Manager::CreateWindow("Teapot", MODE_3D);
	Manager::SetAnimationFunction(rotate);

    Manager::CreateElement(TEAPOT);

    Manager::Show();
    return 0;
}