#include <iostream>
#include <cmath>
#include <basicgl/Manager.hpp>

using namespace BasicGL;
using namespace std;

int main(int argc, char *argv[])
{
    Manager::Init(argc, argv);
    Manager::CreateWindow("Text", MODE_2D);
    Manager::SetBackground(13, 40, 53);

    ElementPtr text = Manager::CreateElement(TEXT);
    text->setText("Hello World", Default9x15);
    text->textAlign(CenterX | CenterY);

    Manager::Show();
    Manager::Destroy();
    
    return 0;
}