#include "Texture.hpp"
#include <cstdio>
#include <cstdlib>
#include <iostream>

#define GL_GLEXT_PROTOTYPES

#include <GL/freeglut.h>
#include <GL/freeglut_ext.h>
#include <GL/gl.h>

using namespace BasicGL;
using namespace std;

Texture::Texture()
{
    hasData = false;
    id = 0;
    width = 0;
    height = 0;
}

Texture::~Texture()
{
    free();
}

bool Texture::available()
{
    return hasData;
}

bool Texture::load(const std::string& fileName)
{
    FILE* f = fopen(fileName.c_str(), "rb");
    if(f == NULL)
        return false;
    
    unsigned char info[54];
    fread(info, sizeof(unsigned char), 54, f);

    unsigned int width = *(int*)&info[18];
    unsigned int height = *(int*)&info[22];

    // cout << endl;
    // cout << "  Name: " << fileName << endl;
    // cout << " Width: " << width << endl;
    // cout << "Height: " << height << endl;

    int size = 3 * width * height;
    unsigned char* data = new unsigned char[size]; // allocate 3 bytes per pixel
    fread(data, sizeof(unsigned char), size, f); // read the rest of the data at once
    fclose(f);

    unsigned char tmp;
    for(int i = 0; i < size; i+= 3)
    {
        tmp = data[i];
        data[i] = data[i+2];
        data[i+2] = tmp;
    }

    fill(width, height, data);
    delete[] data;

    return true;
}

void Texture::fill(const unsigned int width, const unsigned int height, const unsigned char *data)
{
    free();

    glGenBuffers( 1, &vbo );
    
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,GL_MODULATE );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST );

    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_REPEAT );
    gluBuild2DMipmaps( GL_TEXTURE_2D, 3, width, height, GL_RGB, GL_UNSIGNED_BYTE, data );

    this->width = width;
    this->height = height;
    this->hasData = true;
}

void Texture::free()
{
    if(!available())
        return;
    glDeleteTextures(1, &id);
    glDeleteBuffers(1, &vbo);
}

void Texture::begin()
{
    if(!available())
        return;
    glEnable(GL_TEXTURE_2D);
    glBindTexture (GL_TEXTURE_2D, id);
    glColor4f(1.0, 1.0, 1.0, 1.0); 

    glEnableClientState( GL_TEXTURE_COORD_ARRAY );
    //glEnable(GL_TEXTURE_GEN_S);
    //glEnable(GL_TEXTURE_GEN_T);
}

void Texture::end()
{
    if(!available())
        return;
    glDisableClientState( GL_TEXTURE_COORD_ARRAY );
    glBindTexture (GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
    //glDisable(GL_TEXTURE_GEN_S);
    //glDisable(GL_TEXTURE_GEN_T);
}

void Texture::draw(int n, PointTexCoord *coords)
{
    if(!available())
        return;
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, n * sizeof(PointTexCoord), coords, GL_STREAM_DRAW);
    glTexCoordPointer(3, GL_FLOAT, 0, 0);
}