#ifndef _SIMPLE_GL_H_
#define _SIMPLE_GL_H_

#include <string>
#include <vector>
#include <cfloat>
#include <cstdio>
#include <cstring>
#include <cmath>
#include <time.h>
#include <list>

#define MODE_2D 0
#define MODE_3D 1

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

#define ELEMENT_CONTAINER 0
#define ELEMENT_POINT 1
#define ELEMENT_LINE 2
#define ELEMENT_POLYGON 3
#define ELEMENT_TRIANGLE 4
#define ELEMENT_QUAD 5
#define ELEMENT_SPHERE 6
#define ELEMENT_CONE 7
#define ELEMENT_CUBE 8
#define ELEMENT_CYLINDER 9
#define ELEMENT_TEAPOT 10
#define ELEMENT_SEQUENCE 11
#define ELEMENT_CIRCLE 12

static float ABS(float x) 
{
    if(x >= 0)
        return x;
    return -x;
}

typedef struct BasicGLPoint_st
{
    float color[4];
    float xyz[3];

    BasicGLPoint_st()
    {
        color[0] = color[1] = color[2] = color[3] = 1.0f;
        xyz[0] = xyz[1] = xyz[2] = 0.0f;
    }

    struct BasicGLPoint_st* translate(float x, float y, float z)
    {
        xyz[0] += x;
        xyz[1] += y;
        xyz[2] += z;
        return this;
    }

    struct BasicGLPoint_st* moveTo(float x, float y, float z)
    {
        xyz[0] = x;
        xyz[1] = y;
        xyz[2] = z;
        return this;
    }

    struct BasicGLPoint_st* rgb(unsigned char r = 0, unsigned char g = 0, unsigned char b = 0, unsigned char a = 255)
    {
        color[0] = r/255.0f;
        color[1] = g/255.0f;
        color[2] = b/255.0f;
        color[3] = a/255.0f;
        return this;
    }

    struct BasicGLPoint_st* rgb(float r = 0, float g = 0, float b = 0, float a = 1.0f)
    {
        color[0] = r;
        color[1] = g;
        color[2] = b;
        color[3] = a;
        return this;
    }

}BasicGLPoint;

typedef struct BasicGLElement_st
{
    bool visible;
    int element;
    int size;
    float lineWidth;
    bool wireframe;
    float scales[3];
    float position[3];
    float rotation[3];
    float color[4];
    struct BasicGLElement_st *assoc;
    std::vector<BasicGLPoint> points;
    void *data;
    std::list<struct BasicGLElement_st*> elements;


    BasicGLElement_st()
    {
        visible = true;
        assoc = NULL;
        element = ELEMENT_POINT;
        size = 0;
        wireframe = false;
        scales[0] = scales[1] = scales[2] = 1.0f;
        position[0] = position[1] = position[2] = 0;
        rotation[0] = rotation[1] = rotation[2] = 0;
        lineWidth = 1.0f;
        points.clear();
        color[0] = color[1] = color[2] = color[3] = 1.0f;
        data = NULL;
    }

    virtual ~BasicGLElement_st() {}

    struct BasicGLElement_st* reshape(int n, bool byElement = true)
    {
        int m = 1;
        if(byElement)
        {
            switch(element)
            {
                case ELEMENT_LINE:
                    m = 2;
                    break;
                case ELEMENT_TRIANGLE:
                    m = 3;
                    break;
                case ELEMENT_QUAD:
                    m = 4;
                    break;
                default:
                    m = 1;
                    break;
            }
        }
        points.resize(m*n);
        size = n*m;
        return this;
    }

    struct BasicGLElement_st* CreateElement(int element);

    virtual struct BasicGLElement_st* translate(float x = 0.0f, float y = 0.0f, float z = 0.0f)
    {
        position[0] += x;
        position[1] += y;
        position[2] += z;
        return this;
    }

    virtual struct BasicGLElement_st* moveTo(float x = 0.0f, float y = 0.0f, float z = 0.0f)
    {
        position[0] = x;
        position[1] = y;
        position[2] = z;
        return this;
    }

    struct BasicGLElement_st* rotateX(float x = 0.0f)
    {
        return rotate(x, 0, 0);
    }

    struct BasicGLElement_st* rotateY(float y = 0.0f)
    {
        return rotate(0, y, 0);
    }

    struct BasicGLElement_st* rotateZ(float z = 0.0f)
    {
        return rotate(0, 0, z);
    }

    struct BasicGLElement_st* rotateToX(float x = 0.0f)
    {
        return rotateTo(x, rotation[1], rotation[2]);
    }

    struct BasicGLElement_st* rotateToY(float y = 0.0f)
    {
        return rotateTo(rotation[0], y, rotation[2]);
    }

    struct BasicGLElement_st* rotateToZ(float z = 0.0f)
    {
        return rotateTo(rotation[0], rotation[1], z);
    }

    virtual struct BasicGLElement_st* rotate(float x = 0.0f, float y = 0.0f, float z = 0.0f)
    {
        rotation[0] += x;
        rotation[1] += y;
        rotation[2] += z;
        return this;
    }

    virtual struct BasicGLElement_st* rotateTo(float x = 0.0f, float y = 0.0f, float z = 0.0f)
    {
        rotation[0] = x;
        rotation[1] = y;
        rotation[2] = z;
        return this;
    }

    virtual struct BasicGLElement_st* scale(float x = 1.0f, float y = FLT_MIN, float z = FLT_MIN)
    {
        if(y == FLT_MIN) y = x;
        if(z == FLT_MIN) z = y;
        scales[0] *= x;
        scales[1] *= y;
        scales[2] *= z;
        return this;
    }

    virtual struct BasicGLElement_st* scaleTo(float x = 1.0f, float y = FLT_MIN, float z = FLT_MIN)
    {
        if(y == FLT_MIN) y = x;
        if(z == FLT_MIN) z = y;
        scales[0] = x;
        scales[1] = y;
        scales[2] = z;
        return this;
    }

    virtual struct BasicGLElement_st* rgb(unsigned char r = 0, unsigned char g = 0, unsigned char b = 0, unsigned char a = 255)
    {
        color[0] = r/255.0f;
        color[1] = g/255.0f;
        color[2] = b/255.0f;
        color[3] = a/255.0f;
        for(int i = 0; i < size; i++)
            points[i].rgb(r, g, b, a);
        for(std::list<struct BasicGLElement_st*>::iterator i = elements.begin(); i != elements.end(); i++)
            (*i)->rgb(r, g, b, a);
        return this;
    }

    virtual struct BasicGLElement_st* rgb(float r = 0, float g = 0, float b = 0, float a = 1.0f)
    {
        color[0] = r;
        color[1] = g;
        color[2] = b;
        color[3] = a;
        for(int i = 0; i < size; i++)
            points[i].rgb(r, g, b, a);
        for(std::list<struct BasicGLElement_st*>::iterator i = elements.begin(); i != elements.end(); i++)
            (*i)->rgb(r, g, b, a);
        return this;
    }

    virtual struct BasicGLElement_st* setWireframe(bool wireframe)
    {
        this->wireframe = wireframe;
        for(std::list<struct BasicGLElement_st*>::iterator i = elements.begin(); i != elements.end(); i++)
            (*i)->setWireframe(wireframe);
    }

    struct BasicGLElement_st* point(float x, float y, int index = 0)
    {
        if(size > index)
        {
            points[index].xyz[0] = x;
            points[index].xyz[1] = y;
        }
        return this;
    }

    struct BasicGLElement_st* point(float x, float y, float z, int index = 0)
    {
        if(size > index)
        {
            points[index].xyz[0] = x;
            points[index].xyz[1] = y;
            points[index].xyz[2] = z;
        }
        return this;
    }

    struct BasicGLElement_st* line(float x1, float y1, float x2, float y2, int index = 0)
    {
        index *= 2;
        if(size > index + 1)
        {
            points[index].xyz[0] = x1;
            points[index].xyz[1] = y1;
            points[index+1].xyz[0] = x2;
            points[index+1].xyz[1] = y2;
        }
        return this;
    }

    struct BasicGLElement_st* line(float x1, float y1, float z1, float x2, float y2, float z2, int index = 0)
    {
        index *= 2;
        if(size > index + 1)
        {
            points[index].xyz[0] = x1;
            points[index].xyz[1] = y1;
            points[index].xyz[2] = z1;
            points[index+1].xyz[0] = x2;
            points[index+1].xyz[1] = y2;
            points[index+1].xyz[2] = z2;
        }
        return this;
    }

    struct BasicGLElement_st* triangle(float x1, float y1, float x2, float y2, float x3, float y3, int index = 0)
    {
        index *= 3;
        if(size > index + 2)
        {
            points[index].xyz[0] = x1;
            points[index].xyz[1] = y1;
            points[index+1].xyz[0] = x2;
            points[index+1].xyz[1] = y2;
            points[index+2].xyz[0] = x3;
            points[index+2].xyz[1] = y3;
        }
        return this;
    }

    struct BasicGLElement_st* triangle(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, int index = 0)
    {
        index *= 3;
        if(size > index + 2)
        {
            points[index].xyz[0] = x1;
            points[index].xyz[1] = y1;
            points[index].xyz[2] = z1;
            points[index+1].xyz[0] = x2;
            points[index+1].xyz[1] = y2;
            points[index+1].xyz[2] = z2;
            points[index+2].xyz[0] = x3;
            points[index+2].xyz[1] = y3;
            points[index+2].xyz[2] = z3;
        }
        return this;
    }

    struct BasicGLElement_st* quad(float x1, float y1, float x2, float y2, int index = 0)
    {
        index *= 4;
        if(size > index + 3)
        {
            points[index].xyz[0] = x1;
            points[index].xyz[1] = y1;
            points[index+1].xyz[0] = x1;
            points[index+1].xyz[1] = y2;
            points[index+2].xyz[0] = x2;
            points[index+2].xyz[1] = y2;
            points[index+3].xyz[0] = x2;
            points[index+3].xyz[1] = y1;
        }
        return this;
    }

    struct BasicGLElement_st* quad(float x1, float y1, float z1, float x2, float y2, float z2, int index = 0)
    {
        index *= 4;
        if(size > index + 3)
        {
            points[index].xyz[0] = x1;
            points[index].xyz[1] = y1;
            points[index].xyz[2] = z1;
            points[index+1].xyz[0] = x1;
            points[index+1].xyz[1] = y2;
            points[index+1].xyz[2] = (z1+z2)/2.0f;
            points[index+2].xyz[0] = x2;
            points[index+2].xyz[1] = y2;
            points[index+2].xyz[2] = z2;
            points[index+3].xyz[0] = x2;
            points[index+3].xyz[1] = y1;
            points[index+3].xyz[2] = (z1+z2)/2.0f;
        }
        return this;
    }

    struct BasicGLElement_st* circle(float x, float y, float r)
    {
        float step = 2 * M_PI / (size - 1);
        for(int i = 0; i < size; i++)
        {
            points[i].xyz[0] = r * cos(step * i);
            points[i].xyz[1] = r * sin(step * i);
        }
        return this;
    }

    struct BasicGLElement_st* circle(float x, float y, float z, float r)
    {
        float step = 2 * M_PI / (size - 1);
        for(int i = 0; i < size; i++)
        {
            points[i].xyz[0] = r * cos(step * i);
            points[i].xyz[1] = r * sin(step * i);
            points[i].xyz[2] = z;
        }
        return this;
    }

    struct BasicGLElement_st* glow()
    {
        for(int i = 0; i < size; i++)
        {
            switch(i % 4)
            {
                case 0:
                    points[i].color[0] = points[i].color[3] = 1.0f;
                    points[i].color[1] = points[i].color[2] = 0.0f;
                    break;
                case 1:
                    points[i].color[1] = points[i].color[3] = 1.0f;
                    points[i].color[0] = points[i].color[2] = 0.0f;
                    break;
                case 2:
                    points[i].color[2] = points[i].color[3] = 1.0f;
                    points[i].color[0] = points[i].color[1] = 0.0f;
                    break;
                case 3:
                    points[i].color[0] = points[i].color[1] = points[i].color[2] = points[i].color[3] = 1.0f;
                    break;
            }
        }
        return this;
    }

    void draw();

}BasicGLElement;

typedef struct BasicGLPlotSerie_st : BasicGLElement
{
    bool aligned;
    float minX, maxX, minY, maxY;
    float color[4];
    struct BasicGLPlotSerie_st* add(float x, float y);
    struct BasicGLPlotSerie_st* update(std::vector<float> &x, std::vector<float> &y);
    BasicGLPlotSerie_st() : BasicGLElement_st()
    {
        aligned = false;
        color[0] = color[1] = color[2] = 0.0f;
        color[3] = 1.0f;
    }

    void align(float minX, float maxX, float minY, float maxY, float *color);
}BasicGLPlotSerie;


typedef struct BasicGLPlot_st : BasicGLElement
{
    int rows, cols, index;
    float minX, maxX, minY, maxY;
    float axisMinX, axisMaxX, axisMinY, axisMaxY;
    bool fixedAxis;

    struct BasicGLPlotSerie_st* createSerie(const std::string opts = "k");
    struct BasicGLPlotSerie_st* plot(std::vector<float> &x, std::vector<float> &y, const std::string opts = "k");
    struct BasicGLPlot_st* axis(float minX, float maxX, float minY, float maxY);
    BasicGLPlot_st() : BasicGLElement_st()
    {
        axisMinX = axisMinY = -1;
        axisMaxX = axisMaxY = 1;
        fixedAxis = false;
    }

    virtual struct BasicGLPlot_st* rgb(unsigned char r = 0, unsigned char g = 0, unsigned char b = 0, unsigned char a = 255)
    {
        if(assoc != NULL && assoc->assoc != NULL)
            assoc->assoc->rgb(r, g, b, a);
        return this;
    }

    virtual struct BasicGLPlot_st* rgb(float r = 0, float g = 0, float b = 0, float a = 1.0f)
    {
        if(assoc != NULL && assoc->assoc != NULL)
            assoc->assoc->rgb(r, g, b, a);
        return this;
    }
}BasicGLPlot;

typedef struct BasicGLKeyboard_st
{
    int key;
    int x, y;
    bool shift, ctrl, alt;
}&BasicGLKeyboard;

typedef struct BasicGLMouse_st
{
    bool left, middle, right;
    bool pressed, released, move, entered, leave;
    int x, y;
}&BasicGLMouse;

struct BasicGLWindow_st;

typedef const std::vector<BasicGLElement*>& BasicGLElements;
typedef void (*AnimationFunction)(BasicGLElements elements, struct BasicGLWindow_st *window, float ellapsedTime);
typedef void (*KeyboardFunction)(BasicGLKeyboard keyboard, struct BasicGLWindow_st *window);
typedef void (*MouseFunction)(BasicGLMouse mouse, struct BasicGLWindow_st *window);

typedef struct BasicGLCamera_st
{
    float rotation[3];
    float position[3];
    float translation[3];

    BasicGLCamera_st()
    {
        rotation[0] = rotation[1] = rotation[2] = 0;
        translation[0] = 0;
        translation[1] = 0;
        translation[2] = -1.0f;
        position[0] = 0;
        position[1] = 0;
        position[2] = 5.0f;
    }

    struct BasicGLCamera_st* translate(float x = 0.0f, float y = 0.0f, float z = 0.0f)
    {
        position[0] += x;
        position[1] += y;
        position[2] += z;
        return this;
    }

    struct BasicGLCamera_st* moveTo(float x = 0.0f, float y = 0.0f, float z = 0.0f)
    {
        position[0] = x;
        position[1] = y;
        position[2] = z;
        return this;
    }

    struct BasicGLCamera_st* rotate(float x = 0.0f, float y = 0.0f, float z = 0.0f)
    {
        rotation[0] += x;
        rotation[1] += y;
        rotation[2] += z;
        translation[0] = cos(rotation[1]) * -sin(rotation[0]);
        translation[1] = sin(rotation[1]);
        translation[2] = cos(rotation[1]) * -cos(rotation[0]);
        return this;
    }

    struct BasicGLCamera_st* rotateTo(float x = 0.0f, float y = 0.0f, float z = 0.0f)
    {
        rotation[0] = x;
        rotation[1] = y;
        rotation[2] = z;
        translation[0] = cos(rotation[1]) * sin(rotation[0]);
        translation[1] = sin(rotation[1]);
        translation[2] = cos(rotation[1]) * cos(rotation[0]);
        return this;
    }

}BasicGLCamera;

typedef struct BasicGLWindow_st
{
    int id;
    int index;
    int mode;
    bool opened;
    BasicGLCamera camera;
    float bg[4];
    float timeSinceBegin;
    struct timespec lastTime;
    bool cartesian;

    std::vector<BasicGLElement*> elements;
    AnimationFunction animationFunction;
    KeyboardFunction keyboardFunction;
    MouseFunction mouseFunction;

    BasicGLWindow_st()
    {
        bg[0] = bg[1] = bg[2] = 0.0f;
        bg[3] = 1.0f;
        animationFunction = NULL;
        timeSinceBegin = 0;
        cartesian = true;
        memset(&lastTime, '\0', sizeof(lastTime));
        opened = true;
    }

}BasicGLWindow;

class BasicGL
{
    public:
        static void Init(int argc, char *argv[]);
        static void Destroy();

        static int CreateWindow(const char *name, int mode = MODE_2D, int width = 0, int height = 0, int x = -1, int y = -1);
        static int CreateWindow(const std::string name, int mode = MODE_2D, int width = 0, int height = 0, int x = -1, int y = -1);

        static void SelectWindow(int index);
        static void SetBackground(float r, float g, float b, float a = 1.0f);
        static void SetBackground(int r, int g, int b, int a = 255);
        static void SetAnimationFunction(AnimationFunction animationFunction);
        static void SetKeyboardFunction(KeyboardFunction keyboardFunction);
        static void SetMouseFunction(MouseFunction mouseFunction);
        static void SetCartesian(bool cartesian);
        static bool IsOpen();

        static BasicGLElement* CreateElement(int element, bool addToDrawer = true);
        static BasicGLPlot* CreatePlot(int rows = 1, int cols = 1, int index = 1);

        static void Show();
        static void Pause(float seconds = 0);
    
    private:
        static std::vector<BasicGLWindow> windows;
        static int currentWindow;
        static int WindowIndex();

        static void Closed();
        static void Render();
        static void KeyboardNormal(unsigned char key, int x, int y);
        static void KeyboardSpecial(int key, int x, int y);
        static void MouseButtons(int button, int state, int x, int y);
        static void MouseMotion(int x, int y);
        static void MouseEntry(int state);
        static void Resize(int w, int h);
        static void Timer(int index);
};

#endif
