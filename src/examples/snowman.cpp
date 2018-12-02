#include <basicgl/BasicGL.h>
#include <cmath>

void rotate(BasicGLElements elements, BasicGLWindow *window, float ellasedTime)
{
    BasicGLElement *snowman = elements[0];
    snowman->rotate( 0, ellasedTime );
}

int main(int argc, char *argv[])
{
    BasicGL::Init(argc, argv);

    BasicGL::CreateWindow("SnowMan", MODE_3D);
    BasicGL::SetAnimationFunction(rotate);

    BasicGLElement *snowman = BasicGL::CreateElement(ELEMENT_CONTAINER);
    snowman->CreateElement(ELEMENT_SPHERE)->translate(0, -0.8f);;
    snowman->CreateElement(ELEMENT_SPHERE)->scaleTo(0.3f)->translate(0, 0.45f);
    snowman->CreateElement(ELEMENT_SPHERE)->scaleTo(0.06f)->translate(-0.1f, 0.5f, 0.3f)->rgb(0.0f);
    snowman->CreateElement(ELEMENT_SPHERE)->scaleTo(0.06f)->translate( 0.1f, 0.5f, 0.3f)->rgb(0.0f);
    snowman->CreateElement(ELEMENT_CONE)->scaleTo(0.1f, 0.1f, 0.2f)->translate( 0.0f, 0.4f, 0.3f)->rgb((unsigned char)204, 78, 0);

    BasicGL::Run();
    return 0;
}