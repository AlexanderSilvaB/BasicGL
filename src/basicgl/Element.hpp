#ifndef _BASIC_GL_ELEMENT_H_
#define _BASIC_GL_ELEMENT_H_

#include "Point.hpp"
#include "Elements.hpp"
#include "Texture.hpp"
#include "OBJ.hpp"
#include "Font.hpp"
#include <float.h>
#include <vector>
#include <string>

#define GL_GLEXT_PROTOTYPES

#include <GL/freeglut.h>
#include <GL/freeglut_ext.h>
#include <GL/gl.h>

namespace BasicGL
{
    class Element;

    typedef Element* ElementPtr;

    class Element
    {
        private:
            unsigned int vboIds[2];
            GLUquadric *solid;
        protected:
            Elements element;
            void init();
            static void hsvTorgb(float h, float s, float v, float *rgb);
            static float map(float v, float minIn, float maxIn, float minOut, float maxOut);
        public:
            std::string name;
            bool visible;
            float stroke;
            bool wireframe;
            float scales[3];
            float position[3];
            float rotation[3];
            float color[4];
            bool applyColors;
            std::string text;

            ElementPtr assoc;
            Texture texture;
            OBJ obj;
            Font font;

            PointList points;
            std::vector< ElementPtr > elements;
            void *data;
        
            Element(Elements element, const std::string name = "");
            virtual ~Element();

            ElementPtr find(const std::string& name);
            ElementPtr get(int index);

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
            virtual ElementPtr setText(const std::string& text);
            virtual ElementPtr setText(const std::string& text, DefaultFonts defaultFont);

            int newPoint(float x, float y, float z);

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
            ElementPtr glow(float start = 0, float scale = 1.0f);
            ElementPtr glowAt(int index, int pos = -1);
            ElementPtr map();
            ElementPtr textAlign(int alignment);

            float getTextWidth();
            float getTextHeight();

            void invalidate();
            void draw(bool cartesian);
    };
}

#endif
