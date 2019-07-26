#ifndef _BASIC_GL_SERIE_H_
#define _BASIC_GL_SERIE_H_

#include "Element.hpp"
#include <string>

namespace BasicGL
{
    class Serie;
    
    typedef Serie* SeriePtr;

    class Serie : public Element
    {
        public:
            bool aligned;
            float minX, maxX, minY, maxY;

            Serie();
            virtual ~Serie();
            SeriePtr add(float x, float y, float z = 0);
            SeriePtr update(std::vector<float> &x, std::vector<float> &y);
            SeriePtr update(std::vector<float> &x, std::vector<float> &y, std::vector<float> &z);
            void align(float minX, float maxX, float minY, float maxY, float *color);
            void setColor(const std::string& opts);
    };
}

#endif
