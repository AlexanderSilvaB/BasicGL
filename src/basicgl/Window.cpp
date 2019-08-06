#include "Window.hpp"
#include <cstring>
#include <GL/glut.h>
#include <iostream>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb/stb_image_write.h>

#include <jo_mpeg/jo_mpeg.h>

using namespace BasicGL;
using namespace std;

Window::Window()
{
    bg[0] = bg[1] = bg[2] = 0.0f;
    bg[3] = 1.0f;
    animationFunction = NULL;
    timeSinceBegin = 0;
    cartesian = true;
    memset(&lastTime, '\0', sizeof(lastTime));
    opened = true;
    fullscren = false;
    windowedX = 50;
    windowedY = 50;
    windowedWidth = 320;
    windowedHeight = 240;
    lastMouseX = lastMouseY = 0;
    saveFileName = "";
    saveAsVideo = false;
    captureFrame = false;
    fps = 30;
    frame = NULL;
    fp = NULL;
}

Window::~Window()
{

}

void Window::assocElement(ElementPtr element)
{
    assocElements.push_back(element);
}

void Window::Destroy()
{
    for(int i = 0; i < assocElements.size(); i++)
    {
        ElementPtr el = assocElements[i];
        delete el;
    }
    assocElements.clear();
    StopRecording();
}

bool Window::IsFullscreen()
{
    return fullscren;
}

void Window::SetFullscreen(bool enabled)
{
    if(enabled != fullscren)
    {
        fullscren = enabled;
        if(enabled)
        {
            windowedX = glutGet(GLUT_WINDOW_X);
            windowedY = glutGet(GLUT_WINDOW_Y);
            windowedWidth = glutGet(GLUT_WINDOW_WIDTH);
            windowedHeight = glutGet(GLUT_WINDOW_HEIGHT);
            glutFullScreen();
        }
        else
        {
            glutReshapeWindow(windowedWidth, windowedHeight);
            glutPositionWindow(windowedX, windowedY);
        }
    }
}

void Window::ToggleFullscreen()
{
    SetFullscreen(!fullscren);
}

float Window::ZPlane()
{
    return camera.position[2] * 0.5f;
}

ElementPtr Window::find(const string& name)
{
    ElementPtr ret = NULL;
    for(int i = 0; i < elements.size(); i++)
    {
        ret = elements[i]->find(name);
        if(ret != NULL)
            break;
    }
    return ret;
}

ElementPtr Window::get(int index)
{
    if(elements.size() > index)
        return elements[index];
    return NULL;
}

bool Window::Capture(const std::string& fileName)
{
    if(captureFrame)
        return false;

    saveFileName = fileName;
    captureFrame = true;
    saveAsVideo = false;
    return true;
}

bool Window::StartRecording(const std::string& fileName, int fps)
{
    if(captureFrame)
        return false;

    fp = fopen(fileName.c_str(), "wb");
    if(fp == NULL)
    {
        return false;
    }

    saveFileName = fileName;
    captureFrame = true;
    saveAsVideo = true;
    this->fps = fps;
    return true;
}

void Window::StopRecording()
{
    saveFileName = "";
    captureFrame = false;
    saveAsVideo = false;
    if(frame != NULL)
    {
        delete[] frame;
        frame = NULL;
    }
    if(fp != NULL)
    {
        fclose(fp);
        fp = NULL;
    }
}

static bool hasEnding (std::string const &fullString, std::string const &ending) 
{
    if (fullString.length() >= ending.length()) {
        return (0 == fullString.compare (fullString.length() - ending.length(), ending.length(), ending));
    } else {
        return false;
    }
}

void Window::CaptureFrame()
{
    if(!captureFrame)
        return;
    if(!saveAsVideo)
    {
        int sz = width * height * 4;
        if(frame == NULL)
        {
            frame = new unsigned char[sz];
            if(frame == NULL)
            {
                captureFrame = false;
                saveFileName = "";
                return;
            }
        }
        
        glReadPixels(0, 0, (int)width, (int)height, GL_RGBA, GL_UNSIGNED_BYTE, frame);
        stbi_flip_vertically_on_write(1);
        if(hasEnding(saveFileName, ".png") || hasEnding(saveFileName, ".PNG"))
            stbi_write_png(saveFileName.c_str(), (int)width, (int)height, 4, frame, 4 * (int)width);
        else if(hasEnding(saveFileName, ".bmp") || hasEnding(saveFileName, ".BMP"))
            stbi_write_bmp(saveFileName.c_str(), (int)width, (int)height, 4, frame);
        else if(hasEnding(saveFileName, ".tga") || hasEnding(saveFileName, ".TGA"))
            stbi_write_tga(saveFileName.c_str(), (int)width, (int)height, 4, frame);
        else if(hasEnding(saveFileName, ".jpg") || hasEnding(saveFileName, ".JPG") || hasEnding(saveFileName, ".jpeg") || hasEnding(saveFileName, ".JPEG"))
            stbi_write_jpg(saveFileName.c_str(), (int)width, (int)height, 4, frame, 90);
        else
            stbi_write_png(saveFileName.c_str(), (int)width, (int)height, 4, frame, 4 * (int)width);
        captureFrame = false;
        delete[] frame;
        saveFileName = "";
        frame = NULL;
    }
    else
    {
        if(frame == NULL)
        {
            int sz = width * height * 4;
            frame = new unsigned char[sz];
            if(frame == NULL)
            {
                captureFrame = false;
                saveAsVideo = false;
                saveFileName = "";
                return;
            }
        }
        glReadPixels(0, 0, (int)width, (int)height, GL_RGBA, GL_UNSIGNED_BYTE, frame);
        jo_write_mpeg(fp, frame, (int)width, (int)height, fps);
    }
}