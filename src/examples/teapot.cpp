#include <basicgl/Manager.hpp>
#include <cmath>

using namespace BasicGL;

void rotate(ElementsList elements, WindowPtr window, float ellasedTime)
{
    ElementPtr teapot = window->get(0);
    teapot->rotate( 0, 2 * M_PI * ellasedTime, 0 );
}

void keyboard(Keyboard keyboard, WindowPtr window)
{
    if(keyboard.key == KEY_F11)
    {
        window->ToggleFullscreen();
    }
    else if(keyboard.key == KEY_LEFT || keyboard.key == KEY_RIGHT)
    {
        window->get(0)->glow();
    }
}

int main(int argc, char *argv[])
{
    Manager::Init(argc, argv);

    Manager::Create("Teapot", MODE_3D);
	Manager::SetAnimationFunction(rotate);
    Manager::SetKeyboardFunction(keyboard);

    Manager::CreateElement(TEAPOT, "teapot");

    Manager::Show();
    Manager::Destroy();

    return 0;
}