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
#include "hit.h"
#include "stats.h"
#include "raytracer.h"
#include "film.h"

/* Render the ray traced scene to an image. */
void Voodoo(Scene* scene, const char* szImageFileName, const int width, const int height, int bounces, const float weight, const float epsilon, const bool shadows, const bool back, const int samples, const int stratified, const Filter* filter);

int main(int argc, char* argv[])
{
   srand((unsigned int) time(NULL));

   int width = 0, height = 0, bounces = 0, samples = 16, stratified_samples = 5;
   float weight = 0.0f, epsilon = EPSILON;
   bool shadows = false, back = false;
   char* szInputFileName = NULL, * szImageFileName = NULL;
   Filter* filter = new Box();

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
      else if (strcmp(argv[i], "-filter") == 0)
      {
         ++i; assert(i < argc);

         if (strcmp(argv[i], "Tent") == 0)
         {
            ++i; assert(i < argc);

            float radius = (float) atof(argv[i]);

            filter = new Tent(radius);
         }
         else if (strcmp(argv[i], "Gaussian") == 0)
         {
            ++i; assert(i < argc);

            float radius = (float) atof(argv[i]);

            filter = new Gaussian(radius);
         }
      }
      else if (strcmp(argv[i], "-antialiased") == 0)
      {
         ++i; assert(i < argc);
         stratified_samples = atoi(argv[i]);
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

   Stats::Reset(width, height);

   Voodoo(scene, szImageFileName, width, height, bounces, weight, epsilon, shadows, back, samples, stratified_samples, filter);

   Stats::Finish();

   delete filter;
   delete scene;

   Stats::Print();

   return 0;
}

void Voodoo(Scene* scene, const char* szImageFileName, const int width, const int height, int bounces, const float weight, const float epsilon, const bool shadows, const bool back, const int samples, const int stratified, const Filter* filter)
{
   Film frame(width, height, stratified * stratified);

   RayTracer* trace = new RayTracer(scene, bounces, weight, back);
              trace->EnableShadows(shadows, epsilon);
              trace->SetSamples(samples);

   Camera* camera = scene->GetCamera();

   int distribute = 1;

   if (camera != NULL && scene->GetGroup() != NULL)
   {
      if (camera->UseSamples() != false || scene->UseSamples() != false)
      {
         distribute = samples;
      }

      const int size = height * width;
      int last_percent = 0;

      for (int i = 0; i < width; ++i)
      {
         for (int j = 0; j < height; ++j)
         {
            float pc = (float) (i * width + j);
            int percent = (int) (100.0f * (pc / size));

            if (percent != last_percent)
            {
               printf("%2d%c ", percent, '%'); fflush(NULL);

               last_percent = percent;
            }

            for (int s = 0; s < stratified; ++s) /* Using stratified samples in a grid for sub-pixels. */
            {
               for (int t = 0; t < stratified; ++t)
               {
                  vector3f color;

                  vector2f jitter((s + random_float()) / (float) stratified,
                                  (t + random_float()) / (float) stratified);

                  vector2f p((i + jitter[x]) / (float) width,
                             (j + jitter[y]) / (float) height);

                  for (int k = 0; k < distribute; ++k)
                  {
                     Stats::IncrementNonShadowRays();
                     const Ray ray = camera->GenerateRay(p);

                     color = color + trace->TraceRay(ray, 0, 1.0f, 1.0f);
                  }

                  color = color / (float) distribute;

                  frame.SetSample(i, j, s * stratified + t, jitter, color);
               }
            }
         }
      }

      frame.Output(szImageFileName, filter);
   }

   delete trace;

   return;
}
