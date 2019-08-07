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

float n = 0;
void rotate(ElementsList elements, WindowPtr window, float ellapsedTime)
{
    ElementPtr heart = elements.front();
    float scale = 0.5f + 0.5f*ABS(sin(M_PI * window->timeSinceBegin));
    heart->scaleTo( scale );
}

int main(int argc, char *argv[])
{
    Manager::Init(argc, argv);

    Manager::Create("Heart", MODE_2D);
    Manager::SetBackground(13, 40, 53);
    Manager::SetAnimationFunction(rotate);

    ElementPtr heart = Manager::CreateElement(POLYGON);
    heart->reshape(360);
    heart->glow();
    
    float size = 0.03f;
    for(int i = 0; i < 360; i++)
    {
        float t = (i / 360.0f) * 2 * M_PI;
        float x = size * (16*sin(t)*sin(t)*sin(t));
        float y = size * (13*cos(t) - 5*cos(2*t) - 2*cos(3*t) - cos(4*t));
        heart->point(x, y, 0, i);
    }
    
    Manager::Show();
    Manager::Destroy();
    
    return 0;
}