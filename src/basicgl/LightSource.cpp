#include "LightSource.hpp"

#ifdef WIN32
#include <GL/glew.h>
#include <GL/wglew.h>
#endif

#define GL_GLEXT_PROTOTYPES
#include <GL/freeglut.h>
#include <GL/freeglut_ext.h>
#include <GL/gl.h>

using namespace BasicGL;
using namespace std;

LightSource::LightSource() : 
    ambient{0.0f, 0.0f, 0.0f, 1.0f},
    diffuse{1.0f, 1.0f, 1.0f, 1.0f},
    specular{1.0f, 1.0f, 1.0f, 1.0f},
    position{0.0f, 0.0f, 1.0f, 0.0f},
    spotDirection{0.0f, 0.0f, -1.0f},
    spotExponent(0.0f),
    spotCutOff(180.0f),
    constantAttenuation(1.0f),
    linearAttenuation(0.0f),
    quadraticAttenuation(0.0f),
    translation{0.0f, 0.0f, 0.0f},
    rotation{0.0f, 0.0f, 0.0f}
{
    updated = false;
}

LightSource::~LightSource()
{

}

void LightSource::update()
{
    updated = false;
}

void LightSource::apply(unsigned int index, bool cartesian)
{
    if(updated)
        return;

    GLenum light = GL_LIGHT0 + index;

    if(cartesian)
        glTranslatef(translation[0], translation[1], translation[2]);
    else
        glTranslatef(translation[0], -translation[1], translation[2]);
    if(cartesian)
        glRotatef( 57.2958f * rotation[0], 1.0f, 0.0f, 0.0f);
    else
        glRotatef(-57.2958f * rotation[0], 1.0f, 0.0f, 0.0f);
    glRotatef(57.2958f * rotation[1], 0.0f, 1.0f, 0.0f);
    glRotatef(57.2958f * rotation[2], 0.0f, 0.0f, 1.0f);

    glLightfv(light, GL_AMBIENT, ambient);
    glLightfv(light, GL_DIFFUSE, diffuse);
    glLightfv(light, GL_SPECULAR, specular);
    glLightfv(light, GL_POSITION, position);
    glLightfv(light, GL_SPOT_DIRECTION, spotDirection);
    glLightf(light, GL_SPOT_EXPONENT, spotExponent);
    glLightf(light, GL_SPOT_CUTOFF, spotCutOff);
    glLightf(light, GL_CONSTANT_ATTENUATION, constantAttenuation);
    glLightf(light, GL_LINEAR_ATTENUATION, linearAttenuation);
    glLightf(light, GL_QUADRATIC_ATTENUATION, quadraticAttenuation);

    updated = true;
}

LightSourcePtr LightSource::rgb(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
    diffuse[0] = r/255.0f;
    diffuse[1] = g/255.0f;
    diffuse[2] = b/255.0f;
    diffuse[3] = a/255.0f;
    return this;
}

LightSourcePtr LightSource::rgb(float r, float g, float b, float a)
{
    diffuse[0] = r;
    diffuse[1] = g;
    diffuse[2] = b;
    diffuse[3] = a;
    return this;
}

LightSourcePtr LightSource::glow()
{
    float random = rand() % 360;
    Element::hsvTorgb(random, 1.0f, 1.0f, diffuse);
    return this;
}

LightSourcePtr LightSource::translate(float x, float y, float z)
{
    position[0] += x;
    position[1] += y;
    position[2] += z;
    return this;
}

LightSourcePtr LightSource::moveTo(float x, float y, float z)
{
    position[0] = x;
    position[1] = y;
    position[2] = z;
    translation[0] = translation[1] = translation[2] = 0.0f;
    return this;
}

LightSourcePtr LightSource::rotateX(float x)
{
    return rotate(x, 0, 0);
}

LightSourcePtr LightSource::rotateY(float y)
{
    return rotate(0, y, 0);
}

LightSourcePtr LightSource::rotateZ(float z)
{
    return rotate(0, 0, z);
}

LightSourcePtr LightSource::rotateToX(float x)
{
    return rotateTo(x, rotation[1], rotation[2]);
}

LightSourcePtr LightSource::rotateToY(float y)
{
    return rotateTo(rotation[0], y, rotation[2]);
}

LightSourcePtr LightSource::rotateToZ(float z)
{
    return rotateTo(rotation[0], rotation[1], z);
}

LightSourcePtr LightSource::rotate(float x, float y, float z)
{
    rotation[0] += x;
    rotation[1] += y;
    rotation[2] += z;
    return this;
}

LightSourcePtr LightSource::rotateTo(float x, float y, float z)
{
    rotation[0] = x;
    rotation[1] = y;
    rotation[2] = z;
    return this;
}

LightSourcePtr LightSource::rotateAroundX(ElementPtr element, float x)
{
    return rotateAround(element, x, 0, 0);
}

LightSourcePtr LightSource::rotateAroundY(ElementPtr element, float y)
{
    return rotateAround(element, 0, y, 0);
}

LightSourcePtr LightSource::rotateAroundZ(ElementPtr element, float z)
{
    return rotateAround(element, 0, 0, z);
}

LightSourcePtr LightSource::rotateAroundToX(ElementPtr element, float x)
{
    return rotateAroundTo(element, x, rotation[1], rotation[2]);
}

LightSourcePtr LightSource::rotateAroundToY(ElementPtr element, float y)
{
    return rotateAroundTo(element, rotation[0], y, rotation[2]);
}

LightSourcePtr LightSource::rotateAroundToZ(ElementPtr element, float z)
{
    return rotateAroundTo(element, rotation[0], rotation[1], z);
}

LightSourcePtr LightSource::rotateAround(ElementPtr element, float x, float y, float z)
{
    translation[0] = element->position[0];
    translation[1] = element->position[1];
    translation[2] = element->position[2];

    rotation[0] += x;
    rotation[1] += y;
    rotation[2] += z;

    position[0] -= element->position[0];
    position[1] -= element->position[1];
    position[2] -= element->position[2];

    return this;
}

LightSourcePtr LightSource::rotateAroundTo(ElementPtr element, float x, float y, float z)
{
    translation[0] = element->position[0];
    translation[1] = element->position[1];
    translation[2] = element->position[2];

    rotation[0] = x;
    rotation[1] = y;
    rotation[2] = z;

    position[0] -= element->position[0];
    position[1] -= element->position[1];
    position[2] -= element->position[2];

    return this;
}