#include "Point.hpp"

using namespace BasicGL;
using namespace std;

void Point::init(PointLocation *xyz, PointColor* color)
{
    color->data[0] = color->data[1] = color->data[2] = color->data[3] = 1.0f;
    xyz->data[0] = xyz->data[1] = xyz->data[2] = 0.0f;
}

void Point::translate(PointLocation *xyz, float x, float y, float z)
{
    xyz->data[0] += x;
    xyz->data[1] += y;
    xyz->data[2] += z;
}

void Point::moveTo(PointLocation *xyz, float x, float y, float z)
{
    xyz->data[0] = x;
    xyz->data[1] = y;
    xyz->data[2] = z;
}

void Point::rgb(PointColor* color, unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
    color->data[0] = r/255.0f;
    color->data[1] = g/255.0f;
    color->data[2] = b/255.0f;
    color->data[3] = a/255.0f;
}

void Point::rgb(PointColor* color, float r, float g, float b, float a)
{
    color->data[0] = r;
    color->data[1] = g;
    color->data[2] = b;
    color->data[3] = a;
}

// Points list
PointList::PointList()
{
    xyz.clear();
    color.clear();
}

PointList::~PointList()
{

}

int PointList::add()
{
    PointLocation loc;
    PointColor col;
    Point::init(&loc, &col);
    xyz.push_back(loc);
    color.push_back(col);
    return xyz.size() - 1;
}

void PointList::rm(int index)
{
    if(xyz.size() > index)
    {
        xyz.erase(xyz.begin() + index);
        color.erase(color.begin() + index);
    }
}

PointLocation& PointList::getXYZ(int index)
{
    return xyz[index];
}
            
PointColor& PointList::getColor(int index)
{
    return color[index];
}

int PointList::size()
{
    return xyz.size();
}

void PointList::resize(int size)
{
    xyz.resize(size);
    color.resize(size);
}

PointLocation* PointList::rawXYZ()
{
    return xyz.data();
}

PointColor* PointList::rawColor()
{
    return color.data();
}

float PointList::X(int index)
{
    return xyz[index].data[0];
}

float PointList::Y(int index)
{
    return xyz[index].data[1];
}

float PointList::Z(int index)
{
    return xyz[index].data[2];
}

float PointList::R(int index)
{
    return color[index].data[0];
}

float PointList::G(int index)
{
    return color[index].data[1];
}

float PointList::B(int index)
{
    return color[index].data[2];
}

float PointList::A(int index)
{
    return color[index].data[3];
}

void PointList::translate(int index, float x, float y, float z)
{
    if(xyz.size() > index)
    {
        Point::translate(&xyz[index], x, y, z);
    }
}

void PointList::moveTo(int index, float x, float y, float z)
{
    if(xyz.size() > index)
    {
        Point::moveTo(&xyz[index], x, y, z);
    }
}

void PointList::rgb(int index, unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
    if(xyz.size() > index)
    {
        Point::rgb(&color[index], r, g, b, a);
    }
}

void PointList::rgb(int index, float r, float g, float b, float a)
{
    if(xyz.size() > index)
    {
        Point::rgb(&color[index], r, g, b, a);
    }
}