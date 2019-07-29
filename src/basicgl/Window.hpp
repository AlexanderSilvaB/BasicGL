#ifndef _BASIC_GL_WINDOW_H_
#define _BASIC_GL_WINDOW_H_

#include "Element.hpp"
#include "Camera.hpp"
#include "Keyboard.hpp"
#include "Mouse.hpp"
#include <vector>
#include <string>
#include <time.h>

namespace BasicGL
{
    class Window;

    typedef const std::vector< Element* >& ElementsList;
    typedef Window* WindowPtr;

    typedef void (*AnimationFunction)(ElementsList elements, WindowPtr window, float ellapsedTime);
    typedef void (*KeyboardFunction)(Keyboard keyboard, WindowPtr window);
    typedef void (*MouseFunction)(Mouse mouse, WindowPtr window);

    class Window
    {
        private:
            int windowedX, windowedY, windowedWidth, windowedHeight;
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
            std::string saveFileName;
        
            std::vector< Element* > elements;
            AnimationFunction animationFunction;
            KeyboardFunction keyboardFunction;
            MouseFunction mouseFunction;

            Window();
            virtual ~Window();

            bool IsFullscreen();
            void SetFullscreen(bool enabled);
            void ToggleFullscreen();

            float ZPlane();
            ElementPtr find(const std::string& name);
            ElementPtr get(int index);
    };
}

#endif
