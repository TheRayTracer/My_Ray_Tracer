/* File: main.cpp; Mode: C++; Tab-width: 3; Author: Simon Flannery;           */

/*
   A ray tracing framework supporting shadows, reflections, refractions
   and distribution effects.

   The ray traced scene, is committed to an image file.

   Simon Flannery, Advanced Graphics, La Trobe University, 2005.
   Simon Flannery, 2006 - 2020.
 */

#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <assert.h>

#include "math.h"
#include "camera.h"
#include "object.h"
#include "scene.h"
#include "image.h"
#include "hit.h"
#include "Stats.h"
#include "raytracer.h"

/* Render the ray traced scene to an image. */
void Voodoo(Scene* scene, RayTracer* trace, char* szImageFileName, char* szNormalFileName, int width, int height, const int samples, const int antialiase_samples = 9);

int main(int argc, char* argv[])
{
   srand((unsigned int) time(NULL));

   int width = 0, height = 0, bounces = 0, samples = 16, antialiase_samples = 32;
   float weight = 0.0f, epsilon = EPSILON;
   bool shadows = false, back = false;
   char* szInputFileName = NULL, * szImageFileName = NULL, * szNormalFileName = NULL;;

   for (int i = 1; i < argc; ++i)
   {
      if (strcmp(argv[i], "-input") == 0)
      {
         ++i; assert(i < argc);
         szInputFileName = argv[i];
      }
      else if (strcmp(argv[i], "-size") == 0)
      {
         ++i; assert(i < argc);
         width = atoi(argv[i]);
         ++i; assert(i < argc);
         height = atoi(argv[i]);
      }
      else if (strcmp(argv[i], "-output") == 0)
      {
         ++i; assert(i < argc);
         szImageFileName = argv[i];
      }
      else if (strcmp(argv[i], "-normals") == 0)
      {
         ++i; assert(i < argc);
         szNormalFileName = argv[i];
      }
      else if (strcmp(argv[i], "-antialiased") == 0)
      {
         ++i; assert(i < argc);
         antialiase_samples = atoi(argv[i]);
      }
      else if (strcmp(argv[i], "-shadows") == 0)
      {
         shadows = true;
      }
      else if (strcmp(argv[i], "-bounces") == 0)
      {
         ++i; assert(i < argc);
         bounces = atoi(argv[i]);
      }
      else if (strcmp(argv[i], "-weight") == 0)
      {
         ++i; assert(i < argc);
         weight = (float) atof(argv[i]);
      }
      else if (strcmp(argv[i], "-epsilon") == 0)
      {
         ++i; assert(i < argc);
         epsilon = (float) atof(argv[i]);
      }
      else if (strcmp(argv[i], "-shade_back") == 0)
      {
         back = true;
      }
      else if (strcmp(argv[i], "-samples") == 0)
      {
         ++i; assert(i < argc);
         samples = atoi(argv[i]);
      }
   }

   Scene* scene = new Scene(szInputFileName);
   RayTracer* trace = new RayTracer(scene, bounces, weight, back);
   trace->EnableShadows(shadows);
   trace->SetSamples(samples);
   trace->SetEpsilon(epsilon);

   Stats::Reset(height = width, width);

   Voodoo(scene, trace, szImageFileName, szNormalFileName, width, height, samples, antialiase_samples);

   Stats::Finish();

   delete scene;
   delete trace;

   Stats::Print();

   return 0;
}

void Voodoo(Scene* scene, RayTracer* trace, char* szImageFileName, char* szNormalFileName, int width, int height, const int samples, const int antialiase_samples)
{
   Image capture(height = width); /* Just make it square! */
   Image normal(height = width);

   Camera* camera = scene->GetCamera();
   Group*  group  = scene->GetGroup();

   int distribute = 1;

   if (camera != NULL && group != NULL)
   {
      if (camera->UseSamples() != false || scene->UseSamples() != false)
      {
         distribute = samples;
      }

      const int size = height * width;
      int last_percent = 0;
      int percent = 0;

      for (int i = 0; i < width; ++i)
      {
         for (int j = 0; j < height; ++j)
         {
            float pc = (float) (i * width + j);
            percent = (int) (100.0f * (pc / size));

            if (percent != last_percent)
            {
               printf("%2d%c ", percent, '%'); fflush(NULL);

               last_percent = percent;
            }

            vector3f color, normal_color;

            for (int a = 0; a < antialiase_samples; ++a)
            {
               vector2f p((i + random_float()) / (float) width, (j + random_float()) / (float) height);

               for (int k = 0; k < distribute; ++k)
               {
                  Stats::IncrementNonShadowRays();
                  Ray ray = camera->GenerateRay(p);
                  Hit hit; /* This is output! */
                  if (group->Intersect(ray, hit, camera->GetTMin()) != false) /* Render Object Hit Color! */
                  {
                     normal_color = normal_color + hit.GetNormal().Abs();
                     color = color + trace->TraceRay(ray, hit, 0, 1.0f, 1.0f);
                  }
                  else
                  {
                     color = color + scene->GetBackgroundColor(p[y]);
                  }
               }

             //  color = color / (float) distribute;
             //  normal_color = normal_color / (float) distribute;
            }

            color = color / (float) antialiase_samples;
            normal_color = normal_color / (float) antialiase_samples;

            capture.SetPixel(i, j, color / (float) distribute);
            normal.SetPixel(i, j, normal_color / (float) distribute);
         }
      }

      Image::Format type = Image::TGA;

      if (strstr(szImageFileName, ".bmp") != NULL)
      {
         type = Image::BMP;
      }

      capture.Save(szImageFileName, type);
      normal.Save(szNormalFileName, type);
   }

   return;
}
