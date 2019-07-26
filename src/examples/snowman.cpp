#include <basicgl/Manager.hpp>
#include <cmath>

using namespace BasicGL;

void rotate(ElementsList elements, WindowPtr window, float ellasedTime)
{
    ElementPtr snowman = elements[0];
    snowman->rotate( 0, 2 * M_PI * ellasedTime );
}

int main(int argc, char *argv[])
{
    Manager::Init(argc, argv);

    Manager::CreateWindow("SnowMan", MODE_3D);
    Manager::SetAnimationFunction(rotate);

    ElementPtr snowman = Manager::CreateElement(CONTAINER);
    snowman->create(SPHERE)->translate(0, -0.8f);;
    snowman->create(SPHERE)->scaleTo(0.3f)->translate(0, 0.45f);
    snowman->create(SPHERE)->scaleTo(0.06f)->translate(-0.1f, 0.5f, 0.3f)->rgb(0.0f);
    snowman->create(SPHERE)->scaleTo(0.06f)->translate( 0.1f, 0.5f, 0.3f)->rgb(0.0f);
    snowman->create(CONE)->scaleTo(0.1f, 0.1f, 0.2f)->translate( 0.0f, 0.4f, 0.3f)->rgb((unsigned char)204, 78, 0);

    Manager::Show();
    return 0;
}