#include "Plot.hpp"
#include <cmath>
#include <algorithm>

using namespace BasicGL;
using namespace std;

Plot::Plot(int rows, int cols, int index, const string name) : Element(CONTAINER, name), rows(rows), cols(cols), index(index)
{
    axisMinX = axisMinY = -1;
    axisMaxX = axisMaxY = 1;
    fixedAxis = false;
    init();
}

Plot::~Plot()
{

}

void Plot::init()
{
    if(index <= 0)
        index = 1;
        
    index--;

    int iY = index / rows;
    int iX = index % rows;
    

    minX = -1.0f + (2.0f / cols) * iX;
    maxX = minX + (2.0f / cols);

    minY = -1.0f + (2.0f / rows) * iY;
    maxY = minY + (2.0f / rows);

    Element* bg = new Element(RECTANGLE);
    float ratioX = (maxX - minX);
    float ratioY = (maxY - minY);
    bg->scaleTo(0.95 * ratioX, 0.95 * ratioY, 1.0f);
    bg->moveTo(0.5f * (minX + maxX), 0.5f * (minY + maxY), 0);
    bg->rgb(1.0f, 1.0f, 1.0f);

    Element* lines = new Element(LINE);
    lines->reshape(2);
    lines->line(-0.5, 0.5, -0.5, -0.5, 0);
    lines->line(-0.5, -0.5, 0.5, -0.5, 1);
    lines->rgb(0.0f, 0.0f, 0.0f);
    lines->scaleTo(0.91f * ratioX, 0.91f * ratioY, 1.0f);
    lines->moveTo(0.5f * (minX + maxX), 0.5f * (minY + maxY), 0);

    assoc = lines;
    lines->assoc = bg; 
}

SeriePtr Plot::createSerie(const string opts)
{
    SeriePtr serie = new Serie();
    serie->setColor(opts);
    elements.push_back(serie);
    return serie;
}

SeriePtr Plot::createNamedSerie(const string& name, const string opts)
{
    SeriePtr serie = createSerie(opts);
    serie->name = name;
    return serie;
}

SeriePtr Plot::plot(vector<float> &x, vector<float> &y, const string opts)
{
    SeriePtr serie = new Serie();
    serie->setColor(opts);
    serie->update(x, y);
    elements.push_back(serie);
    return serie;
}

SeriePtr Plot::plot(const string& name, vector<float> &x, vector<float> &y, const string opts)
{
    SeriePtr serie = plot(x, y, opts);
    serie->name = name;
    return serie;
}

SeriePtr Plot::plot(vector<float> &x, vector<float> &y, vector<float> &z, const string opts)
{
    SeriePtr serie(new Serie());
    serie->setColor(opts);
    serie->update(x, y, z);
    elements.push_back(serie);
    return serie;
}

SeriePtr Plot::plot(const string& name, vector<float> &x, vector<float> &y, vector<float> &z, const string opts)
{
    SeriePtr serie = plot(x, y, z, opts);
    serie->name = name;
    return serie;
}

PlotPtr Plot::background(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
    if(assoc && assoc->assoc)
        assoc->assoc->rgb(r, g, b, a);
    return this;
}

PlotPtr Plot::background(float r, float g, float b, float a)
{
    if(assoc && assoc->assoc)
        assoc->assoc->rgb(r, g, b, a);
    return this;
}