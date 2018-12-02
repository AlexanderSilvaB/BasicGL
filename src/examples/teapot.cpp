#include <basicgl/BasicGL.h>
#include <cmath>

void rotate(BasicGLElements elements, BasicGLWindow *window, float ellasedTime)
{
    BasicGLElement *teapot = elements[0];
    teapot->rotate( 0, ellasedTime );
}

int main(int argc, char *argv[])
{
    BasicGL::Init(argc, argv);

    BasicGL::CreateWindow("Teapot", MODE_3D);
	BasicGL::SetAnimationFunction(rotate);

    BasicGL::CreateElement(ELEMENT_TEAPOT);

    BasicGL::Run();
    return 0;
}