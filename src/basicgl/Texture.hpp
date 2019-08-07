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
            unsigned int width, height, bpp;
            void free();

            // Capture image
            void *captureDevicePointer;
        public:
            bool autoCapture;
            Texture();
            virtual ~Texture();

            bool available();
            bool load(const std::string& fileName);
            void fill(const unsigned int width, const unsigned int height, const unsigned int bpp, const unsigned char *data);
            void update(const unsigned char *data);

            int getWidth();
            int getHeight();
            float getRatio();

            bool setCaptureDevice(int id);
            bool setCaptureDevice(const std::string& path);
            void captureFromDevice();
            static bool supportsCapture();

            void begin();
            void draw(int n, PointTexCoord *coords);
            void end();
    };
}

#endif
