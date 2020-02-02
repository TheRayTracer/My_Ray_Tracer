/* File: image.h; Mode: C++; Tab-width: 3; Author: Simon Flannery;            */

#ifndef IMAGE_H
#define IMAGE_H

#define _CRT_SECURE_NO_DEPRECATE

#include <stdio.h>
#include "math.h"

#define BITMAP_SUPPORT 
#define BITMAP_ID      0x4D42

class Image
{
public:
   Image(int size);
   Image(const Image& i);
   ~Image();

   enum Format {TGA, BMP};

   int GetWidth()  const {   return w;   }
   int GetHeight() const {   return h;   }

   void SetPixel(float x, float y, const vector3f& color);
   void SetPixel(int x, int y, const vector3f& color);
   vector3f GetPixel(int x, int y) const;

   void Save(const char* szFileName, Format t) const;

protected:
private:
   void SaveTGA(const char* szFileName) const;
   void SaveBMP(const char* szFileName) const;

   static void WriteByte(FILE* file, unsigned char b)
   {
      fwrite(&b, sizeof(unsigned char), 1, file);

      return;
   }

   static unsigned char ClampColorComponent(float c)
   {
      int b = (int) (c * 255.0f);
      
      if (b <   0) b = 0;
      if (b > 255) b = 255;

      return (unsigned char) b;
   }

   int w, h;
   vector3f* data;
};

#endif
