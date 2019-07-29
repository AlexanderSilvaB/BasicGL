# BasicGL
[![Build Status](https://travis-ci.com/AlexanderSilvaB/BasicGL.svg?branch=master)](https://travis-ci.com/AlexanderSilvaB/BasicGL)

This is a simple framework which allows to create simple OpenGL apps without much code 

## Examples
Creates a simple triangle: [triangle.cpp](src/examples/triangle.cpp)

![Triangle example](docs/images/triangle.PNG?raw=true "Triangle")

Creates an arbitrary shape: [heart.cpp](src/examples/heart.cpp)

![Heart example](docs/images/heart.PNG?raw=true "Heart")

Uses the plotting capabilities: [plot.cpp](src/examples/plot.cpp)

![Plot example](docs/images/plot.PNG?raw=true "Plot")

Uses the container element to pack shapes together: [snowman.cpp](src/examples/snowman.cpp)

![Snowman example](docs/images/snowman.PNG?raw=true "Snowman")

Allows drawing using the mouse and keyboard features: [paint.cpp](src/examples/paint.cpp)

![Paint example](docs/images/paint.PNG?raw=true "Paint")

A simple OBJ viewer with zoom, translation and rotation capabilities: [viewer.cpp](src/examples/viewer.cpp)

![Viewer example](docs/images/viewer.PNG?raw=true "Viewer")

## Features
* Windows
* Camera control
* Cartesian and OpenGL modes (Y inverted)
* Keyboard and Mouse support
* 2D and 3D modes
* Basic shapes (point, line, triangle, rectangle, circle, sphere, cone, cube, cylinder)
* Complex 2D shapes (polygon, sequence, loop, connected triangles, connected rectangles, radial triangles )
* Complex 3D shapes (polygon, teapot)
* Container elements
* Text support
* Data plotting
* Automatic rainbow colors generation
* Animations
* Textures
* Automatic texture maps generation
* OBJ files
* Rotate, translate and scale any element
* Screen image capture

## To-do
- [ ] True Type fonts
- [ ] Screen video capture
- [ ] Lighting

## Example code
This simple code creates a rotating teapot
```cpp
// Includes the BasicGL library and the C++ standard math library (for M_PI)
#include <basicgl/Manager.hpp>
#include <cmath>

// Prevents from using BasicGL::
using namespace BasicGL;

/** 
    Rotates the teapot at 2 x PI radians per second
    @param elements: The list of elements of the current window
    @param window: A pointer to the current window
    @param ellapsedTime: The amount of seconds since the last call to this funcion 
*/
void rotate(ElementsList elements, WindowPtr window, float ellasedTime)
{
    /* 
        Select the first element of the window.
        As there's just one element (teapot), it will be the element of index = 0
        The element can also be selected using the elements parameter or the find function of the window
    */
    ElementPtr teapot = window->get(0);

    // Rotates the teapot element around the Y axis (speed x time)
    teapot->rotate( 0, 2 * M_PI * ellasedTime, 0 );
}


// The begin of the program
int main(int argc, char *argv[])
{
    // Initialize the BasicGL library
    Manager::Init(argc, argv);

    // Creates a new window in 3D mode named Teapot
    Manager::CreateWindow("Teapot", MODE_3D);

    // Sets the rotate function as the animation function for the window
    Manager::SetAnimationFunction(rotate);

    // Creates a new element of type TEAPOT and also named teapot
    Manager::CreateElement(TEAPOT, "teapot");

    // Shows the window
    Manager::Show();
    return 0;
}
```