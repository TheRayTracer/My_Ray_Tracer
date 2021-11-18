/* File: film.h; Mode: C++; Tab-width: 3; Author: Simon Flannery;             */

#ifndef FILM_H
#define FILM_H

#include <assert.h>

#include "image.h"
#include "math.h"

class Filter
{
public:
   virtual float GetWeight(const vector2f& p) const = 0;

   virtual ~Filter() { }
};

class Box : public Filter
{
public:
   virtual float GetWeight(const vector2f&) const
   {
      return 1.0f;
   }

protected:
private:
};

class Tent : public Filter
{
public:
   Tent(float radius) : r(radius) { }

   virtual float GetWeight(const vector2f& p) const
   {
      return Calculate(p[x]) * Calculate(p[y]);
   }

protected:
private:
   float Calculate(float d) const
   {
      float c = r - (float) fabs(d);

      if (c < 0.0)
      {
         c = 0.0f;
      }

      return c;
   }

   float r;
};

class Gaussian : public Filter
{
public:
   Gaussian(float sigma) : s(sigma) { }

   virtual float GetWeight(const vector2f& p) const
   {
   // return Calculate(p[x]) * Calculate(p[y]);

      return Calculate(p);
   }

protected:
private:
   float Calculate(const vector2f& p) const
   {
   // float c = (float) (expf(-((d * d) / (2.0f * s * s))));

      float c = (float) (expf(-(((p[x] * p[x]) + (p[y] * p[y])) / (2.0f * s * s))));

      if (c < 0.0)
      {
         c = 0.0f;
      }

      return c;
   }

   float s;
};

class Sample
{
public:
   Sample() : offset(vector2f(0.5f, 0.5f)), color(vector3f(0.0f, 0.0f, 0.0f)) { };
   Sample(const vector2f& p, const vector3f& c) : offset(p), color(c) { };

   vector2f GetOffset() const {   return offset;   }
   vector3f GetColor() const {   return color;   }

   bool Set(const vector2f& p, const vector3f& c)
   {
      bool result = false;

      if (p[x] >= 0.0f && p[x] <= 1.0f && p[y] >= 0.0f && p[y] <= 1.0f)
      {
         offset = p;
         color = c;

         result = true;
      }

      return result;
   }

private:
   vector2f offset;
   vector3f color;
};

class Film
{
public:
   Film(int w, int h, int n)
   {
      width = w;
      height = h;
      num_samples = n;

      samples = new Sample[width * height * num_samples];
   }

   ~Film()
   {
      delete [] samples;
   }

   int GetWidth() const { return width; }
   int GetHeight() const { return height; }
   int GetNumSamples() const { return num_samples; }

   void SetSample(int _x, int _y, int i, const vector2f& offset, const vector3f& color)
   {
      samples[GetIndex(_x, _y, i)].Set(offset, color);

      return;
   }

   void SetSample(int _x, int _y, int i, const Sample& s)
   {
      samples[GetIndex(_x, _y, i)].Set(s.GetOffset(), s.GetColor());

      return;
   }

   vector3f GetColor(int _x, int _y, const Filter* filter)
   {
      vector3f color;
      float weight_sum = 0.0f;

      for (int i = 0; i < num_samples; ++i)
      {
         Sample s = GetSample(_x, _y, i);
         float weight = filter->GetWeight(s.GetOffset());

         weight_sum = weight_sum + weight;

         color = color + (weight * s.GetColor());
      }

      return color / (float) weight_sum;
   }

   void Output(const char* szImageFileName, const Filter* filter)
   {
      Image capture(width, height);

      for (int i = 0; i < width; ++i)
      {
         for (int j = 0; j < height; ++j)
         {
            capture.SetPixel(i, j, GetColor(i, j, filter));
         }
      }

      Image::Format type = Image::Format::TGA;

      if (strstr(szImageFileName, ".bmp") != NULL)
      {
         type = Image::Format::BMP;
      }

      capture.Save(szImageFileName, type);

      return;
   }

protected:
private:
   Sample GetSample(int i, int j, int n) const
   {
      return samples[GetIndex(i, j, n)];
   }

   int GetIndex(int i, int j, int n) const
   {
      return i * height * num_samples + j * num_samples + n;
   }

   int width;
   int height;
   int num_samples;
   Sample* samples;
};

#endif
