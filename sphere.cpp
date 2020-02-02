/* File: object.h; Mode: C++; Tab-width: 3; Author: Simon Flannery;           */

#ifndef OBJECT_H
#define OBJECT_H

#include <float.h>

#include "math.h"
#include "ray.h"
#include "hit.h"

#define SIZE 0x0A

class Object
{
public:
   Object(Material* m = NULL) : material(m) { }

   virtual bool Intersect(const Ray& r, Hit& h, float tmin) = 0;

protected:
   Material* material;

private:
};

class Sphere : public Object
{
public:
   Sphere(vector3f p, float r, Material* m) : point(p), radius(r) {   material = m;   }

   virtual bool Intersect(const Ray& ray, Hit& h, float tmin)
   {
      bool result = false;

      float tp = 0.0f, td = 0.0f, t = FLT_MAX;
      float rod = vector3f::Distance(point, ray.GetOrigin());

      if (rod * rod > radius * radius) /* Outside Sphere. */
      {
         vector3f ro = point - ray.GetOrigin();

         tp = ro.Dot(ray.GetDirection());

         if (tp < 0.0f) /* No Hit. */
         {

         }
         else /* Possible Hit. */
         {
            vector3f p = ray.PointAtParameter(tp);
            float d = vector3f::Distance(point, p);

            if (d * d > radius * radius) /* No Hit. */
            {

            }
            else /* Hit. */
            {
               float td = sqrt(radius * radius - d * d);
               t = tp - td;

               if (t > tmin && t < h.GetT())
               {
                  h.Set(t, material, ray);

                  result = true;
               }
            }
         }
      }

      return result;
   }

protected:
private:
   vector3f point;
   float radius;
};

#endif
