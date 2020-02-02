/* File: raytracer.h; Mode: C++; Tab-width: 3; Author: Simon Flannery;        */

#ifndef RAYTRACER_H
#define RAYTRACER_H

#include "math.h"
#include "Stats.h"

#define EPSILON  1e-4f

class Scene;
class Hit;
class Ray;

class RayTracer
{
public:
   RayTracer(Scene* s, int max_bounces, float cutoff_weight, bool back) : scene(s), bounces(max_bounces), cutoff(cutoff_weight), epsilon(EPSILON), shadow_support(false), shade_back(back), samples(1) {   }

   void EnableShadows(bool enable) {   shadow_support = enable;   return;   }

   void SetSamples(int s) {   samples = s;   return;   }

   void SetEpsilon(float e) {   epsilon = e;   return;   }

   vector3f TraceRay(Ray& ray, Hit& hit, int bounce, float weight, float index_of_refraction) const;

   void ConstructBasisFromSingleVector(const vector3f& a, vector3f& w, vector3f& u, vector3f& v) const
   {
      w = a;
      w.Normalize();

      vector3f t = w;

      if (fabs(t[x]) < fabs(t[y]) && fabs(t[x]) < fabs(t[z]))
      {
         t[x] = 1.0f;
      }
      else if (fabs(t[y]) < fabs(t[x]) && fabs(t[y]) < fabs(t[z]))
      {
         t[y] = 1.0f;
      }
      else if (fabs(t[z]) < fabs(t[y]) && fabs(t[z]) < fabs(t[x]))
      {
         t[z] = 1.0f;
      }

      u = vector3f::Cross(t, w);
      u.Normalize();

      v = vector3f::Cross(w, u);
   // v.Normalize();

      return;
   }

   vector3f MirrorDirection(const vector3f& normal, const vector3f& incoming) const
   {
      return incoming - (2.0f * vector3f::Dot(incoming, normal) * normal);
   }

   bool TransmittedDirection(const vector3f& normal, const vector3f& incoming, float index_i, float index_t, vector3f& t) const
   {
      bool result = false;
      float dn = vector3f::Dot(normal, incoming);

      vector3f a = (index_i * (incoming - (normal * dn))) / index_t;
      float    b = (1.0f - ((index_i * index_i) * (1.0f - (dn * dn))) / (index_t * index_t));

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
