#include <basicgl/Manager.hpp>
#include <cmath>
#include <iostream>

using namespace std;
using namespace BasicGL;

float ABS(float x) 
{
    if(x >= 0)
        return x;
    return -x;
}

void rotate(ElementsList elements, WindowPtr window, float ellasedTime)
{
    ElementPtr heart = elements.front();
    float scale = 0.5f + 0.5f*ABS(sin(2 * M_PI * window->timeSinceBegin));
    heart->scaleTo( scale );
}

int main(int argc, char *argv[])
{
    Manager::Init(argc, argv);

    Manager::CreateWindow("Heart", MODE_2D);
    Manager::SetBackground(13, 40, 53);
    Manager::SetAnimationFunction(rotate);

    ElementPtr heart = Manager::CreateElement(POLYGON);
    heart->reshape(360);
    heart->glow();
    
    for(int i = 0; i < 360; i++)
    {
        float t = (i / 360.0f) * 2 * M_PI;
        float x = 0.03f * (16*sin(t)*sin(t)*sin(t));
        float y = -0.03f * (13*cos(t) - 5*cos(2*t) - 2*cos(3*t) - cos(4*t));
        heart->point(x, -y, 0, i);
    }
    
    Manager::Show();
    Manager::Destroy();
    
    return 0;
}