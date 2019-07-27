#ifndef _BASIC_GL_ELEMENT_H_
#define _BASIC_GL_ELEMENT_H_

#include "Point.hpp"
#include "Elements.hpp"
#include <float.h>
#include <vector>
#include <string>

namespace BasicGL
{
    enum Fonts { Default8x13, Default9x15, TimesRoman10, TimesRoman24, Helvetica10, Helvetica12, Helvetica18 };
    
    #define Left 1
    #define CenterX 2
    #define Right 4
    #define Top 8
    #define CenterY 16
    #define Bottom 32

    class Element;

    typedef Element* ElementPtr;

    class Element
    {
        private:
            unsigned int vboIds[2];
        protected:
            Elements element;
            void init();
            static void hsvTorgb(float h, float s, float v, float *rgb);
            void* getFont();
        public:
            bool visible;
            float lineWidth;
            bool wireframe;
            float scales[3];
            float position[3];
            float rotation[3];
            float color[4];
            std::string text;
            Fonts font;
            int alignment;
            ElementPtr assoc;
            PointList points;
            std::vector< ElementPtr > elements;
            void *data;
        
            Element(Elements element);
            virtual ~Element();
            
            ElementPtr reshape(int n, bool byElement = true);
            ElementPtr create(Elements element);

            virtual ElementPtr translate(float x = 0, float y = 0, float z = 0);
            virtual ElementPtr moveTo(float x = 0, float y = 0, float z = 0);
            virtual ElementPtr rotateX(float x = 0);
            virtual ElementPtr rotateY(float y = 0);
            virtual ElementPtr rotateZ(float z = 0);
            virtual ElementPtr rotateToX(float x = 0);
            virtual ElementPtr rotateToY(float y = 0);
            virtual ElementPtr rotateToZ(float z = 0);
            virtual ElementPtr rotate(float x = 0, float y = 0, float z = 0);
            virtual ElementPtr rotateTo(float x = 0, float y = 0, float z = 0);
            virtual ElementPtr scale(float x = 1.0f, float y = FLT_MIN, float z = FLT_MIN);
            virtual ElementPtr scaleTo(float x = 1.0f, float y = FLT_MIN, float z = FLT_MIN);
            virtual ElementPtr rgb(unsigned char r, unsigned char g = 0, unsigned char b = 0, unsigned char a = 255);
            virtual ElementPtr rgb(float r, float g = 0.0f, float b = 0.0f, float a = 1.0f);
            virtual ElementPtr setWireframe(bool wireframe);
            virtual ElementPtr setText(const std::string& text, Fonts font);

            float getTextWidth();
            float getTextHeight();
            ElementPtr textAlign(int alignmenet);

            ElementPtr point(float x, float y, int index = 0);
            ElementPtr point(float x, float y, float z, int index = 0);
            ElementPtr line(float x1, float y1, float x2, float y2, int index = 0);
            ElementPtr line(float x1, float y1, float z1, float x2, float y2, float z2, int index = 0);
            ElementPtr triangle(float x1, float y1, float x2, float y2, float x3, float y3, int index = 0);
            ElementPtr triangle(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, int index = 0);
            ElementPtr rectangle(float x1, float y1, float x2, float y2, int index = 0);
            ElementPtr rectangle(float x1, float y1, float z1, float x2, float y2, float z2, int index = 0);
            ElementPtr circle(float x, float y, float r);
            ElementPtr circle(float x, float y, float z, float r);
            ElementPtr glow();

            void invalidate();
            void draw();
    };
}

#endif
