#include "Serie.hpp"
#include <cmath>
#include <algorithm>

using namespace BasicGL;
using namespace std;

Serie::Serie() : Element(SEQUENCE)
{
    aligned = false;
    rgb(0.0f, 0.0f, 0.0f);
}

Serie::~Serie()
{

}

SeriePtr Serie::add(float x, float y, float z)
{
    Point pt;
    pt.moveTo(x, y, z);
    pt.rgb(color[0], color[1], color[2], color[3]);
    points.push_back(pt);
    if(points.size() == 1)
    {
        minX = maxX = x;
        minY = maxY = y;
    }
    else
    {
        if(x < minX)
            minX = x;
        if(x > maxX)
            maxX = x;
        if(y < minY)
            minY = y;
        if(y > maxY)
            maxY = y;
    }
    aligned = false;
    return this;
}

SeriePtr Serie::update(vector<float> &x, vector<float> &y)
{
    int sz = min(x.size(), y.size());
    reshape(sz);
    for(int i = 0; i < sz; i++)
    {
        points[i].moveTo(x[i], y[i], 0);
        points[i].rgb(color[0], color[1], color[2], color[3]);
        if(i == 0)
        {
            minX = maxX = x[i];
            minY = maxY = y[i];
        }
        else
        {
            if(x[i] < minX)
                minX = x[i];
            if(x[i] > maxX)
                maxX = x[i];
            if(y[i] < minY)
                minY = y[i];
            if(y[i] > maxY)
                maxY = y[i];
        }
    }
    aligned = false;
    return this;
}

SeriePtr Serie::update(vector<float> &x, vector<float> &y, vector<float> &z)
{
    int sz = min( min(x.size(), y.size()), z.size() );
    reshape(sz);
    for(int i = 0; i < sz; i++)
    {
        points[i].moveTo(x[i], y[i], z[i]);
        points[i].rgb(color[0], color[1], color[2], color[3]);
        if(i == 0)
        {
            minX = maxX = x[i];
            minY = maxY = y[i];
        }
        else
        {
            if(x[i] < minX)
                minX = x[i];
            if(x[i] > maxX)
                maxX = x[i];
            if(y[i] < minY)
                minY = y[i];
            if(y[i] > maxY)
                maxY = y[i];
        }
    }
    aligned = false;
    return this;
}

void Serie::align(float minX, float maxX, float minY, float maxY, float *color)
{
    float ratioX = (maxX - minX) / (this->maxX - this->minX);
    float ratioY = (maxY - minY) / (this->maxY - this->minY);
    float sx = ratioX * 0.9;
    float sy = ratioY * 0.9;
    float x = 0.5f * (minX + maxX) - (0.5f * (this->minX + this->maxX)) * sx;
    float y = 0.5f * (minY + maxY) - (0.5f * (this->minY + this->maxY)) * sy;
    moveTo(x, y, 0);
    scaleTo(sx, sy, 1.0f);
}

void Serie::setColor(const string& opts)
{
    color[0] = color[1] = color[2] = 0.0f;
    color[3] = 1.0f;
    for(int i = 0; i < opts.size(); i++)
    {
        switch (opts[i])
        {
            case 'k':
            case 'K':
                color[0] = color[1] = color[2] = 0.0f;
                break;
            case 'r':
            case 'R':
                color[0] = 1.0f;
                color[1] = color[2] = 0.0f;
                break;
            case 'g':
            case 'G':
                color[1] = 1.0f;
                color[0] = color[2] = 0.0f;
                break;
            case 'b':
            case 'B':
                color[2] = 1.0f;
                color[0] = color[1] = 0.0f;
                break;
            case 'w':
            case 'W':
                color[0] = color[1] = color[2] = 1.0f;
                break;
            case 'm':
            case 'M':
                color[1] = 0.0f;
                color[0] = color[2] = 1.0f;
                break;
            case 'c':
            case 'C':
                color[0] = 0.0f;
                color[1] = color[2] = 1.0f;
                break;
            default:
                break;
        }
    }
}