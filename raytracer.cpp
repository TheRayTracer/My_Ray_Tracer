/* File: raytracer.cpp; Mode: C++; Tab-width: 3; Author: Simon Flannery;      */

#include "raytracer.h"
#include "scene.h"
#include "object.h"
#include "material.h"
#include "hit.h"
#include "light.h"

vector3f RayTracer::TraceRay(Ray& ray, Hit& hit, int bounce, float weight, float index_of_refraction) const
{
   vector3f color;

   if (bounce > bounces)
   {
      return color;
   }

   if (weight < cutoff)
   {
      return color;
   }

   if (hit.GetMaterial() == NULL)
   {
      return scene->GetBackgroundColor();
   }

   color = scene->GetAmbientLight() * hit.GetMaterial()->GetDiffuseColor(hit.GetIntersectionPoint());
   vector3f diffuse = hit.GetMaterial()->GetDiffuseColor(hit.GetIntersectionPoint());
   vector3f flip = hit.GetNormal();
/* Flip the normal when the eye is on the "wrong" side of the surface. */
   if (shade_back != false && vector3f::Dot(flip, ray.GetDirection()) > 0.0f)
   {
      flip.Negate();
   }

   for (int k = 0; k < scene->GetNumLights(); ++k)
   {
      float distance_to_light = 0.0f;
      Light* light = scene->GetLight(k);
      vector3f light_direction, light_color;

      int distribute = 1;

      if (light->UseSamples() != false)
      {
         distribute = samples;
      }

      vector3f distributed_color;
      for (int i = 0; i < distribute; ++i)
      {
         light->GetIllumination(hit.GetIntersectionPoint(), light_direction, light_color, distance_to_light);

         float d = vector3f::Dot(light_direction, flip);

         if (d < 0.0f || isnan(d) != false) // Added the isnan check 30-01-2020 as this was producing black pixels with AA enabled.
         {
            d = 0.0f;
         }

         if (shadow_support != false)
         {
            Stats::IncrementShadowRays();
            Ray ray2(hit.GetIntersectionPoint(), light_direction);
            Hit hit2(distance_to_light, NULL);

            if (scene->GetGroup()->ShadowIntersect(ray2, hit2, epsilon) == false) /* Not In Shadow. */
            {
               distributed_color = distributed_color + (d * light_color * diffuse);
               distributed_color = distributed_color + hit.GetMaterial()->Shade(ray, hit, light_direction, light_color);;
            }
         }
         else
         {
            distributed_color = distributed_color + (d * light_color * diffuse);
            distributed_color = distributed_color + hit.GetMaterial()->Shade(ray, hit, light_direction, light_color);
         }
      }

      color = color + distributed_color / (float) distribute;
   }

   vector3f reflective_color = hit.GetMaterial()->GetReflectiveColor(hit.GetIntersectionPoint());

   if (reflective_color > vector3f(0.0f, 0.0f, 0.0f))
   {
      float fa = hit.GetMaterial()->GetBlur(hit.GetIntersectionPoint());

      if (bounce < 1 && fa > 0.0f)
      {
         vector3f d = MirrorDirection(hit.GetNormal(), ray.GetDirection());
         vector3f vw, vu, vv;

      /* Another "big money" function! */
         ConstructBasisFromSingleVector(d, vw, vu, vv);

         vector3f distributed_color;
         for (int i = 0; i < samples; ++i)
         {
            float fu = (fa / -2.0f) + random_float() * fa;
            float fv = (fa / -2.0f) + random_float() * fa;

            vector3f d2 = d + fu * vu + fv * vv;

            Stats::IncrementNonShadowRays();
            Ray ray2(hit.GetIntersectionPoint(), d2.Normalize());
            Hit hit2;

            scene->GetGroup()->Intersect(ray2, hit2, epsilon);
            distributed_color = distributed_color + reflective_color * TraceRay(ray2, hit2, bounce + 1, weight * reflective_color.Length(), index_of_refraction);
         }

         color = color + distributed_color / (float) samples;
      }
      else
      {
         vector3f d = MirrorDirection(hit.GetNormal(), ray.GetDirection());

         Stats::IncrementNonShadowRays();
         Ray ray2(hit.GetIntersectionPoint(), d.Normalize());
         Hit hit2;

         scene->GetGroup()->Intersect(ray2, hit2, epsilon);
         color = color + reflective_color * TraceRay(ray2, hit2, bounce + 1, weight * reflective_color.Length(), index_of_refraction);
      }
   }

   vector3f transparent_color = hit.GetMaterial()->GetTransparentColor(hit.GetIntersectionPoint());

   if (transparent_color > vector3f(0.0f, 0.0f, 0.0f))
   {
      float index = 1.0f;
      if (index_of_refraction == 1.0f)
      {
         index = hit.GetMaterial()->GetRefractionIndex(hit.GetIntersectionPoint());
      }

      vector3f t;

      if (TransmittedDirection(flip, ray.GetDirection(), index_of_refraction, index, t) != false)
      {
         Stats::IncrementNonShadowRays();
         Ray ray2(hit.GetIntersectionPoint(), t.Normalize());
         Hit hit2;

         scene->GetGroup()->Intersect(ray2, hit2, epsilon);
         color = color + transparent_color * TraceRay(ray2, hit2, bounce + 1, transparent_color.Length(), index);
      }
   }

   return color;
}
