#include <basicgl/Manager.hpp>
#include <cmath>
#include <iostream>
#include <vector>

using namespace std;
using namespace BasicGL;

int main(int argc, char *argv[])
{
    Manager::Init(argc, argv);

    Manager::CreateWindow("Robot", MODE_2D);
    Manager::SetBackground(13, 40, 53);

    ElementPtr robot = Manager::CreateElement(CONTAINER);
    robot->create(TRIANGLE)->rotateTo(0, 0, M_PI/2)->scaleTo(0.5)->translate(0.6, 0, 0);
    robot->create(CIRCLE);
    robot->scaleTo(0.2f)->rgb(0, 0, 1.0f);

    float x = 0, y = 0, th = 0;
    float vl = 0.1;
    float vth = 0;

    float xeq, yeq, dxeq, dyeq;
    float a = 0.6f;

    float t = 0;
    float dt = 0.01;
    while(Manager::IsOpen())
    {
        xeq = a*sqrt(2)*cos(t)/(sin(t)*sin(t) + 1);
        yeq = a*sqrt(2)*cos(t)*sin(t)/(sin(t)*sin(t) + 1);
        dxeq = xeq - x;
        dyeq = yeq - y;
        vl = sqrt(dxeq*dxeq + dyeq*dyeq);
        vth = atan2(dyeq, dxeq) - th;

        x += vl*cos(th + vth);
        y += vl*sin(th + vth);
        th += vth;

        robot->moveTo(x, y, 0);
        robot->rotateToZ(th);
        Manager::Pause();
        t += dt;
    }
    Manager::Show();
    Manager::Destroy();
        
    return 0;
}