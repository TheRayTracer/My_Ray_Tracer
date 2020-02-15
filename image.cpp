/* File: image.cpp; Mode: C++; Tab-width: 3; Author: Simon Flannery;          */

#include "image.h"

#ifdef BITMAP_SUPPORT
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

Image::Image(int width, int height) : w(width), h(height), data(NULL)
{
   int size = w * h;
   data = new vector3f[size];

   vector3f white(1.0f, 1.0f, 1.0f);

   for (int i = 0; i < size; ++i)
   {
      data[i] = white;
   }
}

Image::Image(const Image& i)
{
   w = i.w;
   h = i.w;

   int size = w * h;
   data = new vector3f[size];

   for (int j = 0; j < size; ++j)
   {
      data[j] = i.data[j];
   }
}

Image::~Image()
{
   delete [] data;
}

void Image::SetPixel(float x, float y, const vector3f& color)
{
   if (x < 0.0f)
   {
      x = 0.0f;
   }
   else if (x > 1.0f)
   {
      x = 1.0f;
   }

   if (y < 0.0f)
   {
      y = 0.0f;
   }
   else if (y > 1.0f)
   {
      y = 1.0f;
   }

   int ix = (int) (x * (float) w);
   int iy = (int) (y * (float) h);
   
   data[iy * w + ix] = color;

   return;
}

void Image::SetPixel(int x, int y, const vector3f& color)
{
   if (x < 0)
   {
      x = 0;
   }
   else if (x >= w)
   {
      x = w - 1;
   }

   if (y < 0)
   {
      y = 0;
   }
   else if (y >= h)
   {
      y = h - 1;
   }

   data[y * w + x] = color;

   return;
}

vector3f Image::GetPixel(int x, int y) const
{
   return data[y * w + x];
}

void Image::Save(const char* szFileName, Format t) const
{
   if (szFileName != NULL)
   {
      if (t == TGA)
      {
         SaveTGA(szFileName);
      }
      else if (t == BMP)
      {
         SaveBMP(szFileName);
      }
   }

   return;
}

void Image::SaveTGA(const char* szFileName) const
{
   FILE* file = fopen(szFileName, "wb");

   if (file != NULL)
   {
   /* Header information. */
      for (int i = 0; i < 18; ++i)
      {
         if (i == 2) WriteByte(file, 2);
         else if (i == 12) WriteByte(file, static_cast<unsigned char>(w % 256));
         else if (i == 13) WriteByte(file, static_cast<unsigned char>(w / 256));
         else if (i == 14) WriteByte(file, static_cast<unsigned char>(h % 256));
         else if (i == 15) WriteByte(file, static_cast<unsigned char>(h / 256));
         else if (i == 16) WriteByte(file, static_cast<unsigned char>(24));
         else if (i == 17) WriteByte(file, static_cast<unsigned char>(32));
         else WriteByte(file, 0);
      }

   /* Write the image data. */
      for (int j = h - 1; j >= 0; --j)
      {
         for (int i = 0; i < w; ++i)
         {
            vector3f v = GetPixel(i, j);
            WriteByte(file, ClampColorComponent(v[b]));
            WriteByte(file, ClampColorComponent(v[g]));
            WriteByte(file, ClampColorComponent(v[r]));
         }
      }
      
      fclose(file);
   }

   return;
}

void Image::SaveBMP(const char* szFileName) const
{
#ifdef BITMAP_SUPPORT
   FILE* file = fopen(szFileName, "wb");

   if (file != NULL)
   {
      BITMAPFILEHEADER bmpHeader = {0};  /* Bitmap file header. */
             bmpHeader.bfSize    = sizeof(BITMAPFILEHEADER);
             bmpHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
             bmpHeader.bfType    = BITMAP_ID; /* The magic number.  */

      int pad = (4 - (w * 3) % 4) % 4;

      BITMAPINFOHEADER bmpInfomation = {0}; /* Bitmap infomation header. */
      bmpInfomation.biSize          = sizeof(BITMAPINFOHEADER);
      bmpInfomation.biPlanes        = 1;  /* The only supported value. */
      bmpInfomation.biBitCount      = 24;
      bmpInfomation.biCompression   = BI_RGB;
      bmpInfomation.biWidth         = w;
      bmpInfomation.biHeight        = h;
      bmpInfomation.biSizeImage     = ((w * 3) + pad) * h;
      bmpInfomation.biXPelsPerMeter = 2834;
      bmpInfomation.biYPelsPerMeter = 2834;

      fwrite(&bmpHeader,     sizeof(BITMAPFILEHEADER), 1, file);
      fwrite(&bmpInfomation, sizeof(BITMAPINFOHEADER), 1, file);

   /* Write the image data upside-down. */
      for (int j = 0; j < h; ++j)
      {
         for (int i = 0; i < w; ++i)
         {
            vector3f v = GetPixel(i, j);
            WriteByte(file, ClampColorComponent(v[b]));
            WriteByte(file, ClampColorComponent(v[g]));
            WriteByte(file, ClampColorComponent(v[r]));
         }

         for (int i = 0; i < pad; ++i)
         {
            WriteByte(file, 0);
         }
      }
      
      fclose(file);
   }
#endif
   return;
}

