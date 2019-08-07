#include "Font.hpp"
#include "Manager.hpp"
#include <cstdio>
#include <cstdlib>
#include <iostream>

#ifdef WIN32
#include <GL/glew.h>
#include <GL/wglew.h>
#endif

#define GL_GLEXT_PROTOTYPES
#include <GL/freeglut.h>
#include <GL/freeglut_ext.h>
#include <GL/gl.h>

#define STB_TRUETYPE_IMPLEMENTATION
#include <stb/stb_truetype.h>

#include <stb/stb_image_write.h>

using namespace BasicGL;
using namespace std;

Font::Font()
{
    isLoaded = false;
    defaultFont = Default8x13;
    alignment = Align_Left;
    texture = 0;
    firstChar = ' ';
    lastChar = '~';
    width = 1024;
    height = 1024;
    fontSize = 100;
    size = 40.0f;
}

Font::~Font()
{
    free();
}

bool Font::loaded()
{
    if(defaultFont != Custom)
        return true;
    return isLoaded;
}

bool Font::load(const std::string& fileName)
{
    long size;
    unsigned char* fontBuffer;
    
    FILE* fontFile = fopen(fileName.c_str(), "rb");
    if(fontFile == NULL)
        return false;

    fseek(fontFile, 0, SEEK_END);
    size = ftell(fontFile);
    fseek(fontFile, 0, SEEK_SET);
    
    fontBuffer = new unsigned char[size];
    
    fread(fontBuffer, size, 1, fontFile);
    fclose(fontFile);

    bool ret = fill(fontBuffer);
    delete[] fontBuffer;

    return ret;
}

bool Font::fill(const unsigned char *fontData)
{
    free();
    int charCount = lastChar - firstChar;

    unsigned char *data = new unsigned char[width * height];
    cdata = new stbtt_bakedchar[charCount];
    
    int ret = stbtt_BakeFontBitmap(fontData, 0, fontSize, data, width, height, firstChar, charCount, (stbtt_bakedchar*)cdata);

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, width, height, 0, GL_ALPHA, GL_UNSIGNED_BYTE, data);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST);
    // glGenerateMipmap(GL_TEXTURE_2D);
    // glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    glBindTexture(GL_TEXTURE_2D, 0);

    delete[] data;

    defaultFont = Custom;
    isLoaded = true;
    return true;
}

void Font::free()
{
    if(!isLoaded)
        return;
    glDeleteTextures(1, &texture);
}

void Font::print(const string& text, bool flip)
{
    if(!loaded())
        return;

    if(defaultFont != Custom)
    {
        float tx = 0, ty = 0, tz = 0;
        if(alignment & Align_CenterX)
            tx = - (getTextWidth(text)/Manager::WindowWidth());
        else if(alignment & Align_Right)
            tx = - 2*(getTextWidth(text)/Manager::WindowWidth());
        if(alignment & Align_CenterY)
            ty = -0.5f*(getTextHeight(text)/Manager::WindowHeight());
        else if(alignment & Align_Top)
            ty = -(getTextHeight(text)/Manager::WindowHeight());

        glDisable(GL_LIGHTING);
        glRasterPos3f(tx, ty, tz);
        int len = text.length();
        void* font = getDefaultFont();
        for (int i = 0; i < len; i++) 
        {
            glutBitmapCharacter(font, text[i]);
        }
        glEnable(GL_LIGHTING);
    }
    else
    {
        float x = 0, y = 0;
        float x0 = 0, y0 = 0;
        int len = text.length();
        char c;
        float xMin, xMax, yMin, yMax;

        float sx = size * ( 1.0f / ( Manager::WindowWidth() * fontSize ) );
        float sy = size * ( 1.0f / ( Manager::WindowHeight() * fontSize ) );

        if(alignment & Align_CenterX)
            x0 = -getTextWidth(text) / 2.0f;
        else if(alignment & Align_Right)
            x0 = - getTextWidth(text);
        if(alignment & Align_CenterY)
            y0 = -getTextHeight(text) / 2.0f;
        else if(alignment & Align_Top)
            y0 = -getTextHeight(text);

        if(flip)
            glScalef(1.0f, -1.0f, 1.0f);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texture);
        glBegin(GL_QUADS);

        x = x0;
        y = y0;

        for (int i = 0; i < len; i++) 
        {
            c = text[i];
            if(c == '\n')
            {
                x = x0;
                y += size;
                continue;
            }
            else if (c >= firstChar && c <= lastChar) 
            {
                stbtt_aligned_quad q;
                stbtt_GetBakedQuad((stbtt_bakedchar*)cdata, width, height, c - firstChar, &x, &y, &q, 1);
                xMin = q.x0 * sx;
                xMax = q.x1 * sx;
                yMin = q.y0 * sy;
                yMax = q.y1 * sy;
                glTexCoord2f(q.s0,q.t0); 
                glVertex2f(xMin, yMin);
                glTexCoord2f(q.s1,q.t0); 
                glVertex2f(xMax, yMin);
                glTexCoord2f(q.s1,q.t1); 
                glVertex2f(xMax, yMax);
                glTexCoord2f(q.s0,q.t1); 
                glVertex2f(xMin, yMax);
            }
        }

        glEnd();
        glBindTexture(GL_TEXTURE_2D, 0);
        glDisable(GL_TEXTURE_2D);
    }
}

void Font::setDefaultFont(DefaultFonts defaultFont)
{
    this->defaultFont = defaultFont;
}

float Font::getTextWidth(const string& text)
{
    float x = 0, y = 0, w1 = 0, w2 = 0;
    char c;
    unsigned char t[2];
    t[1] = '\0';
    float defH = getCharHeight(' ');
    float sc = size / 5.0f;
    for (int i = 0; i < text.length(); i++) 
    {
        c = text[i];
        if(c == '\n')
        {
            x = 0;
            if(defaultFont != Custom)
                y +=  defH;
            else
                y += fontSize;
            w2 = w1;
            w1 = 0;
            continue;
        }
        else if(c >= firstChar && c <= lastChar)
        {
            if(defaultFont != Custom)
            {
                t[0] = c;
                w1 += glutBitmapLength(getDefaultFont(), t);
            }
            else
            {
                stbtt_aligned_quad q;
                stbtt_GetBakedQuad((stbtt_bakedchar*)cdata, width, height, c - firstChar, &x, &y, &q, 1);
                w1 += q.x1 - q.x0 + sc;
            }
        }
    }
    return max(w1, w2);
}

float Font::getTextHeight(const std::string& text)
{
    float x = 0, y = 0, h, h1 = 0, h2 = 0;
    char c;
    float defH = getCharHeight(' ');
    for (int i = 0; i < text.length(); i++) 
    {
        c = text[i];
        if(c == '\n')
        {
            x = 0;
            if(defaultFont != Custom)
                y +=  defH;
            else
                y += fontSize;
            h2 += h1;
            h1 = 0;
            continue;
        }
        else if(c >= firstChar && c <= lastChar)
        {
            if(defaultFont != Custom)
            {
                h = defH;
            }
            else
            {
                stbtt_aligned_quad q;
                stbtt_GetBakedQuad((stbtt_bakedchar*)cdata, width, height, c - firstChar, &x, &y, &q, 1);
                h += q.y1 - q.y0;
            }
            if(h > h1)
                h1 = h;
        }
    }
    return h1 + h2;
}

float Font::getCharHeight(char c)
{
    float ret = 13;
    switch(defaultFont)
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
        case Custom:
            ret = size;
            break;
        default:
            break;
    }
    return ret;
}

void* Font::getDefaultFont()
{
    void* ret = GLUT_BITMAP_8_BY_13;
    switch(defaultFont)
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