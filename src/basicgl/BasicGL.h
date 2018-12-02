#ifndef _SIMPLE_GL_H_
#define _SIMPLE_GL_H_

#include <string>
#include <vector>
#include <cfloat>

#define MODE_2D 0
#define MODE_3D 1

#define ELEMENT_POINT 1
#define ELEMENT_LINE 2
#define ELEMENT_POLYGON 3
#define ELEMENT_TRIANGLE 4
#define ELEMENT_QUAD 5

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

    void translate(float x, float y, float z)
    {
        xyz[0] += x;
        xyz[1] += y;
        xyz[2] += z;
    }

    void moveTo(float x, float y, float z)
    {
        xyz[0] = x;
        xyz[1] = y;
        xyz[2] = z;
    }

    void rgb(unsigned char r = 0, unsigned char g = 0, unsigned char b = 0, unsigned char a = 255)
    {
        color[0] = r/255.0f;
        color[1] = g/255.0f;
        color[2] = b/255.0f;
        color[3] = a/255.0f;
    }

    void rgb(float r = 0, float g = 0, float b = 0, float a = 1.0f)
    {
        color[0] = r;
        color[1] = g;
        color[2] = b;
        color[3] = a;
    }

}BasicGLPoint;

typedef struct BasicGLElement_st
{
    int element;
    int size;
    float scales[3];
    float position[3];
    float rotation[3];
    BasicGLPoint *points;
    void *data;

    BasicGLElement_st()
    {
        element = ELEMENT_POINT;
        size = 0;
        scales[0] = scales[1] = scales[2] = 1.0f;
        position[0] = position[1] = position[2] = 0;
        rotation[0] = rotation[1] = rotation[2] = 0;
        points = NULL;
        data = NULL;
    }

    void reshape(int n)
    {
        if(points != NULL)
            delete[] points;
        if(n == 0)
            points = NULL;
        else
            points = new BasicGLPoint[n];
        size = n;
    }

    void translate(float x = 0.0f, float y = 0.0f, float z = 0.0f)
    {
        position[0] += x;
        position[1] += y;
        position[2] += z;
    }

    void moveTo(float x = 0.0f, float y = 0.0f, float z = 0.0f)
    {
        position[0] = x;
        position[1] = y;
        position[2] = z;
    }

    void rotate(float x = 0.0f, float y = 0.0f, float z = 0.0f)
    {
        rotation[0] += x;
        rotation[1] += y;
        rotation[2] += z;
    }

    void rotateTo(float x = 0.0f, float y = 0.0f, float z = 0.0f)
    {
        rotation[0] = x;
        rotation[1] = y;
        rotation[2] = z;
    }

    void scale(float x = 0.0f, float y = FLT_MIN, float z = FLT_MIN)
    {
        if(y == FLT_MIN) y = x;
        if(z == FLT_MIN) z = y;
        scales[0] += x;
        scales[1] += y;
        scales[2] += z;
    }

    void scaleTo(float x = 1.0f, float y = FLT_MIN, float z = FLT_MIN)
    {
        if(y == FLT_MIN) y = x;
        if(z == FLT_MIN) z = y;
        scales[0] = x;
        scales[1] = y;
        scales[2] = z;
    }

    void rgb(unsigned char r = 0, unsigned char g = 0, unsigned char b = 0, unsigned char a = 255)
    {
        for(int i = 0; i < size; i++)
            points[i].rgb(r, g, b, a);
    }

    void rgb(float r = 0, float g = 0, float b = 0, float a = 1.0f)
    {
        for(int i = 0; i < size; i++)
            points[i].rgb(r, g, b, a);
    }

}BasicGLElement;

struct BasicGLWindow_st;

typedef const std::vector<BasicGLElement*>& BasicGLElements;
typedef void (*AnimationFunction)(BasicGLElements elements, struct BasicGLWindow_st *window, float ellapsedTime);

typedef struct BasicGLWindow_st
{
    int id;
    int index;
    int mode;
    float bg[4];
    float timeSinceBegin;
    std::vector<BasicGLElement*> elements;
    AnimationFunction animationFunction;

    BasicGLWindow_st()
    {
        bg[0] = bg[1] = bg[2] = 0.0f;
        bg[3] = 1.0f;
        animationFunction = NULL;
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

        static BasicGLElement* CreateElement(int element);

        static void Run();
    
    private:
        static std::vector<BasicGLWindow> windows;
        static int currentWindow;

        static void Render();
        static void Resize(int w, int h);
        static void Timer(int index);
};

#endif
