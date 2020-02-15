/* File: material.h; Mode: C++; Tab-width: 3; Author: Simon Flannery;         */

#ifndef MATERIAL_H
#define MATERIAL_H

#include "math.h"
#include "ray.h"
#include "hit.h"
#include "perlin.h"

#define max(a, b) (((a) > (b)) ? (a) : (b))

class Material
{
public:
   virtual vector3f GetDiffuseColor(const vector3f&) const {   return diffuse_color;   }
   virtual vector3f GetSpecularColor(const vector3f&) const {   return specular_color;   }
   virtual vector3f GetReflectiveColor(const vector3f&) const {   return reflective_color;   }
   virtual vector3f GetTransparentColor(const vector3f&) const {   return transparent_color;   }
   virtual float    GetRefractionIndex(const vector3f&) const {   return refraction_index;   }
   virtual float    GetBlur(const vector3f&) const {   return blur;   }

   virtual void ShadeBack(bool back = true) {   shade_back = back;   return;   }

   virtual vector3f Shade(const Ray& ray, const Hit& hit, const vector3f& direction_to_light, const vector3f& light_color) const = 0;

protected:
   vector3f diffuse_color;
   vector3f specular_color;
   vector3f reflective_color;
   vector3f transparent_color;

   float exponent;
   float refraction_index;
   float blur;

   bool shade_back;

private:
   friend class Checkerboard;
   friend class NoiseMaterial;
   friend class MarbleMaterial;
   friend class WoodMaterial;
};

class PhongMaterial : public Material
{
public:
   PhongMaterial(const vector3f& diffuse, const vector3f& specular, float e)
   {
      diffuse_color  = diffuse;
      specular_color = specular;
      exponent = e;
      blur = 0.0f;

      shade_back = false;
   }

   PhongMaterial(const vector3f& diffuse, const vector3f& specular, float e, const vector3f& reflective, const vector3f& transparent, float refraction)
   {
      diffuse_color  = diffuse;
      specular_color = specular;
      exponent = e;
      reflective_color  = reflective;
      transparent_color = transparent;
      refraction_index  = refraction;
      blur = 0.0f;

      shade_back = false;
   }

   PhongMaterial(const vector3f& diffuse, const vector3f& specular, float e, const vector3f& reflective, const vector3f& transparent, float refraction, float blurriness)
   {
      diffuse_color  = diffuse;
      specular_color = specular;
      exponent = e;
      reflective_color  = reflective;
      transparent_color = transparent;
      refraction_index  = refraction;
      blur = blurriness;

      shade_back = false;
   }

   virtual vector3f Shade(const Ray& ray, const Hit& hit, const vector3f& direction_to_light, const vector3f& light_color) const
   {
      vector3f normal = hit.GetNormal();

   /* Flip the normal when the eye is on the "wrong" side of the surface. */
      if (shade_back != false && vector3f::Dot(normal, ray.GetDirection()) > 0.0f)
      {
         normal.Negate();
      }

      const float d = max(vector3f::Dot(normal, direction_to_light), 0.0f);

      vector3f h = ray.GetDirection().Negate() + direction_to_light;

      const float nh = vector3f::Dot(normal, h.Normalize());

      const vector3f shade = (d * light_color * diffuse_color) + light_color * specular_color * (float) pow((max(nh, 0.0f)), exponent);

      return shade;
   }

protected:
private:
};

class Checkerboard : public Material
{
public:
   Checkerboard(Matrix m, Material* m1, Material* m2) : matrix(m), material1(m1), material2(m2)
   {

   }

   virtual vector3f Shade(const Ray& ray, const Hit& hit, const vector3f& direction_to_light, const vector3f& light_color) const
   {
      vector3f t = hit.GetIntersectionPoint();
      matrix.Transform(t);

      int cx = (int) floor(t[x]);
      int cy = (int) floor(t[y]);
      int cz = (int) floor(t[z]);

      vector3f shade;
      if ((cx + cy + cz) % 2 == 0) /* Even! */
      {
         shade = material1->Shade(ray, hit, direction_to_light, light_color);
      }
      else
      {
         shade = material2->Shade(ray, hit, direction_to_light, light_color);
      }

      return shade;
   }

   virtual vector3f GetDiffuseColor(const vector3f& point) const
   {
      vector3f t = point;
      matrix.Transform(t);

      int cx = (int) floor(t[x]);
      int cy = (int) floor(t[y]);
      int cz = (int) floor(t[z]);

      vector3f diffuse;
      if ((cx + cy + cz) % 2 == 0) /* Even! */
      {
         diffuse = material1->GetDiffuseColor(point);
      }
      else
      {
          diffuse = material2->GetDiffuseColor(point);
      }

      return diffuse;
   }

   virtual vector3f GetReflectiveColor(const vector3f& point) const
   {
      vector3f t = point;
      matrix.Transform(t);

      int cx = (int) floor(t[x]);
      int cy = (int) floor(t[y]);
      int cz = (int) floor(t[z]);

      vector3f reflective;
      if ((cx + cy + cz) % 2 == 0) /* Even! */
      {
         reflective = material1->GetReflectiveColor(point);
      }
      else
      {
         reflective = material2->GetReflectiveColor(point);
      }

      return reflective;
   }

   virtual vector3f GetTransparentColor(const vector3f& point) const
   {
      vector3f t = point;
      matrix.Transform(t);

      int cx = (int) floor(t[x]);
      int cy = (int) floor(t[y]);
      int cz = (int) floor(t[z]);

      vector3f transparent;
      if ((cx + cy + cz) % 2 == 0) /* Even! */
      {
         transparent = material1->GetTransparentColor(point);
      }
      else
      {
         transparent = material2->GetTransparentColor(point);
      }

      return transparent;
   }

   virtual float GetRefractionIndex(const vector3f& point) const
   {
      vector3f t = point;
      matrix.Transform(t);

      int cx = (int) floor(t[x]);
      int cy = (int) floor(t[y]);
      int cz = (int) floor(t[z]);

      float refract;
      if ((cx + cy + cz) % 2 == 0) /* Even! */
      {
         refract = material1->GetRefractionIndex(point);
      }
      else
      {
         refract = material2->GetRefractionIndex(point);
      }

      return refract;
   }

   virtual float GetBlur(const vector3f& point) const
   {
      vector3f t = point;
      matrix.Transform(t);

      int cx = (int) floor(t[x]);
      int cy = (int) floor(t[y]);
      int cz = (int) floor(t[z]);

      float b;
      if ((cx + cy + cz) % 2 == 0) /* Even! */
      {
         b = material1->GetBlur(point);
      }
      else
      {
         b = material2->GetBlur(point);
      }

      return b;
   }

protected:
   Matrix matrix;
   Material* material1, * material2;

private:
};

class NoiseMaterial : public Material
{
public:
   NoiseMaterial(Matrix m, Material* m1, Material* m2, int oct) : matrix(m), material1(m1), material2(m2), octaves(oct)
   {

   }

   virtual vector3f Shade(const Ray& ray, const Hit& hit, const vector3f& direction_to_light, const vector3f& light_color) const
   {
      vector3f normal = hit.GetNormal();

      if (shade_back != false && vector3f::Dot(normal, ray.GetDirection()) > 0.0f)
      {
         normal.Negate();
      }

      const float d = max(vector3f::Dot(normal, direction_to_light), 0.0f);

      vector3f h = ray.GetDirection().Negate() + direction_to_light;

      const float nh = vector3f::Dot(hit.GetNormal(), h.Normalize());

      const vector3f shade = (d * light_color * GetDiffuseColor(hit.GetIntersectionPoint())) + light_color * GetSpecularColor(hit.GetIntersectionPoint()) * (float)pow((max(nh, 0.0f)), exponent);

      return shade;
   }

   virtual vector3f GetDiffuseColor(const vector3f& point) const
   {
      float noise = CalulateNoise(point);

      vector3f color_range = material1->diffuse_color - material2->diffuse_color;

      vector3f color = (color_range * noise) + material2->diffuse_color;

      return color.Clamp();
   }

   virtual vector3f GetSpecularColor(const vector3f& point) const
   {
      float noise = CalulateNoise(point);

      vector3f color_range = material1->specular_color - material2->specular_color;

      vector3f color = (color_range * noise) + material2->specular_color;

      return color.Clamp();
   }

   virtual vector3f GetReflectiveColor(const vector3f& point) const
   {
      float noise = CalulateNoise(point);

      vector3f color_range = material1->reflective_color - material2->reflective_color;

      vector3f color = (color_range * noise) + material2->reflective_color;

      return color.Clamp();
   }

protected:
   virtual float CalulateNoise(const vector3f& point) const
   {
      vector3f t = point;
      matrix.Transform(t);

      float noise = 0.0f;

      for (int i = 0, j = 1; i < octaves; ++i, j = 2 * j)
      {
         noise = noise + (float) (Perlin::noise(t[x] * j, t[y] * j, t[z] * j) / j);
      }

      return noise;
   }

   Matrix matrix;
   Material* material1, * material2;
   int octaves;

private:
};

class MarbleMaterial : public NoiseMaterial
{
public:
   MarbleMaterial(Matrix m, Material* m1, Material* m2, int oct, float fre, float amp) : NoiseMaterial(m, m1, m2, oct), frequency(fre), amplitude(amp)
   {

   }

   virtual vector3f GetDiffuseColor(const vector3f& point) const
   {
      float noise = CalulateNoise(point);

      vector3f color_range = material1->diffuse_color - material2->diffuse_color;

      vector3f color = (color_range * noise) + material2->diffuse_color;

      return color.Clamp();
   }

   virtual vector3f GetSpecularColor(const vector3f& point) const
   {
      float noise = CalulateNoise(point);

      vector3f color_range = material1->specular_color - material2->specular_color;

      vector3f color = (color_range * noise) + material2->specular_color;

      return color.Clamp();
   }

   virtual vector3f GetReflectiveColor(const vector3f& point) const
   {
      float noise = CalulateNoise(point);

      vector3f color_range = material1->reflective_color - material2->reflective_color;

      vector3f color = (color_range * noise) + material2->reflective_color;

      return color.Clamp();
   }

protected:
   virtual float CalulateNoise(const vector3f& point) const
   {
      vector3f t = point;
      matrix.Transform(t);

      float noise = 0.0f;

      for (int i = 0, j = 1; i < octaves; ++i, j = 2 * j)
      {
         noise = noise + (float) (Perlin::noise(t[x] * j, t[y] * j, t[z] * j) / j);
      }

      noise = (float) sin(frequency * t[x] + amplitude * noise);

      return noise;
   }

private:
   float frequency;
   float amplitude;
};

class WoodMaterial : public NoiseMaterial
{
public:
   WoodMaterial(Matrix m, Material* m1, Material* m2, int oct, float fre, float amp) : NoiseMaterial(m, m1, m2, oct), frequency(fre), amplitude(amp)
   {

   }

   virtual vector3f GetDiffuseColor(const vector3f& point) const
   {
      float noise = CalulateNoise(point);

      vector3f color_range = material1->diffuse_color - material2->diffuse_color;

      vector3f color = (color_range * noise) + material2->diffuse_color;

      return color.Clamp();
   }

   virtual vector3f GetSpecularColor(const vector3f& point) const
   {
      float noise = CalulateNoise(point);

      vector3f color_range = material1->specular_color - material2->specular_color;

      vector3f color = (color_range * noise) + material2->specular_color;

      return color.Clamp();
   }

   virtual vector3f GetReflectiveColor(const vector3f& point) const
   {
      float noise = CalulateNoise(point);

      vector3f color_range = material1->reflective_color - material2->reflective_color;

      vector3f color = (color_range * noise) + material2->reflective_color;

      return color.Clamp();
   }

protected:
   virtual float CalulateNoise(const vector3f& point) const
   {
      vector3f t = point;
      matrix.Transform(t);

      float noise = 0.0f;

      for (int i = 0, j = 1; i < octaves; ++i, j = 2 * j)
      {
         noise = noise + (float) (Perlin::noise(t[x] * j, t[y] * j, t[z] * j) / j);
      }

      noise = noise + (float) sin(frequency * sqrt(t[x] * t[x] + t[y] * t[y]) * amplitude);

      return noise;
   }

private:
   float frequency;
   float amplitude;
};

#endif
