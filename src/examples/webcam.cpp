#include <basicgl/Manager.hpp>
#include <iostream>
#include <cmath>

using namespace BasicGL;
using namespace std;

int main(int argc, char *argv[])
{
    Manager::Init(argc, argv);
    Manager::Create("Webcam Preview", MODE_2D, 30);
    Manager::SetBackground(13, 40, 53);

    cout << "Webcam support: " << Texture::supportsCapture() << endl;

    ElementPtr rect = Manager::CreateElement(RECTANGLE);
	rect->scaleTo(1.8f);
    rect->texture.autoCapture = true;
    rect->applyColors = true;

    bool opened = rect->texture.setCaptureDevice(0);
    if(!opened)
        cout << "Failed to open webcam 0" << endl;


    Manager::Show();
    Manager::Destroy();
    
    return 0;
}