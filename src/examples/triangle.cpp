#include <basicgl/BasicGL.h>
#include <cmath>

int main(int argc, char *argv[])
{
    BasicGL::Init(argc, argv);

    BasicGL::CreateWindow("Triangle", MODE_2D);
    BasicGL::SetBackground(13, 40, 53);

    BasicGL::CreateElement(ELEMENT_TRIANGLE);

    BasicGL::Run();
    return 0;
}