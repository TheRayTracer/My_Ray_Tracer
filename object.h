/* File: object.h; Mode: C++; Tab-width: 3; Author: Simon Flannery;           */

#ifndef OBJECT_H
#define OBJECT_H

#include <float.h>
#include "math.h"

class Ray;
class Hit;
class Material;

class Object
{
public:
   virtual bool Intersect(const Ray& ray, Hit& h, float tmin) const = 0;
   virtual bool Intersect(const Ray& ray, Hit& h1, Hit& h2, float tmin) const= 0;
   virtual bool ShadowIntersect(const Ray& ray, Hit& h, float tmin) const = 0;

protected:
   Material* material;

private:
};

class Sphere : public Object
{
public:
   Sphere(const vector3f& p, float r, Material* m);

   virtual bool Intersect(const Ray& ray, Hit& h, float tmin) const;
   virtual bool Intersect(const Ray& ray, Hit& h1, Hit& h2, float tmin) const;
   virtual bool ShadowIntersect(const Ray& ray, Hit& h, float tmin) const;

protected:
   vector3f point;
   float radius;

private:
};

class MotionSphere : public Sphere
{
public:
   MotionSphere(const vector3f& p, float r, const vector3f& v, Material* m);

   virtual bool Intersect(const Ray& ray, Hit& h, float tmin) const;

protected:
private:
   vector3f velocity;
};

class Plane : public Object
{
public:
   Plane(const vector3f& n, float offset, Material* m);

   virtual bool Intersect(const Ray& ray, Hit& h, float tmin) const;
   virtual bool Intersect(const Ray& ray, Hit& h1, Hit& h2, float tmin) const;
   virtual bool ShadowIntersect(const Ray& ray, Hit& h, float tmin) const;

protected:
private:
   float d;
   vector3f normal;
};

class Triangle : public Object
{
public:
   Triangle(const vector3f& a, const vector3f& b, const vector3f& c, Material* m);

   virtual bool Intersect(const Ray& ray, Hit& h, float tmin) const;
   virtual bool Intersect(const Ray& ray, Hit& h1, Hit& h2, float tmin) const;
   virtual bool ShadowIntersect(const Ray& ray, Hit& h, float tmin) const;

protected:
private:
   vector3f va, vb, vc, ea, eb;
   vector3f normal;
};

class XYRectangle : public Object
{
public:
   XYRectangle(const vector2f low, const vector2f up, const float _k, Material* m);

   virtual bool Intersect(const Ray& ray, Hit& h, float tmin) const;
   virtual bool Intersect(const Ray& ray, Hit& h1, Hit& h2, float tmin) const;
   virtual bool ShadowIntersect(const Ray& ray, Hit& h, float tmin) const;

protected:
private:
   float k; // k = z in (x, y, z)
   vector2f lower, upper;
   vector3f normal;
};

class XZRectangle : public Object
{
public:
   XZRectangle(const vector2f low, const vector2f up, const float _k, Material* m);

   virtual bool Intersect(const Ray& ray, Hit& h, float tmin) const;
   virtual bool Intersect(const Ray& ray, Hit& h1, Hit& h2, float tmin) const;
   virtual bool ShadowIntersect(const Ray& ray, Hit& h, float tmin) const;

protected:
private:
   float k; // k = y in (x, y, z)
   vector2f lower, upper;
   vector3f normal;
};

class YZRectangle : public Object
{
public:
   YZRectangle(const vector2f low, const vector2f up, const float _k, Material* m);

   virtual bool Intersect(const Ray& ray, Hit& h, float tmin) const;
   virtual bool Intersect(const Ray& ray, Hit& h1, Hit& h2, float tmin) const;
   virtual bool ShadowIntersect(const Ray& ray, Hit& h, float tmin) const;

protected:
private:
   float k; // k = x in (x, y, z)
   vector2f lower, upper;
   vector3f normal;
};

class Cube : public Object
{
public:
   Cube(const vector3f& p, float size, Material* m);
   Cube(const vector3f& p, const vector3f& z, Material* m);

   virtual bool Intersect(const Ray& ray, Hit& h, float tmin) const;
   virtual bool Intersect(const Ray& ray, Hit& h1, Hit& h2, float tmin) const;
   virtual bool ShadowIntersect(const Ray& ray, Hit& h, float tmin) const;

protected:
private:
   vector3f max, min;
};

class Group : public Object
{
public:
   Group(size_t s);
   ~Group();

   virtual bool Intersect(const Ray& ray, Hit& h, float tmin) const;
   virtual bool Intersect(const Ray& ray, Hit& h1, Hit& h2, float tmin) const;
   virtual bool ShadowIntersect(const Ray& ray, Hit& h, float tmin) const;

   void SetAt(size_t i, Object* obj);
   size_t GetSize() {   return size;   }

   void SetBB(const vector3f& vmin, const vector3f& vmax);

protected:
   size_t size;
   Object** object;

private:
   bool PossibleHit(const Ray& ray, float tmin) const;
   vector3f bb_vmin, bb_vmax;
};

class CSGGroup : public Group
{
public:
   enum Type { Union, Intersection, Difference };

   CSGGroup(int s, Type t = Type::Union);
   ~CSGGroup();

   virtual bool Intersect(const Ray& ray, Hit& h, float tmin) const;
   void SetType(Type t) { type = t;   return; };

protected:
private:
   Type type;
};

class Transform : public Object
{
public:
   Transform(const Matrix& m, Object* o);

   virtual bool Intersect(const Ray& ray, Hit& h, float tmin) const;
   virtual bool Intersect(const Ray& ray, Hit& h1, Hit& h2, float tmin) const;
   virtual bool ShadowIntersect(const Ray& ray, Hit& h, float tmin) const;

protected:
private:
   Matrix matrix;
   Object* object;
};

#endif