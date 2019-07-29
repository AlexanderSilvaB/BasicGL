#ifndef _BASIC_GL_MOUSE_H_
#define _BASIC_GL_MOUSE_H_

namespace BasicGL
{
    typedef struct Mouse_st
    {
        bool left, middle, right;
        bool pressed, released, move, entered, leave;
        int windowX, windowY;
        float x, y, dx, dy;
        float scroll;
    }&Mouse;
}

#endif
