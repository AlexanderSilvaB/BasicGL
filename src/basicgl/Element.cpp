#include "Element.hpp"
#include "Serie.hpp"
#include "Plot.hpp"
#include "Manager.hpp"
#include <cmath>
#include <iostream>
#include <unistd.h>

#define GL_GLEXT_PROTOTYPES

#include <GL/freeglut.h>
#include <GL/freeglut_ext.h>
#include <GL/gl.h>

using namespace BasicGL;
using namespace std;

Element::Element(Elements element) : element(element)
{
    assoc = NULL;
    visible = true;
    wireframe = false;
    scales[0] = scales[1] = scales[2] = 1.0f;
    position[0] = position[1] = position[2] = 0;
    rotation[0] = rotation[1] = rotation[2] = 0;
    stroke = 1.0f;
    color[0] = color[1] = color[2] = color[3] = 1.0f;
    data = NULL;
    text = "";
    font = Default8x13;
    alignment = Left;
    applyColors = true;
    solid = NULL;
    init();
}

Element::~Element()
{

}

void Element::init()
{
    switch(element)
    {
        case POINT:
        case LINE:
        case TRIANGLE:
        case RECTANGLE:
            glGenBuffers(2, vboIds);
            reshape(1);
            break;
        case POLYGON:
        case SEQUENCE:
        case LOOP:
        case CONNECTED_TRIANGLES:
        case RADIAL_TRIANGLES:
        case CONNECTED_RECTANGLES:
            glGenBuffers(2, vboIds);
            reshape(0);
            break;
        case CIRCLE:
            glGenBuffers(2, vboIds);
            reshape(36);
            break;
        default:
            reshape(0);
            break;
    }
    
    switch(element)
    {
        case POINT:
            point(0, 0, 0);
            glow();
            map();
            break;
        case LINE:
            line(-0.5f, 0, 0, 0.5f, 0, 0);
            glow();
            map();
            break;
        case TRIANGLE:
            triangle(0, -0.5f, 0, -0.5f, 0.5f, 0, 0.5f, 0.5f, 0);
            glow();
            map();
            break;
        case RECTANGLE:
            rectangle(-0.5f, -0.5f, 0, 0.5f, 0.5f, 0);
            glow();
            map();
            break;
        case CIRCLE:
            circle(0, 0, 0, 0.5f);
            glow();
            map();
            break;
        default:
            break;
    }

    solid = gluNewQuadric();
    gluQuadricDrawStyle(solid, GLU_FILL);
    gluQuadricTexture(solid, GL_TRUE);
    gluQuadricNormals(solid, GLU_SMOOTH);
}

ElementPtr Element::reshape(int n, bool byElement)
{
    int m = 1;
    if(byElement)
    {
        switch(element)
        {
            case LINE:
                m = 2;
                break;
            case TRIANGLE:
                m = 3;
                break;
            case RECTANGLE:
                m = 4;
                break;
            default:
                break;
        }
    }
    points.resize(m*n);
    return this;
}

ElementPtr Element::create(Elements element)
{
    ElementPtr el = new Element(element);
    elements.push_back(el);
    return el;
}

ElementPtr Element::translate(float x, float y, float z)
{
    position[0] += x;
    position[1] += y;
    position[2] += z;
    return this;
}

ElementPtr Element::moveTo(float x, float y, float z)
{
    position[0] = x;
    position[1] = y;
    position[2] = z;
    return this;
}

ElementPtr Element::rotateX(float x)
{
    return rotate(x, 0, 0);
}

ElementPtr Element::rotateY(float y)
{
    return rotate(0, y, 0);
}

ElementPtr Element::rotateZ(float z)
{
    return rotate(0, 0, z);
}

ElementPtr Element::rotateToX(float x)
{
    return rotateTo(x, rotation[1], rotation[2]);
}

ElementPtr Element::rotateToY(float y)
{
    return rotateTo(rotation[0], y, rotation[2]);
}

ElementPtr Element::rotateToZ(float z)
{
    return rotateTo(rotation[0], rotation[1], z);
}

ElementPtr Element::rotate(float x, float y, float z)
{
    rotation[0] += x;
    rotation[1] += y;
    rotation[2] += z;
    return this;
}

ElementPtr Element::rotateTo(float x, float y, float z)
{
    rotation[0] = x;
    rotation[1] = y;
    rotation[2] = z;
    return this;
}

ElementPtr Element::scale(float x, float y, float z)
{
    if(y == FLT_MIN) y = x;
    if(z == FLT_MIN) z = y;
    scales[0] *= x;
    scales[1] *= y;
    scales[2] *= z;
    return this;
}

ElementPtr Element::scaleTo(float x, float y, float z)
{
    if(y == FLT_MIN) y = x;
    if(z == FLT_MIN) z = y;
    scales[0] = x;
    scales[1] = y;
    scales[2] = z;
    return this;
}

ElementPtr Element::rgb(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
    color[0] = r/255.0f;
    color[1] = g/255.0f;
    color[2] = b/255.0f;
    color[3] = a/255.0f;
    for(int i = 0; i < points.size(); i++)
        points.rgb(i, r, g, b, a);
    for(int i = 0; i < elements.size(); i++)
        elements[i]->rgb(r, g, b, a);
    return this;
}

ElementPtr Element::rgb(float r, float g, float b, float a)
{
    color[0] = r;
    color[1] = g;
    color[2] = b;
    color[3] = a;
    for(int i = 0; i < points.size(); i++)
        points.rgb(i, r, g, b, a);
    for(int i = 0; i < elements.size(); i++)
        elements[i]->rgb(r, g, b, a);
    return this;
}

ElementPtr Element::setWireframe(bool wireframe)
{
    this->wireframe = wireframe;
    for(int i = 0; i < elements.size(); i++)
        elements[i]->setWireframe(wireframe);
    return this;
}

int Element::newPoint(float x, float y, float z)
{
    int pt = points.add();
    points.moveTo(pt, x, y, z);
    points.rgb(pt, color[0], color[1], color[2], color[3]);
    return pt;
}

ElementPtr Element::point(float x, float y, int index)
{
    if(points.size() > index)
    {
        points.moveTo(index, x, y, points.Z(index));
    }
    return this;
}

ElementPtr Element::point(float x, float y, float z, int index)
{
    if(points.size() > index)
    {
        points.moveTo(index, x, y, z);
    }
    return this;
}

ElementPtr Element::line(float x1, float y1, float x2, float y2, int index)
{
    index *= 2;
    if(points.size() > index + 1)
    {
        points.moveTo(index, x1, y1, points.Z(index));
        points.moveTo(index+1, x2, y2, points.Z(index+1));
    }
    return this;
}

ElementPtr Element::line(float x1, float y1, float z1, float x2, float y2, float z2, int index)
{
    index *= 2;
    if(points.size() > index + 1)
    {
        points.moveTo(index, x1, y1, z1);
        points.moveTo(index+1, x2, y2, z2);
    }
    return this;
}

ElementPtr Element::triangle(float x1, float y1, float x2, float y2, float x3, float y3, int index)
{
    index *= 3;
    if(points.size() > index + 2)
    {
        points.moveTo(index, x1, y1, points.Z(index));
        points.moveTo(index+1, x2, y2, points.Z(index+1));
        points.moveTo(index+2, x3, y3, points.Z(index+2));
    }
    return this;
}

ElementPtr Element::triangle(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, int index)
{
    index *= 3;
    if(points.size() > index + 2)
    {
        points.moveTo(index, x1, y1, z1);
        points.moveTo(index+1, x2, y2, z2);
        points.moveTo(index+2, x3, y3, z3);
    }
    return this;
}

ElementPtr Element::rectangle(float x1, float y1, float x2, float y2, int index)
{
    index *= 4;
    if(points.size() > index + 3)
    {
        points.moveTo(index, x1, y1, points.Z(index));
        points.moveTo(index+1, x1, y2, points.Z(index+1));
        points.moveTo(index+2, x2, y2, points.Z(index+2));
        points.moveTo(index+3, x2, y1, points.Z(index+3));
    }
    return this;
}

ElementPtr Element::rectangle(float x1, float y1, float z1, float x2, float y2, float z2, int index)
{
    index *= 4;
    if(points.size() > index + 3)
    {
        points.moveTo(index, x1, y1, z1);
        points.moveTo(index+1, x1, y2, (z1+z2)/2.0f);
        points.moveTo(index+2, x2, y2, z2);
        points.moveTo(index+3, x2, y1, (z1+z2)/2.0f);
    }
    return this;
}

ElementPtr Element::circle(float x, float y, float r)
{
    float step = 2 * M_PI / (points.size() - 1);
    for(int i = 0; i < points.size(); i++)
    {
        points.moveTo(i, x + r * cos(step * i), y + r * sin(step * i), points.Z(i));
    }
    return this;
}

ElementPtr Element::circle(float x, float y, float z, float r)
{
    float step = 2 * M_PI / (points.size() - 1);
    for(int i = 0; i < points.size(); i++)
    {
        points.moveTo(i, x + r * cos(step * i), y + r * sin(step * i), z);
    }
    return this;
}

ElementPtr Element::setText(const string& text, Fonts font)
{
    this->text = text;
    this->font = font;
}

float Element::getTextWidth()
{
    const unsigned char* txt = (unsigned char*)text.c_str();
    return glutBitmapLength(getFont(), txt);
}

float Element::getTextHeight()
{
    float ret = 13;
    switch(font)
    {
        case Default8x13:
            ret = 13;
            break;
        case Default9x15:
            ret = 15;
            break;
        case TimesRoman10:
            ret = 10;
            break;
        case TimesRoman24:
            ret = 24;
            break;
        case Helvetica10:
            ret = 10;
            break;
        case Helvetica12:
            ret = 12;
            break;
        case Helvetica18:
            ret = 18;
            break;
        default:
            break;
    }
    return ret;
}

ElementPtr Element::textAlign(int alignment)
{
    this->alignment = alignment;
}

ElementPtr Element::glow()
{
    float random = rand() % 360;
    hsvTorgb(random, 1.0f, 1.0f, color);

    float step = 360.0f / points.size();
    for(int i = 0; i < points.size(); i++)
    {
        hsvTorgb(step*i, 1.0f, 1.0f, points.getColor(i).data);
    }
    return this;
}

ElementPtr Element::glow(int index, int pos)
{
    if(index < points.size())
    {
        float random;
        if(pos < 0)
            random = rand() % 360;
        else
            random = pos % 360;
        hsvTorgb(random, 1.0f, 1.0f, points.getColor(index).data);
    }
    return this;
}

ElementPtr Element::map()
{
    float minX, maxX, minY, maxY, x, y;
    for(int i = 0; i < points.size(); i++)
    {
        if(i == 0)
        {
            minX = maxX = points.X(i);
            minY = maxY = points.Y(i);
        }
        else
        {
            x = points.X(i);
            y = points.Y(i);
            if(x < minX)
                minX = x;
            else if(x > maxX)
                maxX = x;
            if(y < minY)
                minY = y;
            else if(y > maxY)
                maxY = y;
        }
    }
    for(int i = 0; i < points.size(); i++)
    {
        x = map(points.X(i), minX, maxX, 0.0f, 1.0f);
        y = map(points.Y(i), minY, maxY, 0.0f, 1.0f);
        points.map(i, x, y, 0);
    }
}

float Element::map(float v, float minIn, float maxIn, float minOut, float maxOut)
{
    return (v - minIn) * (maxOut - minOut) / (maxIn - minIn) + minOut;
}

void* Element::getFont()
{
    void* ret = GLUT_BITMAP_8_BY_13;
    switch(font)
    {
        case Default8x13:
            ret = GLUT_BITMAP_8_BY_13;
            break;
        case Default9x15:
            ret = GLUT_BITMAP_9_BY_15;
            break;
        case TimesRoman10:
            ret = GLUT_BITMAP_TIMES_ROMAN_10;
            break;
        case TimesRoman24:
            ret = GLUT_BITMAP_TIMES_ROMAN_24;
            break;
        case Helvetica10:
            ret = GLUT_BITMAP_HELVETICA_10;
            break;
        case Helvetica12:
            ret = GLUT_BITMAP_HELVETICA_12;
            break;
        case Helvetica18:
            ret = GLUT_BITMAP_HELVETICA_18;
            break;
        default:
            break;
    }
    return ret;
}

void Element::hsvTorgb(float h, float s, float v, float *rgb)
{
    float c = v*s;
    float h_ = h/60.0f;
    float x = c*(1 - abs( fmod(h_, 2) - 1));
    float r1, g1, b1;
    if(h_ >= 0 && h_ <= 1)
        r1 = c, g1 = x, b1 = 0;
    else if(h_ > 1 && h_ <= 2)
        r1 = x, g1 = c, b1 = 0;
    else if(h_ > 2 && h_ <= 3)
        r1 = 0, g1 = c, b1 = x;
    else if(h_ > 3 && h_ <= 4)
        r1 = 0, g1 = x, b1 = c;
    else if(h_ > 4 && h_ <= 5)
        r1 = x, g1 = 0, b1 = c;
    else if(h_ > 5 && h_ <= 6)
        r1 = c, g1 = 0, b1 = x;
    else
        r1 = 0, g1 = 0, b1 = 0;
    float m = v - c;
    rgb[0] = r1 + m;
    rgb[1] = g1 + m;
    rgb[2] = b1 + m;
    rgb[3] = 1.0f;
}

void Element::draw()
{
    if(!visible)
        return;
    if(assoc != NULL)
    {
        glPushMatrix();
        assoc->draw();
        glPopMatrix();
    }
    int n = 0;
    bool withBegin = true;
    GLenum t = GL_POINTS;
    switch(element)
    {
        case POINT:
            n = points.size();
            t = GL_POINTS;
            break;
        case LINE:
            n = points.size();
            t = GL_LINES;
            break;
        case TRIANGLE:
            n = points.size();
            if(wireframe)
                t = GL_LINE_LOOP;
            else
                t = GL_TRIANGLES;
            break;
        case RECTANGLE:
            n = 4;
            if(wireframe)
                t = GL_LINE_LOOP;
            else
                t = GL_QUADS;
            break;
        case POLYGON:
            n = points.size();
            if(wireframe)
                t = GL_LINE_LOOP;
            else
                t = GL_POLYGON;
            break;
        case SEQUENCE:
            n = points.size();
            t = GL_LINE_STRIP;
            break;
        case LOOP:
            n = points.size();
            t = GL_LINE_LOOP;
            break;
        case CIRCLE:
            n = points.size();
            if(wireframe)
                t = GL_LINE_LOOP;
            else
                t = GL_POLYGON;
            break;
        case CONNECTED_TRIANGLES:
            n = points.size();
            if(wireframe)
                t = GL_LINE_LOOP;
            else
                t = GL_TRIANGLE_STRIP;
            break;
        case RADIAL_TRIANGLES:
            n = points.size();
            if(wireframe)
                t = GL_LINE_LOOP;
            else
                t = GL_TRIANGLE_FAN;
            break;
        case CONNECTED_RECTANGLES:
            n = points.size();
            if(wireframe)
                t = GL_LINE_LOOP;
            else
                t = GL_QUAD_STRIP;
            break;
        case SPHERE:
        case CONE:
        case CUBE:
        case CYLINDER:
        case TEAPOT:
            withBegin = false;
            break;
        case CONTAINER:
            withBegin = false;
            break;
        default:
            break;
    }

    glPointSize(stroke);
    glLineWidth(stroke);
    glTranslatef(position[0], position[1], position[2]);
    glScalef(scales[0], scales[1], scales[2]);
    glRotatef(57.2958f * rotation[0], 1.0f, 0.0f, 0.0f);
    glRotatef(57.2958f * rotation[1], 0.0f, 1.0f, 0.0f);
    glRotatef(57.2958f * rotation[2], 0.0f, 0.0f, 1.0f);

    if(element == TEXT)
    {
        float tx = 0, ty = 0, tz = 0;
        if(alignment & CenterX)
            tx = - (getTextWidth()/Manager::WindowWidth());
        else if(alignment & Right)
            tx = - 2*(getTextWidth()/Manager::WindowWidth());
        if(alignment & CenterY)
            ty = -0.5f*(getTextHeight()/Manager::WindowHeight());
        else if(alignment & Top)
            ty = -(getTextHeight()/Manager::WindowHeight());

        glRasterPos3f(tx, ty, tz);
        glColor4fv(color);
        int len = text.length();
        void* font = getFont();
        for (int i = 0; i < len; i++) 
        {
            glutBitmapCharacter(font, text[i]);
        }
    }
    else
    {
        texture.begin();
        if(withBegin)
        {
            glEnableClientState(GL_VERTEX_ARRAY);
            if(applyColors)
                glEnableClientState(GL_COLOR_ARRAY);
            
            glBindBuffer(GL_ARRAY_BUFFER, vboIds[0]);
            glBufferData(GL_ARRAY_BUFFER, n * sizeof(PointLocation), points.rawXYZ(), GL_STREAM_DRAW);
            glVertexPointer(3, GL_FLOAT, 0, 0);

            if(applyColors)
            {
                glBindBuffer(GL_ARRAY_BUFFER, vboIds[1]);
                glBufferData(GL_ARRAY_BUFFER, n * sizeof(PointColor), points.rawColor(), GL_STREAM_DRAW);
                glColorPointer(4, GL_FLOAT, 0, 0);
            }

            texture.draw(n, points.rawCoords());
        
            glDrawArrays(t, 0, n);

            glDisableClientState(GL_VERTEX_ARRAY);
            if(applyColors)
                glDisableClientState(GL_COLOR_ARRAY); 
            
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }
        else
        {
            glColor4fv(color);
            switch(element)
            {
                case SPHERE:
                    if(wireframe)
                        glutWireSphere(1.0f, 20, 20);
                    else
                        gluSphere(solid, 1.0f, 20, 20);
                        // glutSolidSphere(1.0f, 20, 20);
                    break;
                case CONE:
                    if(wireframe)
                        glutWireCone(0.5f, 1.0f, 20, 20);
                    else
                        //glutSolidCone(0.5f, 1.0f, 20, 20);
                        gluCylinder(solid, 0.5f, 0.0f, 1.0f, 20, 20);
                    break;
                case CUBE:
                    if(wireframe)
                        glutWireCube(1.0f);
                    else
                    {
                        // temp
                        glEnable(GL_TEXTURE_GEN_S);
                        glEnable(GL_TEXTURE_GEN_T);
                        glutSolidCube(1.0f);
                        glDisable(GL_TEXTURE_GEN_S);
                        glDisable(GL_TEXTURE_GEN_T);
                    }
                    break;
                case CYLINDER:
                    if(wireframe)
                        glutWireCylinder(0.5f, 1.0f, 20, 20);
                    else
                        //glutSolidCylinder(0.5f, 1.0f, 20, 20);
                        gluCylinder(solid, 0.5f, 0.5f, 1.0f, 20, 20);
                    break;
                case TEAPOT:
                    if(wireframe)
                        glutWireTeapot(1.0f);
                    else
                        glutSolidTeapot(1.0f);
                    break;
                default:
                    break;
            }   
        }
        texture.end();
    }
    glColor4fv(color);
    for(int i = 0; i < elements.size(); i++)
    {
        if(dynamic_cast<Plot*>(this) != NULL && dynamic_cast<Serie*>(elements[i]) != NULL)
        {
            Plot *plt = (Plot*) this;

            Serie* serie = (Serie*)elements[i];
            if(!serie->aligned)
            {
                if(plt->fixedAxis)
                {
                    serie->minX = plt->axisMinX;
                    serie->maxX = plt->axisMaxX;
                    serie->minY = plt->axisMinY;
                    serie->maxY = plt->axisMaxY;
                }
                serie->align(plt->minX, plt->maxX, plt->minY, plt->maxY, serie->color);
                serie->aligned = true;
            }
        }
        glPushMatrix();
        elements[i]->draw();
        glPopMatrix();
    }
}