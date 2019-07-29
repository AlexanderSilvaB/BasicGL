#ifndef _BASIC_GL_PLOT_H_
#define _BASIC_GL_PLOT_H_

#include "Element.hpp"
#include "Serie.hpp"
#include <string>

namespace BasicGL
{
    class Plot;

    typedef Plot* PlotPtr;

    class Plot : public Element
    {
        private:
            int rows, cols, index;
            void init();
        public:
            float minX, maxX, minY, maxY;
            float axisMinX, axisMaxX, axisMinY, axisMaxY;
            bool fixedAxis;

            Plot(int rows = 1, int cols = 1, int index = 1, const std::string name = "");
            virtual ~Plot();

            SeriePtr createSerie(const std::string opts = "k");
            SeriePtr createNamedSerie(const std::string& name, const std::string opts = "k");
            SeriePtr plot(std::vector<float> &x, std::vector<float> &y, const std::string opts = "k");
            SeriePtr plot(const std::string& name, std::vector<float> &x, std::vector<float> &y, const std::string opts = "k");
            SeriePtr plot(std::vector<float> &x, std::vector<float> &y, std::vector<float> &z, const std::string opts = "k");
            SeriePtr plot(const std::string& name, std::vector<float> &x, std::vector<float> &y, std::vector<float> &z, const std::string opts = "k");
            PlotPtr axis(float minX, float maxX, float minY, float maxY);
            
            PlotPtr background(unsigned char, unsigned char g = 0, unsigned char b = 0, unsigned char a = 255);
            PlotPtr background(float r, float g = 0, float b = 0, float a = 1.0f);
    };
}

#endif
