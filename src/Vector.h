//////////////////////////////////////////////////////////////////
// Description : Class for handling vector math.                //
//////////////////////////////////////////////////////////////////
#include <math.h>
#include <stdio.h>

#include "matrix.h"

#ifndef _VECTOR_H_
#define _VECTOR_H_

struct Vector {
  Vector();
  Vector(const float &, const float &, const float &);
  void setVector(const float &, const float &, const float &);

  void normalize();
  float dotProduct(const Vector &);
  void crossProduct(const Vector &, const Vector &);
  float getDistance(const Vector &);
  float getLength(void);
  float getScaler(const Vector &);
  float getSum(void);
  void scale(float);
  void average(const Vector &, const Vector &);
  void calcNorm(const Vector &, const Vector &, const Vector &);

  Vector operator *(float);
  Vector operator +(const Vector &);
  void operator =(const Vector &);
  void operator +=(const Vector &);
  void operator -=(const Vector &);

  void rotateVector(const Matrix &, const Vector &);
  void transformVector(const Matrix &, const Vector &);

  bool intersectBox(const Vector &, Vector[], float, Vector &);
  bool intersectBox(const Vector &, Vector[], float);

  float x, y, z;
};

#endif
