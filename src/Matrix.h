/**
 * @file Matrix.h
 * @brief Representation of 4x4 matrix with common matrix operations.
 */
#ifndef _MATRIX_H_
#define _MATRIX_H_

#include <math.h>
#include <stdio.h>

#define NUM_CELLS 16 // 4 X 4

class Matrix {
  public:
    float data[NUM_CELLS];

    Matrix();
    Matrix(const Matrix&);
    Matrix(
      const float &, const float &, const float &, const float &,
      const float &, const float &, const float &, const float &,
      const float &, const float &, const float &, const float &,
      const float &, const float &, const float &, const float &
    );

    void loadIdentity();
    void loadZero();

    void operator =(const Matrix &);
    Matrix operator *(const Matrix &);

    void transpose(Matrix&);
    void setRotation(const float &, const float &, const float &, const float &);
    void setRotation2(const float &, const float &, const float &);
    void setTranslation(const float &, const float &, const float &);
    void setScale(const float &, const float &, const float &);
    void setShadow(float lightPos[4], float plane[4]);
    void transformVertex(float* , float*);
    void display();

    void CreateFromAxisAngle(const float &, const float &, const float &, const float &);
    void CreateFromAxisAngles(const float &, const float &, const float &, const float &);

    void fix();
};

#endif
