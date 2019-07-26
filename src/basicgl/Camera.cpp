#include "Camera.hpp"
#include <cmath>

using namespace BasicGL;
using namespace std;

Camera::Camera()
{
    rotation[0] = rotation[1] = rotation[2] = 0;
    translation[0] = 0;
    translation[1] = 0;
    translation[2] = -1.0f;
    position[0] = 0;
    position[1] = 0;
    position[2] = 5.0f;
}

Camera::~Camera()
{

}

Camera& Camera::translate(float x, float y, float z)
{
    position[0] += x;
    position[1] += y;
    position[2] += z;
    return *this;
}

Camera& Camera::moveTo(float x, float y, float z)
{
    position[0] = x;
    position[1] = y;
    position[2] = z;
    return *this;
}

Camera& Camera::rotate(float x, float y, float z)
{
    rotation[0] += x;
    rotation[1] += y;
    rotation[2] += z;
    translation[0] = cos(rotation[1]) * -sin(rotation[0]);
    translation[1] = sin(rotation[1]);
    translation[2] = cos(rotation[1]) * -cos(rotation[0]);
    return *this;
}

Camera& Camera::rotateTo(float x, float y, float z)
{
    rotation[0] = x;
    rotation[1] = y;
    rotation[2] = z;
    translation[0] = cos(rotation[1]) * sin(rotation[0]);
    translation[1] = sin(rotation[1]);
    translation[2] = cos(rotation[1]) * cos(rotation[0]);
    return *this;
}