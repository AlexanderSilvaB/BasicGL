#include <basicgl/Manager.hpp>
#include <cmath>

using namespace BasicGL;

void rotate(ElementsList elements, WindowPtr window, float ellasedTime)
{
    ElementPtr teapot = window->get(0);
    LightSourcePtr light = window->getLightSource(0);

    light->moveTo(0.0f, 0.0f, 1.0f);
    light->rotateAroundY( teapot, M_PI * ellasedTime );
    light->update();
}

void keyboard(Keyboard keyboard, WindowPtr window)
{
    window->getLightSource(0)->glow();
}

int main(int argc, char *argv[])
{
    Manager::Init(argc, argv);

    Manager::Create("Light", MODE_3D);
	Manager::SetAnimationFunction(rotate);
    Manager::SetKeyboardFunction(keyboard);

    Manager::CreateElement(TEAPOT, "teapot");

    Manager::Show();
    Manager::Destroy();

    return 0;
}