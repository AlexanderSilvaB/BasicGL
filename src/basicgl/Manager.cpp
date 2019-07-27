#include "Manager.hpp"
#include <GL/freeglut.h>
#include <GL/freeglut_ext.h>
#include <GL/gl.h>
#include <iostream>
#include <cmath>
#include <unistd.h>

using namespace BasicGL;
using namespace std;

vector< Window > Manager::windows;
int Manager::currentWindow = -1;

void Manager::now(struct timespec &t)
{
    clock_gettime( CLOCK_REALTIME, &t);
}

void Manager::Init(int argc, char *argv[])
{
    glutInit(&argc, argv);  
}

int Manager::CreateWindow(const char *name, Modes mode, int width, int height, int x, int y)
{
    unsigned int _mode = GLUT_DOUBLE | GLUT_RGB;
    if(mode == MODE_3D)
        _mode |= GLUT_DEPTH;
    glutInitDisplayMode( _mode ); 

    if(width > 0 && height > 0)
        glutInitWindowSize( width, height );
    if(x >= 0 && y >= 0)
        glutInitWindowPosition( x, y );
    int _window = glutCreateWindow( name );

    currentWindow = windows.size();
    Window window;

    window.id = _window;
    window.index = currentWindow;
    window.mode = mode;
    window.timeSinceBegin = 0;
    window.opened = true;
    windows.push_back(window);

    glutIdleFunc(Render);
    glutReshapeFunc(Resize);
    glutKeyboardFunc(KeyboardNormal);
    glutSpecialFunc(KeyboardSpecial);
    glutMouseFunc(MouseButtons);
    glutMotionFunc(MouseMotion);
    glutEntryFunc(MouseEntry);
    glutTimerFunc(33, Timer, currentWindow);
    glutCloseFunc(Closed);

    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    //const float lightPos[4] = {1, .5, 1, 0};
    //glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glEnable(GL_DEPTH_TEST);
}

int Manager::CreateWindow(const string name, Modes mode, int width, int height, int x, int y)
{
    return CreateWindow(name.c_str(), mode, width, height, x, y);
}

void Manager::SelectWindow(int index)
{
    if(index >= 0 && index < windows.size())
        currentWindow = index;
}

bool Manager::IsOpen()
{
    return windows[currentWindow].opened;
}

void Manager::SetBackground(float r, float g, float b, float a)
{
    windows[currentWindow].bg[0] = r;
    windows[currentWindow].bg[1] = g;
    windows[currentWindow].bg[2] = b;
    windows[currentWindow].bg[3] = a;
}

void Manager::SetBackground(int r, int g, int b, int a)
{
    SetBackground(r/255.0f, g/255.0f, b/255.0f, a/255.0f);
}

void Manager::SetCartesian(bool cartesian)
{
    windows[currentWindow].cartesian = cartesian;
}

Element* Manager::CreateElement(Elements element, bool addToDrawer)
{
    Element* el = new Element(element);
    if(addToDrawer)
        windows[currentWindow].elements.push_back(el);
    return el;
}

Plot* Manager::CreatePlot(int rows, int cols, int index)
{
    Plot* plt = new Plot(rows, cols, index);
    windows[currentWindow].elements.push_back(plt);
    return plt;
}

void Manager::SetAnimationFunction(AnimationFunction animationFunction)
{
    windows[currentWindow].animationFunction = animationFunction;
}

void Manager::SetKeyboardFunction(KeyboardFunction keyboardFunction)
{
    windows[currentWindow].keyboardFunction = keyboardFunction;
}

void Manager::Show()
{
    glutMainLoop();
}

void Manager::Pause(float seconds)
{
    glutMainLoopEvent();
    Render();
    if(seconds > 0)
        usleep((useconds_t)(seconds * 1000000));
}

float Manager::WindowWidth()
{
    return windows[currentWindow].width;
}

float Manager::WindowHeight()
{
    return windows[currentWindow].height;
}

int Manager::WindowIndex()
{
    int id = glutGetWindow();
    int index = -1;
    for(int i = 0; i < windows.size(); i++)
    {
        if(windows[i].id == id)
        {
            index = i;
            break;
        }
    }
    return index;
}

void Manager::Resize(GLsizei w, GLsizei h)
{
    int index = WindowIndex();
    if(index < 0)
        return;
    
    Window& window = windows[index];
    window.width = w;
    window.height = h;
    float flipY = window.cartesian ? -1.0f : 1.0f;

    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    GLfloat fAspect = (GLfloat)w/(GLfloat)h;
    if(window.mode == MODE_3D)
    {
        GLfloat angle = 45;
        gluPerspective(angle, fAspect, 1, 100);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
    }
    else
    {

        gluOrtho2D (-1.0f*fAspect, 1.0f*fAspect*flipY, 1.0f, -1.0f*flipY);
    }
}

void Manager::KeyboardNormal(unsigned char key, int x, int y)
{
    int index = WindowIndex();
    if(index < 0)
        return;
    
    Window& window = windows[index];

    struct Keyboard_st keyboard;
    keyboard.key = key;
    keyboard.x = x;
    keyboard.y = y;
    int mod = glutGetModifiers();
    keyboard.shift = (mod & GLUT_ACTIVE_SHIFT) > 0;
    keyboard.alt = (mod & GLUT_ACTIVE_ALT) > 0;
    keyboard.ctrl = (mod & GLUT_ACTIVE_CTRL) > 0;

    if(window.keyboardFunction != NULL)
        window.keyboardFunction(keyboard, &window);
}

void Manager::KeyboardSpecial(int key, int x, int y)
{
    int index = WindowIndex();
    if(index < 0)
        return;
    
    Window& window = windows[index];

    struct Keyboard_st keyboard;
    keyboard.key = key;
    keyboard.x = x;
    keyboard.y = y;
    int mod = glutGetModifiers();
    keyboard.shift = (mod & GLUT_ACTIVE_SHIFT) > 0;
    keyboard.alt = (mod & GLUT_ACTIVE_ALT) > 0;
    keyboard.ctrl = (mod & GLUT_ACTIVE_CTRL) > 0;

    if(window.keyboardFunction != NULL)
        window.keyboardFunction(keyboard, &window);
}

void Manager::MouseButtons(int button, int state, int x, int y)
{
    int index = WindowIndex();
    if(index < 0)
        return;
    
    Window& window = windows[index];

    struct Mouse_st mouse;
    mouse.left = button == GLUT_LEFT_BUTTON;
    mouse.middle = button == GLUT_MIDDLE_BUTTON;
    mouse.right = button == GLUT_RIGHT_BUTTON;
    mouse.x = x;
    mouse.y = y;
    mouse.pressed = state == GLUT_DOWN;
    mouse.released = state == GLUT_UP;
    mouse.move = false;
    mouse.entered = false;
    mouse.leave = false;
    if(window.mouseFunction != NULL)
        window.mouseFunction(mouse, &window);
}

void Manager::MouseMotion(int x, int y)
{
    int index = WindowIndex();
    if(index < 0)
        return;
    
    Window& window = windows[index];

    struct Mouse_st mouse;
    mouse.left = false;
    mouse.middle = false;
    mouse.right = false;
    mouse.x = x;
    mouse.y = y;
    mouse.pressed = false;
    mouse.released = false;
    mouse.move = true;
    mouse.entered = false;
    mouse.leave = false;
    if(window.mouseFunction != NULL)
        window.mouseFunction(mouse, &window);
}

void Manager::MouseEntry(int state)
{
    int index = WindowIndex();
    if(index < 0)
        return;
    
    Window& window = windows[index];

    struct Mouse_st mouse;
    mouse.left = false;
    mouse.middle = false;
    mouse.right = false;
    mouse.x = 0;
    mouse.y = 0;
    mouse.pressed = false;
    mouse.released = false;
    mouse.move = false;
    mouse.entered = state == GLUT_ENTERED;
    mouse.leave = state == GLUT_LEFT;
    if(window.mouseFunction != NULL)
        window.mouseFunction(mouse, &window);
}

void Manager::Timer(int index)
{
    Window& window = windows[index];

    struct timespec currentTime;
    now(currentTime);
    float secs = ( currentTime.tv_sec - window.lastTime.tv_sec )
          + ( ((float) currentTime.tv_nsec - window.lastTime.tv_nsec )
            / (float)1000000000L);

    if(secs > 1.0f)
        secs = 1.0f;
    window.timeSinceBegin += secs;

    if(window.animationFunction != NULL)
        window.animationFunction(window.elements, &window, secs);

    now(window.lastTime);

    glutTimerFunc(33, Timer, index);
}

void Manager::Closed()
{
    int index = WindowIndex();
    if(index < 0)
        return;
    
    Window& window = windows[index];
    window.opened = false;
}

void Manager::Render()
{
    int index = WindowIndex();
    if(index < 0)
        return;
    
    Window& window = windows[index];

    int mode = GL_COLOR_BUFFER_BIT;
    if(window.mode == MODE_3D)
        mode |= GL_DEPTH_BUFFER_BIT;
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glClearColor(window.bg[0], window.bg[1], window.bg[2], window.bg[3]);
    glClear(mode);

    if(window.mode == MODE_3D)
    {
        gluLookAt(  window.camera.position[0],                                  window.camera.position[1],                                  window.camera.position[2],
                    window.camera.position[0] + window.camera.translation[0],   window.camera.position[1] + window.camera.translation[1],   window.camera.position[2] + window.camera.translation[2],
                    0.0f, 1.0f, 0.0f);
    }

    for(int i = 0; i < window.elements.size(); i++)
    {
        glPushMatrix();
        window.elements[i]->draw();
        glPopMatrix();
    }

    glutSwapBuffers();
}