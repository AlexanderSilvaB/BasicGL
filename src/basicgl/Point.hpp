#ifndef _BASIC_GL_POINT_H_
#define _BASIC_GL_POINT_H_

#include <vector>

namespace BasicGL
{
    typedef struct
    {
        float data[3];
    }PointLocation;

    typedef struct
    {
        float data[4];
    }PointColor;

    typedef struct
    {
        float data[3];
    }PointTexCoord;
    


    class Point
    {
        public:
            static void init(PointLocation *xyz, PointColor* color, PointTexCoord* coords);

            
            static void translate(PointLocation *xyz, float x = 0, float y = 0, float z = 0);
            static void moveTo(PointLocation *xyz, float x = 0, float y = 0, float z = 0);
            static void rgb(PointColor* color, unsigned char r, unsigned char g = 0, unsigned char b = 0, unsigned char a = 255);
            static void rgb(PointColor* color, float r, float g = 0.0f, float b = 0.0f, float a = 1.0f);
            static void map(PointTexCoord* coords, float x = 0, float y = 0, float z = 0);
    };

    class PointList
    {
        private:
            std::vector<PointLocation> xyz;
            std::vector<PointColor> color;
            std::vector<PointTexCoord> coord;
        public:
            PointList();
            virtual ~PointList();

            int add();
            void rm(int index);
            PointLocation& getXYZ(int index);
            PointColor& getColor(int index);
            PointTexCoord& getCoord(int index);
            int size();

            void resize(int size);

            PointLocation* rawXYZ();
            PointColor* rawColor();
            PointTexCoord* rawCoords();

            float X(int index);
            float Y(int index);
            float Z(int index);
            float R(int index);
            float G(int index);
            float B(int index);
            float A(int index);
            float CX(int index);
            float CY(int index);
            float CZ(int index);

            void translate(int index, float x = 0, float y = 0, float z = 0);
            void moveTo(int index, float x = 0, float y = 0, float z = 0);
            void rgb(int index, unsigned char r, unsigned char g = 0, unsigned char b = 0, unsigned char a = 255);
            void rgb(int index, float r, float g = 0.0f, float b = 0.0f, float a = 1.0f);
            void map(int index, float x = 0, float y = 0, float z = 0);
    };
}

#endif
