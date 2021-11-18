/* File: camera.h; Mode: C++; Tab-width: 3; Author: Simon Flannery;           */

#ifndef CAMERA_H
#define CAMERA_H

#include <stdlib.h>
#include <float.h>
#include "math.h"
#include "ray.h"

class Camera
{
public:
   virtual Ray GenerateRay(const vector2f& point) const = 0;
   virtual bool UseSamples() const = 0;

   static vector3f LookAt(const vector3f& eye, const vector3f& look_at)
   {
      vector3f d = look_at - eye;

      return d.Normalize();
   }

   virtual ~Camera() { }
};

class OrthographicCamera : public Camera
{
public:
   OrthographicCamera(const vector3f& c, const vector3f& d, const vector3f& t, float s) : center(c), direction(d), up(t), size(s)
   {
      direction.Normalize();

      vector3f u = direction * vector3f::Dot(direction, up);
      up = up - u;

      up.Normalize();

      horizontal = vector3f::Cross(direction, up);
      horizontal.Normalize();
      
   // up = vector3f::Cross(horizontal, direction);
   // up.Normalize();
   }

   virtual Ray GenerateRay(const vector2f& point) const
   {
      vector3f origin = center + (point[x] - 0.5f) * size * horizontal + (point[y] - 0.5f) * size * up;

      Ray ray(origin, direction);

      return ray;
   }

   virtual bool UseSamples() const
   {
      return false;
   }

protected:

private:
   vector3f center;
   vector3f direction;
   vector3f up;
   vector3f horizontal;
   float size;
};

class PerspectiveCamera : public Camera
{
public:
   PerspectiveCamera(const vector3f& c, const vector3f& d, const vector3f& t, float a) : center(c), direction(d), up(t), angle(a)
   {
      direction.Normalize();

      vector3f u = direction * vector3f::Dot(direction, up);
      up = up - u;

      up.Normalize();

      horizontal = vector3f::Cross(direction, up);
      horizontal.Normalize();
      
      size = 1.0f * (float) tan(angle);
   }

   virtual Ray GenerateRay(const vector2f& point) const
   {
   // vector3f screen = center + direction + (point[x] - 0.5f) * size * horizontal + (point[y] - 0.5f) * size * up;
      vector3f d = direction + (point[x] - 0.5f) * size * horizontal + (point[y] - 0.5f) * size * up;

   /* Direction vectors are then calculated by subtracting the camera
      center point from the screen point. Don't forget to normalize. */
   // vector3f d = screen - center;

      Ray ray(center, d.Normalize());

      return ray;
   }

   virtual bool UseSamples() const
   {
      return false;
   }

protected:

private:
   vector3f center;
   vector3f direction;
   vector3f up;
   vector3f horizontal;
   float angle;
   float size;
};

class FocalCamera : public Camera
{
public:
   FocalCamera(const vector3f& c, const vector3f& d, const vector3f& t, float a, float focal, float e) : center(c), direction(d), up(t), angle(a), focal_depth(focal), lens(e)
   {
      direction.Normalize();

      vector3f u = direction * vector3f::Dot(direction, up);
      up = up - u;

      up.Normalize();

      horizontal = vector3f::Cross(direction, up);
      horizontal.Normalize();

      size = focal_depth * (float) tan(angle);
   }

   virtual Ray GenerateRay(const vector2f& point) const
   {
      vector3f screen = center + direction * focal_depth + (point[x] - 0.5f) * size * horizontal + (point[y] - 0.5f) * size * up;

      vector3f eye = center + ((rand() / (float) RAND_MAX) - 0.5f) * size * lens * horizontal + ((rand() / (float) RAND_MAX) - 0.5f) * size * lens * up;

      vector3f d = screen - eye;

      Ray ray(eye, d.Normalize());

      return ray;
   }

   virtual bool UseSamples() const
   {
      return true;
   }

protected:

private:
   vector3f center;
   vector3f direction;
   vector3f up;
   vector3f horizontal;
   float angle;
   float size;
   float focal_depth;
   float lens;
};

#endif
