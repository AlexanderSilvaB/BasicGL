#ifndef _BASIC_GL_TEXTURE_H_
#define _BASIC_GL_TEXTURE_H_

#include "Point.hpp"
#include <string>

namespace BasicGL
{
    class Texture
    {
        private:
            bool hasData;
            unsigned int id, vbo;
            unsigned int width, height;
            void free();
        public:
            Texture();
            virtual ~Texture();

            bool available();
            bool load(const std::string& fileName);
            void fill(const unsigned int width, const unsigned int height, const unsigned char *data);

            void begin();
            void draw(int n, PointTexCoord *coords);
            void end();
    };
}

#endif
