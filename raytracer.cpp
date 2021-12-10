/* File: raytracer.cpp; Mode: C++; Tab-width: 3; Author: Simon Flannery;      */

#include "raytracer.h"
#include "scene.h"
#include "object.h"
#include "material.h"
#include "light.h"
#include "stats.h"

color3f RayTracer::TraceRay(const Ray& ray, size_t bounce, float weight, float index_of_refraction) const
{
   color3f color;

   if (bounce > bounces)
   {
      return color;
   }

   if (weight < cutoff)
   {
      return color;
   }

   Stats::IncrementIntersections();

   Hit hit;
   if (scene->GetGroup()->Intersect(ray, hit, epsilon) != false)
   {
      const vector3f diffuse = hit.GetMaterial()->GetDiffuseColor(hit.GetIntersectionPoint());
      
      color = scene->GetAmbientLight() * diffuse;

      hit.GetMaterial()->ShadeBack(shade_back);

      for (size_t k = 0; k < scene->GetNumLights(); ++k)
      {
         Light* light = scene->GetLight(k);

         size_t num_samples = 1;

         if (light->UseSamples() != false)
         {
            num_samples = samples;
         }

         color3f distributed_color;
         for (size_t i = 0; i < num_samples; ++i)
         {
            float distance_to_light = 0.0f;
            vector3f light_direction;
            color3f light_color;
            light->GetIllumination(hit.GetIntersectionPoint(), light_direction, light_color, distance_to_light);

            if (shadow_support != false)
            {
               Stats::IncrementShadowRays();
               const Ray ray2(hit.GetIntersectionPoint(), light_direction);
               Hit hit2(distance_to_light);

               if (scene->GetGroup()->ShadowIntersect(ray2, hit2, epsilon) == false) /* Not In Shadow. */
               {
                  distributed_color = distributed_color + hit.GetMaterial()->Shade(ray, hit, light_direction, light_color);
               }
            // else
            // { 
            /// * The following is very expermential to support shadows from transparent objects. */
            //    vector3f transparent_color = hit2.GetMaterial()->GetTransparentColor(hit2.GetIntersectionPoint());

            //    if (transparent_color > vector3f(0.0f, 0.0f, 0.0f))
            //    {
            //       float d = max(vector3f::Dot(light_direction, hit.GetNormal()), 0.0f);

            //       distributed_color = distributed_color + d * light_color * transparent_color;
            //    }
            // }
            }
            else
            {
               distributed_color = distributed_color + hit.GetMaterial()->Shade(ray, hit, light_direction, light_color);
            }
         }

         color = color + distributed_color / (float) num_samples;
      }

      color3f reflective_color = hit.GetMaterial()->GetReflectiveColor(hit.GetIntersectionPoint());

      if (reflective_color > color3f(0.0f, 0.0f, 0.0f))
      {
         const float fa = hit.GetMaterial()->GetBlur(hit.GetIntersectionPoint());

         if (bounce < 1 && fa > 0.0f)
         {
            const vector3f d = MirrorDirection(hit, ray);
            vector3f vw, vu, vv;

            vector3f::ConstructBasisFromSingleVector(d, vw, vu, vv);

            color3f distributed_color;
            for (size_t i = 0; i < samples; ++i)
            {
               float fu = (fa / -2.0f) + random_float() * fa;
               float fv = (fa / -2.0f) + random_float() * fa;

               vector3f d2 = d + fu * vu + fv * vv;

               Stats::IncrementNonShadowRays();
               const Ray ray2(hit.GetIntersectionPoint(), d2.Normalize());

               distributed_color = distributed_color + reflective_color * TraceRay(ray2, bounce + 1, weight * reflective_color.Length(), index_of_refraction);
            }

            color = color + distributed_color / (float) samples;
         }
         else
         {
            vector3f d = MirrorDirection(hit, ray);

            Stats::IncrementNonShadowRays();
            const Ray ray2(hit.GetIntersectionPoint(), d.Normalize());

            color = color + reflective_color * TraceRay(ray2, bounce + 1, weight * reflective_color.Length(), index_of_refraction);
         }
      }

      color3f transparent_color = hit.GetMaterial()->GetTransparentColor(hit.GetIntersectionPoint());

      if (transparent_color > color3f(0.0f, 0.0f, 0.0f))
      {
         float index = 1.0f;
         if (index_of_refraction == 1.0f)
         {
            index = hit.GetMaterial()->GetRefractionIndex(hit.GetIntersectionPoint());
         }

         vector3f td;
         if (TransmittedDirection(hit, ray, index_of_refraction, index, td) != false)
         {
            Stats::IncrementNonShadowRays();
            const Ray ray2(hit.GetIntersectionPoint(), td.Normalize());

            color = color + transparent_color * TraceRay(ray2, bounce + 1, transparent_color.Length(), index);
         }
      }
   }
   else
   {
      color = scene->GetBackgroundColor();
   }

   return color;
}
