#ifndef _BASIC_GL_FONT_H_
#define _BASIC_GL_FONT_H_

#include "Point.hpp"
#include <string>

namespace BasicGL
{
    enum DefaultFonts { Default8x13, Default9x15, TimesRoman10, TimesRoman24, Helvetica10, Helvetica12, Helvetica18, Custom };
    
    #define	Align_Left 1
    #define Align_CenterX 2
    #define Align_Right 4
    #define Align_Top 8
    #define Align_CenterY 16
    #define Align_Bottom 32

    class Font
    {
        private:
            bool isLoaded;
            unsigned int texture;
            void *cdata;
            int firstChar, lastChar;
            int width, height;
            float fontSize;
            void* getDefaultFont();
            void free();
            float getCharHeight(char c);
        public:
            DefaultFonts defaultFont;
            int alignment;
            float size;

            Font();
            virtual ~Font();

            bool loaded();
            bool load(const std::string& fileName);
            bool fill(const unsigned char *fontData);

            void setDefaultFont(DefaultFonts defaultFont);

            void print(const std::string& text, bool flip = false);

            float getTextWidth(const std::string& text);
            float getTextHeight(const std::string& text);
    };
}

#endif
