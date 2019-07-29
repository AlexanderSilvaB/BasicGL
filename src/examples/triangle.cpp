#include <iostream>
#include <cmath>
#include <basicgl/Manager.hpp>

using namespace BasicGL;
using namespace std;

int main(int argc, char *argv[])
{
    Manager::Init(argc, argv);
    Manager::CreateWindow("Triangle", MODE_2D);
    Manager::SetBackground(13, 40, 53);
    Manager::SetCartesian(false);

    Manager::CreateElement(TRIANGLE);

    Manager::Show();
    Manager::Destroy();
    
    return 0;
}