#include "Window.hpp"
#include <cstring>
#include <GL/glut.h>
#include <iostream>

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
    fullscren = false;
    windowedX = 50;
    windowedY = 50;
    windowedWidth = 320;
    windowedHeight = 240;
    lastMouseX = lastMouseY = 0;
    saveFileName = "";
}

Window::~Window()
{

}

void Window::assocElement(ElementPtr element)
{
    assocElements.push_back(element);
}

void Window::Destroy()
{
    for(int i = 0; i < assocElements.size(); i++)
    {
        ElementPtr el = assocElements[i];
        delete el;
    }
    assocElements.clear();
}

bool Window::IsFullscreen()
{
    return fullscren;
}

void Window::SetFullscreen(bool enabled)
{
    if(enabled != fullscren)
    {
        fullscren = enabled;
        if(enabled)
        {
            windowedX = glutGet(GLUT_WINDOW_X);
            windowedY = glutGet(GLUT_WINDOW_Y);
            windowedWidth = glutGet(GLUT_WINDOW_WIDTH);
            windowedHeight = glutGet(GLUT_WINDOW_HEIGHT);
            glutFullScreen();
        }
        else
        {
            glutReshapeWindow(windowedWidth, windowedHeight);
            glutPositionWindow(windowedX, windowedY);
        }
    }
}

void Window::ToggleFullscreen()
{
    SetFullscreen(!fullscren);
}

float Window::ZPlane()
{
    return camera.position[2] * 0.5f;
}

ElementPtr Window::find(const string& name)
{
    ElementPtr ret = NULL;
    for(int i = 0; i < elements.size(); i++)
    {
        ret = elements[i]->find(name);
        if(ret != NULL)
            break;
    }
    return ret;
}

ElementPtr Window::get(int index)
{
    if(elements.size() > index)
        return elements[index];
    return NULL;
}