#include <iostream>
#include <cmath>
#include <basicgl/Manager.hpp>

using namespace BasicGL;
using namespace std;

Elements style = SEQUENCE;
ElementPtr current;

void mouseHandler(Mouse mouse, WindowPtr window)
{
    if(mouse.pressed && current == NULL)
    {
        current = Manager::CreateElement(style);
        current->reshape(0);
    }
    else if(mouse.released || mouse.leave)
    {
        current = NULL;
    }
    else if(mouse.move && current != NULL)
    {
        int i = current->newPoint(mouse.x, mouse.y, 0);
        current->glow();
    }
}

void keyboardHandler(Keyboard keyboard, WindowPtr window)
{
    if(style == POINT)
        style = LINE;
    else if(style == LINE)
        style = SEQUENCE;
    else if(style == SEQUENCE)
        style = LOOP;
    else if(style == LOOP)
        style = POLYGON;  
    else if(style == POLYGON)
        style = POINT; 
}

int main(int argc, char *argv[])
{
    Manager::Init(argc, argv);
    Manager::CreateWindow("Paint", MODE_2D);
    Manager::SetMouseFunction(mouseHandler);
    Manager::SetKeyboardFunction(keyboardHandler);

    Manager::CreateElement(TEXT)->moveTo(0, -0.9)->textAlign(CenterX | CenterY)->setText("Press any key to change style");

    Manager::Show();
    Manager::Destroy();
    
    return 0;
}