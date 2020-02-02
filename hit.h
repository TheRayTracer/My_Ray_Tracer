/* File: hit.h; Mode: C++; Tab-width: 3; Author: Simon Flannery;              */

#ifndef HIT_H
#define HIT_H

#include <float.h>

#include "math.h"
#include "ray.h"

class Material;

class Hit
{
public:
   Hit(float _t = FLT_MAX, Material* m = NULL) : tmin(_t),  material(m) { }
   Hit(float _t, Material* m, const vector3f& n) : tmin(_t), material(m) {   normal = n;   }

   float GetT()            const {   return tmin;   }
   Material* GetMaterial() const {   return material;   }
   vector3f GetIntersectionPoint() const {   return intersection_point;   }
   vector3f GetNormal()    const {   return normal;   }

   void Set(float _t, Material* m, const vector3f& n, const Ray& ray)
   {
      tmin = _t;
      material = m;
      normal = n;

      normal.Normalize();

      intersection_point = ray.PointAtParameter(tmin);

      return;
   }

protected:
private: 
   float tmin;
   Material* material;
   vector3f normal;
   vector3f intersection_point;
};

#endif
