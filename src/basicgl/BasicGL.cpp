#include "BasicGL.h"
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <iostream>

using namespace std;

vector<BasicGLWindow> BasicGL::windows;
int BasicGL::currentWindow = -1;

void now(struct timespec &t)
{
    clock_gettime( CLOCK_REALTIME, &t);
}

void BasicGL::Init(int argc, char *argv[])
{
    glutInit(&argc, argv);  
}

int BasicGL::CreateWindow(const char *name, int mode, int width, int height, int x, int y)
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
    BasicGLWindow window;
    window.id = _window;
    window.index = currentWindow;
    window.mode = mode;
    window.timeSinceBegin = 0;
    windows.push_back(window);

    glutIdleFunc(Render);
    glutReshapeFunc(Resize);
    glutKeyboardFunc(KeyboardNormal);
    glutSpecialFunc(KeyboardSpecial);
    glutMouseFunc(MouseButtons);
    glutMotionFunc(MouseMotion);
    glutEntryFunc(MouseEntry);
    glutTimerFunc(33, Timer, currentWindow);

    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    //const float lightPos[4] = {1, .5, 1, 0};
    //glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glEnable(GL_DEPTH_TEST);
}

int BasicGL::CreateWindow(const string name, int mode, int width, int height, int x, int y)
{
    return CreateWindow(name.c_str(), mode, width, height, x, y);
}

void BasicGL::SelectWindow(int index)
{
    if(index >= 0 && index < windows.size())
        currentWindow = index;
}

void BasicGL::SetBackground(float r, float g, float b, float a)
{
    windows[currentWindow].bg[0] = r;
    windows[currentWindow].bg[1] = g;
    windows[currentWindow].bg[2] = b;
    windows[currentWindow].bg[3] = a;
}

void BasicGL::SetBackground(int r, int g, int b, int a)
{
    SetBackground(r/255.0f, g/255.0f, b/255.0f, a/255.0f);
}

BasicGLElement* BasicGL::CreateElement(int element)
{
    BasicGLElement* el = new BasicGLElement();
    el->element = element;

    switch(el->element)
    {
        case ELEMENT_POINT:
            el->reshape(1);
            break;
        case ELEMENT_LINE:
            el->reshape(2);
            break;
        case ELEMENT_TRIANGLE:
            el->reshape(3);
            break;
        case ELEMENT_QUAD:
            el->reshape(4);
            break;
        case ELEMENT_POLYGON:
            el->reshape(0);
            break;
        default:
            el->reshape(0);
            break;
    }
    
    switch(el->element)
    {
        case ELEMENT_POINT:
            el->points[0].xyz[0] = 0;
            el->points[0].xyz[1] = 0;
            el->points[0].xyz[2] = 0;
            break;
        case ELEMENT_LINE:
            el->points[0].xyz[0] = -0.5f;
            el->points[0].xyz[1] = 0;
            el->points[0].xyz[2] = 0;

            el->points[1].xyz[0] = 0.5f;
            el->points[1].xyz[1] = 0;
            el->points[1].xyz[2] = 0;

            el->points[0].color[1] = el->points[0].color[2] = 0.0;
            el->points[1].color[0] = el->points[1].color[2] = 0.0;
            break;
        case ELEMENT_TRIANGLE:
            el->points[0].xyz[0] = 0;
            el->points[0].xyz[1] = -0.5f;
            el->points[0].xyz[2] = 0;

            el->points[1].xyz[0] = -0.5f;
            el->points[1].xyz[1] = 0.5f;
            el->points[1].xyz[2] = 0;

            el->points[2].xyz[0] = 0.5f;
            el->points[2].xyz[1] = 0.5f;
            el->points[2].xyz[2] = 0;

            el->points[0].color[1] = el->points[0].color[2] = 0.0;
            el->points[1].color[0] = el->points[1].color[2] = 0.0;
            el->points[2].color[0] = el->points[2].color[1] = 0.0;
            break;
        case ELEMENT_QUAD:
            el->points[0].xyz[0] = -0.5f;
            el->points[0].xyz[1] = -0.5f;
            el->points[0].xyz[2] = 0;

            el->points[1].xyz[0] = 0.5f;
            el->points[1].xyz[1] = -0.5f;
            el->points[1].xyz[2] = 0;

            el->points[2].xyz[0] = 0.5f;
            el->points[2].xyz[1] = 0.5f;
            el->points[2].xyz[2] = 0;

            el->points[3].xyz[0] = -0.5f;
            el->points[3].xyz[1] = 0.5f;
            el->points[3].xyz[2] = 0;

            el->points[0].color[1] = el->points[0].color[2] = 0.0;
            el->points[1].color[0] = el->points[1].color[2] = 0.0;
            el->points[2].color[0] = el->points[2].color[1] = 0.0;
            break;
        case ELEMENT_POLYGON:
            break;
        default:
            break;
    }

    windows[currentWindow].elements.push_back(el);

    return el;
}

void BasicGL::SetAnimationFunction(AnimationFunction animationFunction)
{
    windows[currentWindow].animationFunction = animationFunction;
}

void BasicGL::SetKeyboardFunction(KeyboardFunction keyboardFunction)
{
    windows[currentWindow].keyboardFunction = keyboardFunction;
}

void BasicGL::Run()
{
    glutMainLoop();
}

int BasicGL::WindowIndex()
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

void BasicGL::Resize(GLsizei w, GLsizei h)
{
    int index = WindowIndex();
    if(index < 0)
        return;
    
    BasicGLWindow &window = windows[index];

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

        gluOrtho2D (-1.0f*fAspect, 1.0f*fAspect, 1.0f, -1.0f);
    }
}

void BasicGL::KeyboardNormal(unsigned char key, int x, int y)
{
    int index = WindowIndex();
    if(index < 0)
        return;
    
    BasicGLWindow &window = windows[index];

    struct BasicGLKeyboard_st keyboard;
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

void BasicGL::KeyboardSpecial(int key, int x, int y)
{
    int index = WindowIndex();
    if(index < 0)
        return;
    
    BasicGLWindow &window = windows[index];

    struct BasicGLKeyboard_st keyboard;
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

void BasicGL::MouseButtons(int button, int state, int x, int y)
{
    int index = WindowIndex();
    if(index < 0)
        return;
    
    BasicGLWindow &window = windows[index];

    struct BasicGLMouse_st mouse;
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

void BasicGL::MouseMotion(int x, int y)
{
    int index = WindowIndex();
    if(index < 0)
        return;
    
    BasicGLWindow &window = windows[index];

    struct BasicGLMouse_st mouse;
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

void BasicGL::MouseEntry(int state)
{
    int index = WindowIndex();
    if(index < 0)
        return;
    
    BasicGLWindow &window = windows[index];

    struct BasicGLMouse_st mouse;
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

void BasicGL::Timer(int index)
{
    BasicGLWindow &window = windows[index];

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

void BasicGL::Render()
{
    int index = WindowIndex();
    if(index < 0)
        return;
    
    BasicGLWindow &window = windows[index];

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


// Elements
void BasicGLElement::draw()
{
    int n = 0;
    bool withBegin = true;
    GLenum t = GL_POINTS;
    switch(element)
    {
        case ELEMENT_POINT:
            n = 1;
            t = GL_POINTS;
            glPointSize(scales[0]);
            break;
        case ELEMENT_LINE:
            n = 2;
            t = GL_LINES;
            break;
        case ELEMENT_TRIANGLE:
            n = 3;
            t = GL_TRIANGLES;
            break;
        case ELEMENT_QUAD:
            n = 4;
            t = GL_QUADS;
            break;
        case ELEMENT_POLYGON:
            n = size;
            t = GL_POLYGON;
            break;
        case ELEMENT_SPHERE:
        case ELEMENT_CONE:
        case ELEMENT_CUBE:
        case ELEMENT_CYLINDER:
        case ELEMENT_TEAPOT:
            withBegin = false;
            break;
        case ELEMENT_CONTAINER:
            withBegin = false;
            break;
        default:
            break;
    }


    glTranslatef(position[0], position[1], position[2]);
    glScalef(scales[0], scales[1], scales[2]);
    glRotatef(360.0f * rotation[0], 1.0f, 0.0f, 0.0f);
    glRotatef(360.0f * rotation[1], 0.0f, 1.0f, 0.0f);
    glRotatef(360.0f * rotation[2], 0.0f, 0.0f, 1.0f);
    if(withBegin)
    {
        glBegin(t);
        for(int i = 0; i < n; i++)
        {
            glColor4fv(points[i].color);
            glVertex3f(points[i].xyz[0], points[i].xyz[1], points[i].xyz[2]);
        }
        glEnd();
    }
    else
    {
        glColor4fv(color);
        switch(element)
        {
            case ELEMENT_SPHERE:
                if(wirefrane)
                    glutWireSphere(1.0f, 20, 20);
                else
                    glutSolidSphere(1.0f, 20, 20);
                break;
            case ELEMENT_CONE:
                if(wirefrane)
                    glutWireCone(0.5f, 1.0f, 20, 20);
                else
                    glutSolidCone(0.5f, 1.0f, 20, 20);
                break;
            case ELEMENT_CUBE:
                if(wirefrane)
                    glutWireCube(1.0f);
                else
                    glutSolidCube(1.0f);
                break;
            case ELEMENT_CYLINDER:
                if(wirefrane)
                    glutWireCylinder(0.5f, 1.0f, 20, 20);
                else
                    glutSolidCylinder(0.5f, 1.0f, 20, 20);
                break;
            case ELEMENT_TEAPOT:
                if(wirefrane)
                    glutWireTeapot(1.0f);
                else
                    glutSolidTeapot(1.0f);
                break;
            case ELEMENT_CONTAINER:
            {
                for(list<BasicGLElement*>::iterator i = elements.begin(); i != elements.end(); i++)
                {
                    glPushMatrix();
                    (*i)->draw();
                    glPopMatrix();
                }
            }
                break;
            default:
                break;
        }   
    }
}

BasicGLElement* BasicGLElement::CreateElement(int element)
{
    BasicGLElement* el = new BasicGLElement();
    el->element = element;

    switch(el->element)
    {
        case ELEMENT_POINT:
            el->reshape(1);
            break;
        case ELEMENT_LINE:
            el->reshape(2);
            break;
        case ELEMENT_TRIANGLE:
            el->reshape(3);
            break;
        case ELEMENT_QUAD:
            el->reshape(4);
            break;
        case ELEMENT_POLYGON:
            el->reshape(0);
            break;
        default:
            el->reshape(0);
            break;
    }
    
    switch(el->element)
    {
        case ELEMENT_POINT:
            el->points[0].xyz[0] = 0;
            el->points[0].xyz[1] = 0;
            el->points[0].xyz[2] = 0;
            break;
        case ELEMENT_LINE:
            el->points[0].xyz[0] = -0.5f;
            el->points[0].xyz[1] = 0;
            el->points[0].xyz[2] = 0;

            el->points[1].xyz[0] = 0.5f;
            el->points[1].xyz[1] = 0;
            el->points[1].xyz[2] = 0;

            el->points[0].color[1] = el->points[0].color[2] = 0.0;
            el->points[1].color[0] = el->points[1].color[2] = 0.0;
            break;
        case ELEMENT_TRIANGLE:
            el->points[0].xyz[0] = 0;
            el->points[0].xyz[1] = -0.5f;
            el->points[0].xyz[2] = 0;

            el->points[1].xyz[0] = -0.5f;
            el->points[1].xyz[1] = 0.5f;
            el->points[1].xyz[2] = 0;

            el->points[2].xyz[0] = 0.5f;
            el->points[2].xyz[1] = 0.5f;
            el->points[2].xyz[2] = 0;

            el->points[0].color[1] = el->points[0].color[2] = 0.0;
            el->points[1].color[0] = el->points[1].color[2] = 0.0;
            el->points[2].color[0] = el->points[2].color[1] = 0.0;
            break;
        case ELEMENT_QUAD:
            el->points[0].xyz[0] = -0.5f;
            el->points[0].xyz[1] = -0.5f;
            el->points[0].xyz[2] = 0;

            el->points[1].xyz[0] = 0.5f;
            el->points[1].xyz[1] = -0.5f;
            el->points[1].xyz[2] = 0;

            el->points[2].xyz[0] = 0.5f;
            el->points[2].xyz[1] = 0.5f;
            el->points[2].xyz[2] = 0;

            el->points[3].xyz[0] = -0.5f;
            el->points[3].xyz[1] = 0.5f;
            el->points[3].xyz[2] = 0;

            el->points[0].color[1] = el->points[0].color[2] = 0.0;
            el->points[1].color[0] = el->points[1].color[2] = 0.0;
            el->points[2].color[0] = el->points[2].color[1] = 0.0;
            break;
        case ELEMENT_POLYGON:
            break;
        default:
            break;
    }

    elements.push_back(el);

    return el;
}