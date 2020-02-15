/* File: raytracer.h; Mode: C++; Tab-width: 3; Author: Simon Flannery;        */

#ifndef RAYTRACER_H
#define RAYTRACER_H

#include "math.h"
#include "hit.h"
#include "ray.h"
#include "stats.h"

#define EPSILON  1e-4f

class Scene;

class RayTracer
{
public:
   RayTracer(Scene* s, int max_bounces, float cutoff_weight, bool back) : scene(s), bounces(max_bounces), cutoff(cutoff_weight), epsilon(EPSILON), shadow_support(false), shade_back(back), samples(1) {   }

   void EnableShadows(bool enable = true, float e = EPSILON) {   shadow_support = enable;   epsilon = e;   return;   }

   void SetSamples(int s) {   samples = s;   return;   }

   vector3f TraceRay(const Ray& ray, int bounce, float weight, float index_of_refraction) const;

   vector3f MirrorDirection(const Hit& hit, const Ray& ray) const
   {
      const vector3f incoming = ray.GetDirection();

      return incoming - (2.0f * vector3f::Dot(hit.GetNormal(), incoming) * hit.GetNormal());
   }

   bool TransmittedDirection(const Hit& hit, const Ray& ray, float index_i, float index_t, vector3f& t) const
   {
      bool result = false;

      vector3f normal = hit.GetNormal();
      const vector3f incoming = ray.GetDirection();

      if (vector3f::Dot(normal, incoming) > 0.0f)
      {
         normal.Negate();
      }

      const float dn = vector3f::Dot(normal, incoming);

      const vector3f a = (index_i * (incoming - (normal * dn))) / index_t;
      const float    b = (1.0f - ((index_i * index_i) * (1.0f - (dn * dn))) / (index_t * index_t));

      if (b > 0.0f)
      {
         result = true;
         t = a - normal * (float) sqrt(b);
      }

      return result;
   }

private:
   Scene* scene;
   int bounces;
   float cutoff;
   float epsilon;
   bool shadow_support;
   bool shade_back;

   int samples;
};

#endif
