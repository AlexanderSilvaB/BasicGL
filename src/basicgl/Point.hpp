#ifndef _BASIC_GL_POINT_H_
#define _BASIC_GL_POINT_H_

namespace BasicGL
{
    class Point
    {
        public:
            float color[4];
            float xyz[3];
        
            Point();
            virtual ~Point();
            
            Point& translate(float x = 0, float y = 0, float z = 0);
            Point& moveTo(float x = 0, float y = 0, float z = 0);
            Point& rgb(unsigned char r, unsigned char g = 0, unsigned char b = 0, unsigned char a = 255);
            Point& rgb(float r, float g = 0.0f, float b = 0.0f, float a = 1.0f);
    };
}

#endif
