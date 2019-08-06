#ifndef _BASIC_GL_WINDOW_H_
#define _BASIC_GL_WINDOW_H_

#include "Element.hpp"
#include "Camera.hpp"
#include "Keyboard.hpp"
#include "Mouse.hpp"
#include <vector>
#include <string>
#include <time.h>
#include <stdio.h>

namespace BasicGL
{
    class Window;

    typedef const std::vector< ElementPtr >& ElementsList;
    typedef Window* WindowPtr;

    typedef void (*AnimationFunction)(ElementsList elements, WindowPtr window, float ellapsedTime);
    typedef void (*KeyboardFunction)(Keyboard keyboard, WindowPtr window);
    typedef void (*MouseFunction)(Mouse mouse, WindowPtr window);

    class Window
    {
        private:
            int windowedX, windowedY, windowedWidth, windowedHeight;
            std::vector< ElementPtr > assocElements;
            std::string saveFileName;
            bool saveAsVideo;
            bool captureFrame;
            int fps;
            unsigned char *frame;
            FILE *fp;
        public:
            int id;
            int index;
            int mode;
            bool opened;
            bool fullscren;
            Camera camera;
            float bg[4];
            float timeSinceBegin;
            struct timespec lastTime;
            bool cartesian;
            float width, height;
            float lastMouseX, lastMouseY;
        
            std::vector< ElementPtr > elements;
            AnimationFunction animationFunction;
            KeyboardFunction keyboardFunction;
            MouseFunction mouseFunction;

            Window();
            virtual ~Window();

            void assocElement(ElementPtr element);
            void Destroy();

            bool IsFullscreen();
            void SetFullscreen(bool enabled);
            void ToggleFullscreen();

            float ZPlane();
            ElementPtr find(const std::string& name);
            ElementPtr get(int index);

            bool Capture(const std::string& fileName);
            bool StartRecording(const std::string& fileName, int fps = 60);
            void StopRecording();
            void CaptureFrame();
    };
}

#endif
