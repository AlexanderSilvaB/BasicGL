#ifndef _BASIC_GL_KEYBOARD_H_
#define _BASIC_GL_KEYBOARD_H_

namespace BasicGL
{
    #define  KEY_F1                        0x0001
    #define  KEY_F2                        0x0002
    #define  KEY_F3                        0x0003
    #define  KEY_F4                        0x0004
    #define  KEY_F5                        0x0005
    #define  KEY_F6                        0x0006
    #define  KEY_F7                        0x0007
    #define  KEY_F8                        0x0008
    #define  KEY_F9                        0x0009
    #define  KEY_F10                       0x000A
    #define  KEY_F11                       0x000B
    #define  KEY_F12                       0x000C
    #define  KEY_LEFT                      0x0064
    #define  KEY_UP                        0x0065
    #define  KEY_RIGHT                     0x0066
    #define  KEY_DOWN                      0x0067
    #define  KEY_PAGE_UP                   0x0068
    #define  KEY_PAGE_DOWN                 0x0069
    #define  KEY_HOME                      0x006A
    #define  KEY_END                       0x006B
    #define  KEY_INSERT                    0x006C

    typedef struct Keyboard_st
    {
        int key;
        int windowX, windowY;
        float x, y;
        bool shift, ctrl, alt;
    }&Keyboard;
}

#endif
