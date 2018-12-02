#include <basicgl/BasicGL.h>
#include <cmath>
#include <iostream>

using namespace std;

void rotate(BasicGLElements elements, BasicGLWindow *window, float ellasedTime)
{
    BasicGLElement *heart = elements[0];
    float scale = 0.05f + ABS(sin(2 * M_PI * window->timeSinceBegin));
    heart->scaleTo( scale );
}

int main(int argc, char *argv[])
{
    BasicGL::Init(argc, argv);

    BasicGL::CreateWindow("Heart", MODE_2D);
    BasicGL::SetBackground(13, 40, 53);
    BasicGL::SetAnimationFunction(rotate);

    BasicGLElement *heart = BasicGL::CreateElement(ELEMENT_POLYGON);
    heart->reshape(360);
    for(int i = 0; i < 360; i++)
    {
        float t = (i / 360.0f) * 2 * M_PI;
        float x = 0.03f * (16*sin(t)*sin(t)*sin(t));
        float y = 0.03f * (13*cos(t) - 5*cos(2*t) - 2*cos(3*t) - cos(4*t));
        heart->points[i].moveTo(x, y, 0);
        heart->points[i].rgb(1.0f);
    }

    BasicGL::Run();
    return 0;
}