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
  Vector(float, float, float);
  void setVector(float, float, float);

  void normalize();
  float dotProduct(Vector &);
  void crossProduct(Vector &, Vector &);
  float getDistance(Vector);
  float getLength(void);
  float getScaler(Vector);
  float getSum(void);
  void scale(float);
  void average(Vector &, Vector &);
  void calcNorm(Vector &, Vector &, Vector &);

  Vector operator *(float);
  Vector operator +(Vector);
  void operator =(Vector);
  void operator +=(Vector);
  void operator -=(Vector);

  void rotateVector(Matrix &, Vector &);
  void transformVector(Matrix &, Vector &);

  bool intersectBox(Vector, Vector[], float, Vector &);
  bool intersectBox(Vector, Vector[], float);

  float x, y, z;
};

#endif
