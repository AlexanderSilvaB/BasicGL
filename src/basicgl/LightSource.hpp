#ifndef _BASIC_GL_LIGHT_HPP_
#define _BASIC_GL_LIGHT_HPP_

#include "Element.hpp"

namespace BasicGL
{
    class LightSource;

    typedef LightSource* LightSourcePtr;

    class LightSource
    {
        private:
            bool updated;
        public:
            float ambient[4];
            float diffuse[4];
            float specular[4];
            float position[4];
            float spotDirection[3];
            float spotExponent;
            float spotCutOff;
            float constantAttenuation;
            float linearAttenuation;
            float quadraticAttenuation;
            float translation[3];
            float rotation[3];

            LightSource();
            virtual ~LightSource();

            void apply(unsigned int index, bool cartesian);
            void update();

            virtual LightSourcePtr rgb(unsigned char r, unsigned char g = 0, unsigned char b = 0, unsigned char a = 255);
            virtual LightSourcePtr rgb(float r, float g = 0.0f, float b = 0.0f, float a = 1.0f);
            LightSourcePtr glow();

            virtual LightSourcePtr translate(float x = 0, float y = 0, float z = 0);
            virtual LightSourcePtr moveTo(float x = 0, float y = 0, float z = 0);
            virtual LightSourcePtr rotateX(float x = 0);
            virtual LightSourcePtr rotateY(float y = 0);
            virtual LightSourcePtr rotateZ(float z = 0);
            virtual LightSourcePtr rotateToX(float x = 0);
            virtual LightSourcePtr rotateToY(float y = 0);
            virtual LightSourcePtr rotateToZ(float z = 0);
            virtual LightSourcePtr rotate(float x = 0, float y = 0, float z = 0);
            virtual LightSourcePtr rotateTo(float x = 0, float y = 0, float z = 0);

            virtual LightSourcePtr rotateAroundX(ElementPtr element, float x = 0);
            virtual LightSourcePtr rotateAroundY(ElementPtr element, float y = 0);
            virtual LightSourcePtr rotateAroundZ(ElementPtr element, float z = 0);
            virtual LightSourcePtr rotateAroundToX(ElementPtr element, float x = 0);
            virtual LightSourcePtr rotateAroundToY(ElementPtr element, float y = 0);
            virtual LightSourcePtr rotateAroundToZ(ElementPtr element, float z = 0);
            virtual LightSourcePtr rotateAround(ElementPtr element, float x = 0, float y = 0, float z = 0);
            virtual LightSourcePtr rotateAroundTo(ElementPtr element, float x = 0, float y = 0, float z = 0);
    };
}

#endif
