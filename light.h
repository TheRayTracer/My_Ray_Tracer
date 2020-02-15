/* File: light.h; Mode: C++; Tab-width: 3; Author: Simon Flannery;            */

#ifndef LIGHT_H
#define LIGHT_H

#include <stdlib.h>
#include <float.h>
#include "math.h"

class Light
{
public:
   virtual void GetIllumination(const vector3f& p, vector3f& d, vector3f& c, float& distance_to_light) const = 0;
   virtual bool UseSamples() const = 0;
};

class DirectionalLight : public Light
{
public:
   DirectionalLight(const vector3f& d, const vector3f& c)
   {
      direction = d;
      direction.Normalize();
      color = c;
   }
  
   virtual void GetIllumination(const vector3f&, vector3f& d, vector3f& c, float& distance_to_light) const
   {
   /* The direction to the light is the opposite of the
      direction of the directional light source. */
      d = -direction;
      c = color;

      distance_to_light = FLT_MAX; 

      return;
   }

   virtual bool UseSamples() const
   {
      return false;
   }

protected:
private:
  vector3f direction;
  vector3f color;
};

class PointLight : public Light
{
public:
   PointLight(const vector3f& p, const vector3f& c, float a1, float a2, float a3)
   {
      position = p;
      color = c;
      attenuation_1 = a1;
      attenuation_2 = a2;
      attenuation_3 = a3;
   }

   virtual void GetIllumination(const vector3f& p, vector3f& d, vector3f& c, float& distance_to_light) const
   {
      d = position - p;

   /* Grab the length before the direction is normalized. */
      distance_to_light = d.Length(); 
      d.Normalize();

      float attenuation = 1.0f / (attenuation_1 + attenuation_2 * distance_to_light + attenuation_3 * distance_to_light * distance_to_light);

      if (attenuation < 0.0f)
      {
         attenuation = 0.0f;
      }

      c = color * attenuation;

      return;
  }

   virtual bool UseSamples() const
   {
      return false;
   }

protected:
   vector3f position;
   vector3f color;

/* Attenuation of the light. */
   float attenuation_1;
   float attenuation_2;
   float attenuation_3;

private:
};

class SoftLight : public PointLight
{
public:
   SoftLight(const vector3f& p, const vector3f& c, float a1, float a2, float a3) : PointLight(p, c, a1, a2, a3) {   }

   virtual void GetIllumination(const vector3f& p, vector3f& d, vector3f& c, float& distance_to_light) const
   {
      d = position - p;
      float distance_to_center = d.Length();

      vector3f random(random_float() - 0.5f, random_float() - 0.5f, random_float() - 0.5f);

      d = (position + random) - p;

   /* Grab the length before the direction is normalized. */
      distance_to_light = d.Length(); 
      d.Normalize();

      float attenuation = 1.0f / (attenuation_1 + attenuation_2 * distance_to_center + attenuation_3 * distance_to_center * distance_to_center);

      if (attenuation < 0.0f)
      {
         attenuation = 0.0f;
      }

      c = color * attenuation;

      return;
   }

   virtual bool UseSamples() const
   {
      return true;
   }

protected:
private:
};

#endif
