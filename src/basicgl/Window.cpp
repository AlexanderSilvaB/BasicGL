#include "Window.hpp"
#include <cstring>

using namespace BasicGL;
using namespace std;

Window::Window()
{
    bg[0] = bg[1] = bg[2] = 0.0f;
    bg[3] = 1.0f;
    animationFunction = NULL;
    timeSinceBegin = 0;
    cartesian = true;
    memset(&lastTime, '\0', sizeof(lastTime));
    opened = true;
}

Window::~Window()
{

}
