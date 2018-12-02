#include <basicgl/BasicGL.h>
#include <cmath>

void rotate(BasicGLElements elements, BasicGLWindow *window, float ellasedTime)
{
    BasicGLElement *triangle = elements[0];
    triangle->rotate( 0, 0.01f );
}

int main(int argc, char *argv[])
{
    BasicGL::Init(argc, argv);

    BasicGL::CreateWindow("Triangle", MODE_3D);
    BasicGL::SetBackground(13, 40, 53);
    BasicGL::SetAnimationFunction(rotate);

    BasicGL::CreateElement(ELEMENT_TRIANGLE);

    BasicGL::Run();
    return 0;
}