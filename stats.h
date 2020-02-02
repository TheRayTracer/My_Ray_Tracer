/* File: stats.h; Mode: C++; Tab-width: 3; Author: Simon Flannery;            */

#ifndef STATS_H
#define STATS_H

#include <time.h>
#include <limits.h>
#include <stdio.h>

class Stats
{
public:
   static void Reset(int w, int h)
   {
      width  = w;
      height = h;

      start_time = time(NULL);
      finish_time = 0;

      nonshadow_rays = 0;
      shadow_rays    = 0;
      intersections  = 0;

      return;
   }

   static void Finish()
   {
      finish_time = time(NULL);

      return;
   }

/* Call for each non-shadow ray. */
   static void IncrementNonShadowRays()
   {
      nonshadow_rays++;

      return;
   }

/* Call for each shadow ray. */
   static void IncrementShadowRays()
   {
      shadow_rays++;

      return;
   }

/* To be called when each Object primitive's intersect routine is invoked (but not group and transform). 
   Note that this is a count of the number of times ray-primitive intersections are computed, not the number of times it returns true. */
   static void IncrementIntersections()
   {
      intersections++; /* WARNING: This might overflow. */

      return;
   }

   static void Print()
   {
      unsigned long long delta_time = finish_time - start_time;
      
      if (delta_time == 0)
      {
         delta_time = 1;
      }
      
      int seconds = (int) (delta_time % 60);
      int minutes = (int) ((delta_time / 60) % 60);
      int hours   = (int) (delta_time / (60 * 60));

      unsigned long long total_rays = nonshadow_rays + shadow_rays;
      
      float rays_per_sec =  (float) total_rays / (float) delta_time;
      float rays_per_pixel =  (float) total_rays / (float) (width * height);
      float intersections_per_ray = intersections / (float) total_rays;

      printf("\n");
      printf("********************************************\n");
      printf("SCENE RAY TRACE STATISTICS\n");
      printf("  Total time:                 %02d:%02d:%02d\n", hours, minutes, seconds);
      printf("  Total pixels:               %d (%dx%d)\n", width * height, width, height);
      printf("  Total non-shadow rays:      %lld\n",  nonshadow_rays);
      printf("  Total shadow rays:          %lld\n",  shadow_rays);
      printf("  Total intersections:        %lld\n",  intersections);
      printf("  Rays per second:            %0.1f\n", rays_per_sec);
      printf("  Rays per pixel:             %0.1f\n", rays_per_pixel);
      printf("  Intersections per ray:      %0.1f\n", intersections_per_ray);
      printf("********************************************\n");
      
      return;
   }

protected:
private:
   static int width;
   static int height;
   static unsigned long long start_time, finish_time;
   static unsigned long long nonshadow_rays;
   static unsigned long long shadow_rays;
   static unsigned long long intersections;
};

#endif
