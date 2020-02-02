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

   virtual vector3f Shade(const Ray& ray, const Hit& hit, const vector3f& direction_to_light, const vector3f& light_color) const = 0;

protected:
   vector3f diffuse_color;
   vector3f specular_color;
   vector3f reflective_color;
   vector3f transparent_color;

   float exponent;
   float refraction_index;
   float blur;

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
   }

   virtual vector3f Shade(const Ray& ray, const Hit& hit, const vector3f& direction_to_light, const vector3f& light_color) const
   {
      vector3f t = ray.GetDirection().Negate() + direction_to_light;
      vector3f h = t.Normalize();

      float nh = vector3f::Dot(h, hit.GetNormal());

   /* This is the real voodoo magic. */
      vector3f shade = specular_color * (float) pow((max(nh, 0.0f)), exponent) * light_color;

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
   /* Since only lighting effects are being calculated, choose either material,
      it should not matter? */
      return material1->Shade(ray, hit, direction_to_light, light_color);
   }

   virtual vector3f GetDiffuseColor(const vector3f& point) const
   {
      vector3f t = point;
      matrix.Transform(t);

      vector3f diffuse;
      int cx = (int) floor(t[x]);
      int cy = (int) floor(t[y]);
      int cz = (int) floor(t[z]);

      if (cx % 2 == 0) /* Even! */
      {
         if (cz % 2 == 0) /* Even! */
         {
            if (cy % 2 == 0) /* Even! */
            {
               diffuse = material1->GetDiffuseColor(point);
            }
            else
            {
               diffuse = material2->GetDiffuseColor(point);
            }
         }
         else
         {
            if (cy % 2 == 0) /* Even! */
            {
               diffuse = material2->GetDiffuseColor(point);
            }
            else
            {
               diffuse = material1->GetDiffuseColor(point);
            }
         }
      }
      else
      {
         if (cz % 2 == 0) /* Even! */
         {
            if (cy % 2 == 0) /* Even! */
            {
               diffuse = material2->GetDiffuseColor(point);
            }
            else
            {
               diffuse = material1->GetDiffuseColor(point);
            }
         }
         else
         {
            if (cy % 2 == 0) /* Even! */
            {
               diffuse = material1->GetDiffuseColor(point);
            }
            else
            {
               diffuse = material2->GetDiffuseColor(point);
            }
         }
      }

      return diffuse;
   }

   virtual vector3f GetReflectiveColor(const vector3f& point) const
   {
      vector3f t = point;
      matrix.Transform(t);

      vector3f reflective;
      int cx = (int) floor(t[x]);
      int cy = (int) floor(t[y]);
      int cz = (int) floor(t[z]);

      if (cx % 2 == 0) /* Even! */
      {
         if (cz % 2 == 0) /* Even! */
         {
            if (cy % 2 == 0) /* Even! */
            {
               reflective = material1->reflective_color; // GetReflectiveColor(point);
            }
            else
            {
               reflective = material2->reflective_color; // GetReflectiveColor(point);
            }
         }
         else
         {
            if (cy % 2 == 0) /* Even! */
            {
               reflective = material2->reflective_color; // GetReflectiveColor(point);
            }
            else
            {
               reflective = material1->reflective_color; // GetReflectiveColor(point);
            }
         }
      }
      else
      {
         if (cz % 2 == 0) /* Even! */
         {
            if (cy % 2 == 0) /* Even! */
            {
               reflective = material2->reflective_color; // GetReflectiveColor(point);
            }
            else
            {
               reflective = material1->reflective_color; // GetReflectiveColor(point);
            }
         }
         else
         {
            if (cy % 2 == 0) /* Even! */
            {
               reflective = material1->reflective_color; // GetReflectiveColor(point);
            }
            else
            {
               reflective = material2->reflective_color; // GetReflectiveColor(point);
            }
         }
      }

      return reflective;
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
      return 0.5f * (material1->Shade(ray, hit, direction_to_light, light_color) + material2->Shade(ray, hit, direction_to_light, light_color));
   }

   virtual vector3f GetDiffuseColor(const vector3f& point) const
   {
      vector3f t = point;
      matrix.Transform(t);

      float noise = 0.0f;

      for (int i = 0, j = 1; i < octaves; ++i, j = 2 * j)
      {
         noise = noise + (float) (Perlin::noise(t[x] * j, t[y] * j, t[z] * j) / j);
      }

   // if (noise < 0.0f) noise = 0.0f;
   // if (noise > 1.0f) noise = 1.0f;

      vector3f color_range = material1->diffuse_color - material2->diffuse_color;

      vector3f diffuse = (color_range * noise) + material2->diffuse_color;

      return diffuse.Clamp();
   }

protected:
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

   virtual vector3f Shade(const Ray& ray, const Hit& hit, const vector3f& direction_to_light, const vector3f& light_color) const
   {
      return 0.5f * (material1->Shade(ray, hit, direction_to_light, light_color) + material2->Shade(ray, hit, direction_to_light, light_color));
   }

   virtual vector3f GetDiffuseColor(const vector3f& point) const
   {
      vector3f t = point;
      matrix.Transform(t);

      float noise = 0.0f;

      for (int i = 0, j = 1; i < octaves; ++i, j = 2 * j)
      {
         noise = noise + (float) (Perlin::noise(t[x] * j, t[y] * j, t[z] * j) / j);
      }

      noise = (float) sin(frequency * t[x] + amplitude * noise);

   // if (noise < 0.0f) noise = 0.0f;
   // if (noise > 1.0f) noise = 1.0f;

      vector3f color_range = material1->diffuse_color - material2->diffuse_color;

      vector3f diffuse = (color_range * noise) + material2->diffuse_color;

      return diffuse.Clamp();
   }

protected:
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

   virtual vector3f Shade(const Ray& ray, const Hit& hit, const vector3f& direction_to_light, const vector3f& light_color) const
   {
      return 0.5f * (material1->Shade(ray, hit, direction_to_light, light_color) + material2->Shade(ray, hit, direction_to_light, light_color));
   }

   virtual vector3f GetDiffuseColor(const vector3f& point) const
   {
      vector3f t = point;
      matrix.Transform(t);

      float noise = 0.0f;

      for (int i = 0, j = 1; i < octaves; ++i, j = 2 * j)
      {
         noise = noise + (float) (Perlin::noise(t[x] * j, t[y] * j, t[z] * j) / j);
      }

      noise = noise + (float) sin(frequency * sqrt(t[x] * t[x] + t[y] * t[y]) * amplitude);

   // if (noise < 0.0f) noise = 0.0f;
   // if (noise > 1.0f) noise = 1.0f;

      vector3f color_range = material1->diffuse_color - material2->diffuse_color;

      vector3f diffuse = (color_range * noise) + material2->diffuse_color;

      return diffuse.Clamp();
   }

protected:
private:
   float frequency;
   float amplitude;
};

#endif
