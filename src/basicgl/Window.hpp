#ifndef _BASIC_GL_WINDOW_H_
#define _BASIC_GL_WINDOW_H_

#include "Element.hpp"
#include "Camera.hpp"
#include "Keyboard.hpp"
#include "Mouse.hpp"
#include <vector>
#include <time.h>

namespace BasicGL
{
    class Window;

    typedef const std::vector< Element* >& ElementsList;
    typedef const Window* WindowPtr;

    typedef void (*AnimationFunction)(ElementsList elements, WindowPtr window, float ellapsedTime);
    typedef void (*KeyboardFunction)(Keyboard keyboard, WindowPtr window);
    typedef void (*MouseFunction)(Mouse mouse, WindowPtr window);

    class Window
    {
        public:
            int id;
            int index;
            int mode;
            bool opened;
            Camera camera;
            float bg[4];
            float timeSinceBegin;
            struct timespec lastTime;
            bool cartesian;
        
            std::vector< Element* > elements;
            AnimationFunction animationFunction;
            KeyboardFunction keyboardFunction;
            MouseFunction mouseFunction;

            Window();
            virtual ~Window();
    };
}

#endif
