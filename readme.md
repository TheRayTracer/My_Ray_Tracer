# My Ray Tracer
## About
This project started for an advanced graphics course at university, and I have continued to add features and improvements over time. This is a fully functional Whitted-style ray tracing framework supporting reflections, refractions, and shadows. The framework is capable of rendering scene description files supporting simple geometric primitives and models.

## Features
The following is a list of features that have been implemented:
- Loading scene description files
- Shapes and primitives including:
  - Spheres
  - Planes
  - Triangles
  - Cubes
  - Rectangles
  - Cones
- Loading basic model object files (*.obj)
- Shadows
- Solid Constructive Geometry
- Materials
- Perlin Noise
- Light sources:
  - Point light
  - Directional light
- Phong illumination
- Distributed ray tracing effects:
  - Depth of field
  - Glossy reflections
  - Motion blur (of spheres)
  - Soft shadows
- Render scenes to TGA and bitmap formats

## Example Renders
A few renders from the framework.

|                                                                                                     |                                                                                                            |
|-----------------------------------------------------------------------------------------------------|------------------------------------------------------------------------------------------------------------|
| ![Example1](results/focal_red_spheres_far_128.bmp)<br />Depth of field with far focus example       | ![Example2](results/csg_difference_cube_sphere_3.bmp)<br />Solid Constructive Geometry example             |
| ![Example3](results/teapot_on_wood_floor.bmp)<br />Teapot model rendering with a wood checked floor | ![Example4](results/shapes_scene.bmp)<br />Lots of shapes with glossy reflections                         |


## Building
The ray tracer is written in C++, where the source code was initially developed on a Unix system using the g++ compiler, but has since been migrated to using Microsoft Visual Studio. Although it has been a number of years, it still builds using g++ and a handy makefile with minimal changes (disabling bitmap support).

## Usage
Command line helper files have been provided that load the scenes and set support parameters.

## Author
Simon Flannery
