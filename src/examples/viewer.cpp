#include <iostream>
#include <cmath>
#include <basicgl/Manager.hpp>
#include <cstdlib>

using namespace BasicGL;
using namespace std;

bool translate = false;
void mouseHandler(Mouse mouse, WindowPtr window)
{
    ElementPtr object = window->find("object");
    if(mouse.move)
    {
        if(translate)
            object->translate( mouse.dx, -mouse.dy );
        else
            object->rotate( mouse.dy, mouse.dx );
    }
    if(mouse.scroll != 0)
        object->scale(mouse.scroll * 0.02f);
}

void keyboardHandler(Keyboard keyboard, WindowPtr window)
{
    if(keyboard.key == 'T' || keyboard.key == 't')
        translate = !translate;
    else if(keyboard.key == 'F' || keyboard.key == 'f')
    {
        ElementPtr object = window->find("object");
        object->obj.flipYZ = !object->obj.flipYZ;
    }
    else if(keyboard.key == KEY_ESC)
    {
        ElementPtr object = window->find("object");
        object->moveTo(0, 0, 0);
        object->scaleTo(3.0f);
        object->rotateTo(0, 0, 0);
    }
}

int main(int argc, char *argv[])
{
    string objFile = "../src/examples/data/mario.obj";
    if(argc > 1)
        objFile = string(argv[1]);

    cout << "Viewing: " << objFile << endl;

    Manager::Init(argc, argv);
    Manager::Create("Viewer", MODE_3D);
    Manager::SetMouseFunction(mouseHandler);
    Manager::SetKeyboardFunction(keyboardHandler);

    WindowPtr window = Manager::CurrentWindow();
    float zplane = window->ZPlane();

    Manager::CreateElement(TEXT)->setText("Flip YZ: F")->textAlign(Align_Left | Align_Bottom)->moveTo(-1.0f, -0.9f, zplane);
    Manager::CreateElement(TEXT)->setText("Enable/Disable translate: T")->textAlign(Align_Left | Align_Bottom)->moveTo(-1.0f, -1.0f, zplane);

    ElementPtr object = Manager::CreateElement(OBJECT, "object");
    object->scaleTo(3.0f);

    if(!object->obj.load(objFile))
    {
        cerr << "Could not open the file" << endl;
        exit(0);
    }

    Manager::Show();
    Manager::Destroy();
    
    return 0;
}