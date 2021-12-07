/* File: film.h; Mode: C++; Tab-width: 3; Author: Simon Flannery;             */

#ifndef FILM_H
#define FILM_H

#include <assert.h>

#include "image.h"
#include "math.h"

class Filter
{
public:
   virtual float GetWeight(const point2f& p) const = 0;

   virtual ~Filter() { }
};

class Box : public Filter
{
public:
   virtual float GetWeight(const point2f&) const
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

   virtual float GetWeight(const point2f& p) const
   {
      return Calculate(p[x]) * Calculate(p[y]);
   }

protected:
private:
   float Calculate(float d) const
   {
      float c = r - (float) fabs(d);

      if (c < 0.0f)
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
      float c = (float) (expf(-(((p[x] * p[x]) + (p[y] * p[y])) / (2.0f * s * s))));

      if (c < 0.0f)
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
   Sample() : offset(point2f(0.5f, 0.5f)), color(color3f(0.0f, 0.0f, 0.0f)) { };
   Sample(const point2f& p, const color3f& c) : offset(p), color(c) { };

   point2f GetOffset() const {   return offset;   }
   color3f GetColor() const {   return color;   }

   bool Set(const point2f& p, const color3f& c)
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
   point2f offset;
   color3f color;
};

class Film
{
public:
   Film(size_t w, size_t h, size_t n)
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

   size_t GetWidth() const { return width; }
   size_t GetHeight() const { return height; }
   size_t GetNumSamples() const { return num_samples; }

   void SetSample(size_t _x, size_t _y, size_t i, const point2f& offset, const color3f& color)
   {
      samples[GetIndex(_x, _y, i)].Set(offset, color);

      return;
   }

   void SetSample(size_t _x, size_t _y, size_t i, const Sample& s)
   {
      samples[GetIndex(_x, _y, i)].Set(s.GetOffset(), s.GetColor());

      return;
   }

   color3f GetColor(size_t _x, size_t _y, const Filter* filter)
   {
      color3f color;
      float weight_sum = 0.0f;

      for (size_t i = 0; i < num_samples; ++i)
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

      for (size_t i = 0; i < width; ++i)
      {
         for (size_t j = 0; j < height; ++j)
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
   Sample GetSample(size_t i, size_t j, size_t n) const
   {
      return samples[GetIndex(i, j, n)];
   }

   int GetIndex(size_t i, size_t j, size_t n) const
   {
      return i * height * num_samples + j * num_samples + n;
   }

   size_t width;
   size_t height;
   size_t num_samples;
   Sample* samples;
};

#endif
