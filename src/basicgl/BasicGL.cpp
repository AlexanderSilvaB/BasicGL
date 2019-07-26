#include "BasicGL.h"
#include <GL/freeglut.h>
#include <GL/freeglut_ext.h>
#include <GL/gl.h>
#include <iostream>
#include <cmath>
#include <unistd.h>

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

int BasicGL::CreateWindow(const string name, int mode, int width, int height, int x, int y)
{
    return CreateWindow(name.c_str(), mode, width, height, x, y);
}

void BasicGL::SelectWindow(int index)
{
    if(index >= 0 && index < windows.size())
        currentWindow = index;
}

bool BasicGL::IsOpen()
{
    return windows[currentWindow].opened;
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

void BasicGL::SetCartesian(bool cartesian)
{
    windows[currentWindow].cartesian = cartesian;
}

BasicGLElement* BasicGL::CreateElement(int element, bool addToDrawer)
{
    BasicGLElement* el = new BasicGLElement();
    el->element = element;

    switch(el->element)
    {
        case ELEMENT_POINT:
        case ELEMENT_LINE:
        case ELEMENT_TRIANGLE:
        case ELEMENT_QUAD:
            el->reshape(1);
            break;
        case ELEMENT_POLYGON:
        case ELEMENT_SEQUENCE:
            el->reshape(0);
            break;
        case ELEMENT_CIRCLE:
            el->reshape(36);
            break;
        default:
            el->reshape(0);
            break;
    }
    
    switch(el->element)
    {
        case ELEMENT_POINT:
            el->point(0, 0, 0);
            el->glow();
            break;
        case ELEMENT_LINE:
            el->line(-0.5f, 0, 0, 0.5f, 0, 0);
            el->glow();
            break;
        case ELEMENT_TRIANGLE:
            el->triangle(0, -0.5f, 0, -0.5f, 0.5f, 0, 0.5f, 0.5f, 0);
            el->glow();
            break;
        case ELEMENT_QUAD:
            el->quad(-0.5f, -0.5f, 0, 0.5f, 0.5f, 0);
            el->glow();
            break;
        case ELEMENT_POLYGON:
            break;
        case ELEMENT_SEQUENCE:
            break;
        case ELEMENT_CIRCLE:
            el->circle(0, 0, 0, 0.5f);
            el->glow();
            break;
        default:
            break;
    }

    if(addToDrawer)
        windows[currentWindow].elements.push_back(el);

    return el;
}

BasicGLPlot* BasicGL::CreatePlot(int rows, int cols, int index)
{
    BasicGLPlot* plt = new BasicGLPlot();
    plt->element = ELEMENT_CONTAINER;
    plt->rows = rows;
    plt->cols = cols;

    if(index <= 0)
        index = 1;
        
    index--;

    plt->index = index;

    int iY = index / rows;
    int iX = index % rows;
    

    plt->minX = -1.0f + (2.0f / cols) * iX;
    plt->maxX = plt->minX + (2.0f / cols);

    plt->minY = -1.0f + (2.0f / rows) * iY;
    plt->maxY = plt->minY + (2.0f / rows);

    BasicGLElement *bg = CreateElement(ELEMENT_QUAD);
    float ratioX = (plt->maxX - plt->minX);
    float ratioY = (plt->maxY - plt->minY);
    bg->scaleTo(0.95 * ratioX, 0.95 * ratioY, 1.0f);
    bg->moveTo(0.5f * (plt->minX + plt->maxX), 0.5f * (plt->minY + plt->maxY), 0);
    bg->rgb(1.0f, 1.0f, 1.0f);

    BasicGLElement *lines = CreateElement(ELEMENT_LINE);
    lines->reshape(2);
    lines->line(-0.5, 0.5, -0.5, -0.5, 0);
    lines->line(-0.5, -0.5, 0.5, -0.5, 1);
    lines->rgb(0.0f, 0.0f, 0.0f);
    lines->scaleTo(0.91f * ratioX, 0.91f * ratioY, 1.0f);
    lines->moveTo(0.5f * (plt->minX + plt->maxX), 0.5f * (plt->minY + plt->maxY), 0);

    plt->assoc = lines;
    lines->assoc = bg;  

    //windows[currentWindow].elements.push_back(bg);
    //windows[currentWindow].elements.push_back(lines);
    windows[currentWindow].elements.push_back(plt);

    return plt;
}

void BasicGL::SetAnimationFunction(AnimationFunction animationFunction)
{
    windows[currentWindow].animationFunction = animationFunction;
}

void BasicGL::SetKeyboardFunction(KeyboardFunction keyboardFunction)
{
    windows[currentWindow].keyboardFunction = keyboardFunction;
}

void BasicGL::Show()
{
    glutMainLoop();
}

void BasicGL::Pause(float seconds)
{
    glutMainLoopEvent();
    Render();
    usleep((useconds_t)(seconds * 1000000));
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

void BasicGL::Closed()
{
    int index = WindowIndex();
    if(index < 0)
        return;
    
    BasicGLWindow &window = windows[index];
    window.opened = false;
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
    if(!visible)
        return;
    if(assoc != NULL)
    {
        glPushMatrix();
        assoc->draw();
        glPopMatrix();
    }
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
            n = size;
            t = GL_LINES;
            break;
        case ELEMENT_TRIANGLE:
            n = 3;
            if(wireframe)
                t = GL_LINE_LOOP;
            else
                t = GL_TRIANGLES;
            break;
        case ELEMENT_QUAD:
            n = 4;
            if(wireframe)
                t = GL_LINE_LOOP;
            else
                t = GL_QUADS;
            break;
        case ELEMENT_POLYGON:
            n = size;
            if(wireframe)
                t = GL_LINE_LOOP;
            else
                t = GL_POLYGON;
            break;
        case ELEMENT_SEQUENCE:
            n = size;
            t = GL_LINE_STRIP;
            break;
        case ELEMENT_CIRCLE:
            n = size;
            if(wireframe)
                t = GL_LINE_LOOP;
            else
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



    glLineWidth(lineWidth);
    glTranslatef(position[0], position[1], position[2]);
    glScalef(scales[0], scales[1], scales[2]);
    glRotatef(57.2958f * rotation[0], 1.0f, 0.0f, 0.0f);
    glRotatef(57.2958f * rotation[1], 0.0f, 1.0f, 0.0f);
    glRotatef(57.2958f * rotation[2], 0.0f, 0.0f, 1.0f);
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
                if(wireframe)
                    glutWireSphere(1.0f, 20, 20);
                else
                    glutSolidSphere(1.0f, 20, 20);
                break;
            case ELEMENT_CONE:
                if(wireframe)
                    glutWireCone(0.5f, 1.0f, 20, 20);
                else
                    glutSolidCone(0.5f, 1.0f, 20, 20);
                break;
            case ELEMENT_CUBE:
                if(wireframe)
                    glutWireCube(1.0f);
                else
                    glutSolidCube(1.0f);
                break;
            case ELEMENT_CYLINDER:
                if(wireframe)
                    glutWireCylinder(0.5f, 1.0f, 20, 20);
                else
                    glutSolidCylinder(0.5f, 1.0f, 20, 20);
                break;
            case ELEMENT_TEAPOT:
                if(wireframe)
                    glutWireTeapot(1.0f);
                else
                    glutSolidTeapot(1.0f);
                break;
            /*
            case ELEMENT_CONTAINER:
            {
                for(list<BasicGLElement*>::iterator i = elements.begin(); i != elements.end(); i++)
                {
                    if(dynamic_cast<BasicGLPlot*>(this) && dynamic_cast<BasicGLPlotSerie*>(*i))
                    {
                        BasicGLPlot *plt = (BasicGLPlot*) this;

                        BasicGLPlotSerie *serie = (BasicGLPlotSerie*) *i;
                        if(!serie->aligned)
                        {
                            if(plt->fixedAxis)
                            {
                                serie->minX = plt->axisMinX;
                                serie->maxX = plt->axisMaxX;
                                serie->minY = plt->axisMinY;
                                serie->maxY = plt->axisMaxY;
                            }
                            serie->align(plt->minX, plt->maxX, plt->minY, plt->maxY, serie->color);
                            serie->aligned = true;
                        }
                    }
                    glPushMatrix();
                    (*i)->draw();
                    glPopMatrix();
                }
            }
                break;
            */
            default:
                break;
        }   
    }
    glColor4fv(color);
    for(list<BasicGLElement*>::iterator i = elements.begin(); i != elements.end(); i++)
    {
        if(dynamic_cast<BasicGLPlot*>(this) && dynamic_cast<BasicGLPlotSerie*>(*i))
        {
            BasicGLPlot *plt = (BasicGLPlot*) this;

            BasicGLPlotSerie *serie = (BasicGLPlotSerie*) *i;
            if(!serie->aligned)
            {
                if(plt->fixedAxis)
                {
                    serie->minX = plt->axisMinX;
                    serie->maxX = plt->axisMaxX;
                    serie->minY = plt->axisMinY;
                    serie->maxY = plt->axisMaxY;
                }
                serie->align(plt->minX, plt->maxX, plt->minY, plt->maxY, serie->color);
                serie->aligned = true;
            }
        }
        glPushMatrix();
        (*i)->draw();
        glPopMatrix();
    }
}

BasicGLElement* BasicGLElement::CreateElement(int element)
{
    BasicGLElement* el = BasicGL::CreateElement(element, false);
    elements.push_back(el);
    return el;
}

// Plot
void getColor(const string opts, float *color)
{
    color[0] = color[1] = color[2] = 0.0f;
    color[3] = 1.0f;
    for(int i = 0; i < opts.size(); i++)
    {
        switch (opts[i])
        {
            case 'k':
            case 'K':
                color[0] = color[1] = color[2] = 0.0f;
                break;
            case 'r':
            case 'R':
                color[0] = 1.0f;
                color[1] = color[2] = 0.0f;
                break;
            case 'g':
            case 'G':
                color[1] = 1.0f;
                color[0] = color[2] = 0.0f;
                break;
            case 'b':
            case 'B':
                color[2] = 1.0f;
                color[0] = color[1] = 0.0f;
                break;
            case 'w':
            case 'W':
                color[0] = color[1] = color[2] = 1.0f;
                break;
            case 'm':
            case 'M':
                color[1] = 0.0f;
                color[0] = color[2] = 1.0f;
                break;
            case 'c':
            case 'C':
                color[0] = 0.0f;
                color[1] = color[2] = 1.0f;
                break;
            default:
                break;
        }
    }
}

BasicGLPlotSerie* BasicGLPlot::createSerie(const string opts)
{
    BasicGLPlotSerie* serie = new BasicGLPlotSerie();
    serie->element = ELEMENT_SEQUENCE;
    elements.push_back(serie);
    getColor(opts, serie->color);
    return serie;
}

BasicGLPlotSerie* BasicGLPlot::plot(vector<float> &x, vector<float> &y, const string opts)
{
    BasicGLPlotSerie* serie = createSerie(opts);
    serie->update(x, y);
    return serie;
}

BasicGLPlot* BasicGLPlot::axis(float minX, float maxX, float minY, float maxY)
{
    this->fixedAxis = true;
    this->axisMinX = minX;
    this->axisMaxX = maxX;
    this->axisMinY = minY;
    this->axisMaxY = maxY;
    for(list<BasicGLElement*>::iterator i = elements.begin(); i != elements.end(); i++)
    {
        if(dynamic_cast<BasicGLPlotSerie*>(*i))
        {
            BasicGLPlotSerie *serie = (BasicGLPlotSerie*) *i;
            serie->aligned = false;
        }
    }
    return this;
}

BasicGLPlotSerie* BasicGLPlotSerie::add(float x, float y)
{
    BasicGLPoint pt;
    pt.moveTo(x, y, 0);
    pt.rgb(color[0], color[1], color[2], color[3]);
    points.push_back(pt);
    size++;
    if(size == 1)
    {
        minX = maxX = x;
        minY = maxY = y;
    }
    else
    {
        if(x < minX)
            minX = x;
        if(x > maxX)
            maxX = x;
        if(y < minY)
            minY = y;
        if(y > maxY)
            maxY = y;
    }
    aligned = false;
    return this;
}

BasicGLPlotSerie* BasicGLPlotSerie::update(vector<float> &x, vector<float> &y)
{
    int sz = min(x.size(), y.size());
    reshape(sz);
    for(int i = 0; i < sz; i++)
    {
        points[i].moveTo(x[i], y[i], 0);
        points[i].rgb(color[0], color[1], color[2], color[3]);
        if(i == 0)
        {
            minX = maxX = x[i];
            minY = maxY = y[i];
        }
        else
        {
            if(x[i] < minX)
                minX = x[i];
            if(x[i] > maxX)
                maxX = x[i];
            if(y[i] < minY)
                minY = y[i];
            if(y[i] > maxY)
                maxY = y[i];
        }
    }
    aligned = false;
    return this;
}

void BasicGLPlotSerie::align(float minX, float maxX, float minY, float maxY, float *color)
{
    float ratioX = (maxX - minX) / (this->maxX - this->minX);
    float ratioY = (maxY - minY) / (this->maxY - this->minY);
    float sx = ratioX * 0.9;
    float sy = ratioY * 0.9;
    float x = 0.5f * (minX + maxX) - (0.5f * (this->minX + this->maxX)) * sx;
    float y = 0.5f * (minY + maxY) - (0.5f * (this->minY + this->maxY)) * sy;
    moveTo(x, y, 0);
    scaleTo(sx, sy, 1.0f);
}