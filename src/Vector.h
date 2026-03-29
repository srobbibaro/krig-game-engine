/**
 * @file Vector.h
 * @brief Represent and operate on vectors.
 *
 * Vector math.
 */
#ifndef _VECTOR_H_
#define _VECTOR_H_

#include <cmath>
#include <cstdio>

#include "Matrix.h"

struct Vector {
  Vector();
  Vector(const float &, const float &, const float &);
  void setVector(const float &, const float &, const float &);

  void normalize();
  float dotProduct(const Vector &);
  void crossProduct(const Vector &, const Vector &);
  float getDistance(const Vector &);
  float getLength();
  float getScaler(const Vector &);
  float getSum();
  void scale(const float&);
  void average(const Vector &, const Vector &);
  void calcNorm(const Vector &, const Vector &, const Vector &);

  Vector operator *(float);
  Vector operator +(const Vector &);
  void operator =(const Vector &);
  void operator +=(const Vector &);
  void operator -=(const Vector &);

  void rotateVector(const Matrix &, const Vector &);
  void transformVector(const Matrix &, const Vector &);

  // `this` is the ray direction. Returns true if the ray from rayOrigin
  // hits the AABB defined by box[0] (min) and box[1] (max), expanded by
  // margin on all sides. Sets hitPoint to the entry point, or to rayOrigin
  // if the origin is inside the box. Returns false for a zero direction.
  bool intersectBox(const Vector &rayOrigin, Vector box[], float margin, Vector &hitPoint);
  bool intersectBox(const Vector &rayOrigin, Vector box[], float margin);

  float x, y, z;
};

#endif
