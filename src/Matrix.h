/**
 * @file Matrix.h
 * @brief Representation of 4x4 matrix with common matrix operations.
 *
 * Stored in column-major order in `data[]` (OpenGL-style). The 16-float constructor
 * takes four columns: (c11,c21,c31,c41), (c12,...), … not four rows on paper.
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
    /** All 16 components: column 1 (top to bottom), then column 2, … column 4. */
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
    void setTranslation(const float &, const float &, const float &);
    void setScale(const float &, const float &, const float &);
    void setShadow(float lightPos[4], float plane[4]);
    void transformVertex(float* , float*);
    void display();

    void fix();
};

#endif
