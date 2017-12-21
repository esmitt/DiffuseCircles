2D Diffuse Lighting using OpenGL
===================

Three static circles with dynamic radius are drawn, with three dynamic lights into a scene. Circles are created using `GL_POLYGON` with an increment of each vertex of `2 x PI x INDEX / SIZE`, where `INDEX` is the current iteration from 0 to reach `SIZE`.

Lights should be enable/disable using keys F1, F2, and F3 for red, green and blue respectively. Lights affect a pixel's color (distance and position), because a Lambert's cosine law is applied. A normal vector of an internal 2D circle position is obtained as `pos - center`, where `center` is the center of the circle.

Lights bounce over the external window, and their colors are mixed into the final result.

The last time, this code was tested on Visual Studio 2017. You need to use [CMake](https://cmake.org/) to be more easy the dependencies (at least for me :-) due that I have several times the GLUT library on my repositories!)

###### If you want to contribute to this project and make it better, your help is very welcome.