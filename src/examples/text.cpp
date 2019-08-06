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

    ElementPtr textDefault = Manager::CreateElement(TEXT);
    textDefault->setText("Hello World - Default9x15", Default9x15);
    textDefault->textAlign(CenterX);
    textDefault->translate(0, 0.2f);
    textDefault->rgb(1.0f, 0.0f, 0.0f);

    ElementPtr textCustom = Manager::CreateElement(TEXT);
    textCustom->setText("Hello World - TrueType(arial.ttf)");
    textCustom->textAlign(CenterX);
    textCustom->font.load("../src/examples/data/arial.ttf");
    textCustom->translate(0, -0.2f);
    textCustom->rgb(0.0f, 1.0f, 0.0f);

    Manager::CreateElement(POINT)->moveTo(0, -0.2f)->rgb(1.0f);

    Manager::Show();
    Manager::Destroy();
    
    return 0;
}