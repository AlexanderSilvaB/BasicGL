#include <basicgl/Manager.hpp>
#include <cmath>
#include <iostream>
#include <vector>

using namespace std;
using namespace BasicGL;

int main(int argc, char *argv[])
{
    Manager::Init(argc, argv);

    Manager::Create("Plot", MODE_2D);
    Manager::SetBackground(13, 40, 53);

    PlotPtr plt = Manager::CreatePlot();
    
    SeriePtr sint = plt->createSerie("r");
    SeriePtr cost = plt->createSerie("b");

    float x, y;
    for(int i = 0; i < 360*2; i++)
    {
        float t = (i / 360.0f) * 2 * M_PI;
        x = (t / M_PI);
        y = sin(t);
        sint->add(x, y);
        y = cos(t);
        cost->add(x, y);
        Manager::Pause();
    }
    Manager::Show();
    Manager::Destroy();
    
    return 0;
}