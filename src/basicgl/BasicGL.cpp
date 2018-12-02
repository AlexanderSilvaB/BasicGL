#include "BasicGL.h"
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <iostream>

using namespace std;

vector<BasicGLWindow> BasicGL::windows;
int BasicGL::currentWindow = -1;

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

    glutDisplayFunc(Render);
    glutReshapeFunc(Resize);
    glutTimerFunc(33, Timer, currentWindow);
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

void BasicGL::Run()
{
    glutMainLoop();
}

void BasicGL::Resize(GLsizei w, GLsizei h)
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
    if(index < 0)
        return;
    
    BasicGLWindow &window = windows[index];

    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    if(window.mode == MODE_3D)
    {
        GLfloat fAspect = (GLfloat)w/(GLfloat)h;
        GLfloat angle = 0;
        gluPerspective(angle, fAspect, 0.1, 500);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(0,0,200, 0,0,0, 0,1,0);
    }
    else
        gluOrtho2D (-1.0f, 1.0f, -1.0f, 1.0f);
}

void BasicGL::Timer(int index)
{
    BasicGLWindow &window = windows[index];
    window.timeSinceBegin += 0.033f;

    if(window.animationFunction != NULL)
        window.animationFunction(window.elements, &window, 0.033f);

    glutPostRedisplay();
    glutTimerFunc(33, Timer, index);
}

void BasicGL::Render()
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

    for(int i = 0; i < window.elements.size(); i++)
    {
        BasicGLElement *element = window.elements[i];
        int n = 0;
        GLenum t = GL_POINTS;
        switch(element->element)
        {
            case ELEMENT_POINT:
                n = 1;
                t = GL_POINTS;
                glPointSize(element->scales[0]);
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
                n = element->size;
                t = GL_POLYGON;
                break;
            default:
                break;
        }

        glTranslatef(element->position[0], element->position[1], element->position[2]);
        glScalef(element->scales[0], element->scales[1], element->scales[2]);
        glRotatef(360.0f * element->rotation[0], 1.0f, 0.0f, 0.0f);
        glRotatef(360.0f * element->rotation[1], 0.0f, 1.0f, 0.0f);
        glRotatef(360.0f * element->rotation[2], 0.0f, 0.0f, 1.0f);
        glBegin(t);
        for(int i = 0; i < n; i++)
        {
            glColor4fv(element->points[i].color);
            glVertex3f(element->points[i].xyz[0], element->points[i].xyz[1], element->points[i].xyz[2]);
        }
        glEnd();
    }

    glutSwapBuffers();
}