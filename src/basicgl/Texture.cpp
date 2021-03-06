#include "Texture.hpp"
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

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

using namespace BasicGL;
using namespace std;

#ifdef IMAGE_CAPTURE
#include <opencv2/opencv.hpp>
#endif

Texture::Texture()
{
    hasData = false;
    id = 0;
    width = 0;
    height = 0;
    vbo = 0;
    id = 0;


    // Capture image
    autoCapture = false;
    captureDevicePointer = NULL;
}

Texture::~Texture()
{
    free();
    
    #ifdef IMAGE_CAPTURE
    if(captureDevicePointer != NULL)
        delete (cv::VideoCapture*)captureDevicePointer;
    captureDevicePointer = NULL;
    #endif
}

int Texture::getWidth()
{
    return width;
}

int Texture::getHeight()
{
    return height;
}

float Texture::getRatio()
{
    if(height <= 0)
        return 1;
    return width/height;
}

bool Texture::available()
{
    return hasData;
}

bool Texture::load(const std::string& fileName)
{
    int width, height, bpp;
    uint8_t* data = stbi_load(fileName.c_str(), &width, &height, &bpp, STBI_default);
    fill(width, height, bpp, data);
    stbi_image_free(data);
    return true;
}

void Texture::fill(const unsigned int width, const unsigned int height, const unsigned int bpp, const unsigned char *data)
{
    free();

    glEnable(GL_TEXTURE_2D);

    glGenBuffers( 1, &vbo );
    glGenTextures( 1, &id );

    glBindTexture(GL_TEXTURE_2D, id);
    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,GL_MODULATE );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST );

    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_REPEAT );

    if (bpp == 1) 
    {
        gluBuild2DMipmaps( GL_TEXTURE_2D, GL_ALPHA, width, height, GL_ALPHA, GL_UNSIGNED_BYTE, data );
        //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    }
    else if (bpp == 3) 
    {
        gluBuild2DMipmaps( GL_TEXTURE_2D, GL_RGB, width, height, GL_RGB, GL_UNSIGNED_BYTE, data );
        //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    } 
    else if (bpp == 4) 
    {
        gluBuild2DMipmaps( GL_TEXTURE_2D, GL_RGBA, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data );
        //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    } 
    else 
    {
        assert(0);
    }

    glBindTexture(GL_TEXTURE_2D, 0);

    glDisable(GL_TEXTURE_2D);

    this->width = width;
    this->height = height;
    this->bpp = bpp;
    this->hasData = true;
}

void Texture::update(const unsigned char *data)
{
    if(!available())
        return;

    glEnable(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, id);
    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,GL_MODULATE );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST );

    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_REPEAT );

    if (bpp == 1) 
    {
        gluBuild2DMipmaps( GL_TEXTURE_2D, 1, width, height, GL_RED, GL_UNSIGNED_BYTE, data );
        //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    }
    else if (bpp == 3) 
    {
        gluBuild2DMipmaps( GL_TEXTURE_2D, 3, width, height, GL_RGB, GL_UNSIGNED_BYTE, data );
        //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    } 
    else if (bpp == 4) 
    {
        gluBuild2DMipmaps( GL_TEXTURE_2D, 4, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data );
        //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    } 
    else 
    {
        assert(0);
    }

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
}

void Texture::free()
{
    if(!available())
        return;
    glDeleteTextures(1, &id);
    glDeleteBuffers(1, &vbo);
    hasData = false;
}

void Texture::begin()
{
    #ifdef IMAGE_CAPTURE
    if(autoCapture)
        captureFromDevice();
    #endif

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

// Capture image
bool Texture::supportsCapture()
{
    #ifdef IMAGE_CAPTURE
    return true;
    #else
    return false;
    #endif
}

bool Texture::setCaptureDevice(int id)
{
    #ifdef IMAGE_CAPTURE

    if(captureDevicePointer != NULL)
        delete (cv::VideoCapture*)captureDevicePointer;
    captureDevicePointer = new cv::VideoCapture(id);
    if(!((cv::VideoCapture*)captureDevicePointer)->isOpened())
    {
        delete (cv::VideoCapture*)captureDevicePointer;
        captureDevicePointer = NULL;
        return false;
    }
    return true;
    #else
    return false;
    #endif
}

bool Texture::setCaptureDevice(const std::string& path)
{
    #ifdef IMAGE_CAPTURE

    if(captureDevicePointer != NULL)
        delete (cv::VideoCapture*)captureDevicePointer;
    captureDevicePointer = new cv::VideoCapture(path);
    if(!((cv::VideoCapture*)captureDevicePointer)->isOpened())
    {
        delete (cv::VideoCapture*)captureDevicePointer;
        captureDevicePointer = NULL;
        return false;
    }
    return true;
    #else
    return false;
    #endif
}

void Texture::captureFromDevice()
{
    #ifdef IMAGE_CAPTURE
    if(captureDevicePointer == NULL)
        return;
    cv::Mat img;
    if(((cv::VideoCapture*)captureDevicePointer)->read(img))
    {
        cv::cvtColor(img, img, cv::COLOR_BGR2RGB);
        if(!available())
            fill(img.cols, img.rows, 3, img.data);
        else
        {
            if(width == img.cols && height == img.rows && bpp == 3)
                update(img.data);
            else
                fill(img.cols, img.rows, 3, img.data);
        }
    }
    #endif
}