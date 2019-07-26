#include "Point.hpp"

using namespace BasicGL;
using namespace std;

Point::Point()
{
    color[0] = color[1] = color[2] = color[3] = 1.0f;
    xyz[0] = xyz[1] = xyz[2] = 0.0f;
}

Point::~Point()
{

}

Point& Point::translate(float x, float y, float z)
{
    xyz[0] += x;
    xyz[1] += y;
    xyz[2] += z;
    return *this;
}

Point& Point::moveTo(float x, float y, float z)
{
    xyz[0] = x;
    xyz[1] = y;
    xyz[2] = z;
    return *this;
}

Point& Point::rgb(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
    color[0] = r/255.0f;
    color[1] = g/255.0f;
    color[2] = b/255.0f;
    color[3] = a/255.0f;
    return *this;
}

Point& Point::rgb(float r, float g, float b, float a)
{
    color[0] = r;
    color[1] = g;
    color[2] = b;
    color[3] = a;
    return *this;
}