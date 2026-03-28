//////////////////////////////////////////////////////////////////
// Description : Functions to handle matrix multiplication for  //
//               object transformations.                        //
//////////////////////////////////////////////////////////////////
#include "Matrix.h"
#include "constants.h"

//------------------------------------------------------------------------------
Matrix::Matrix() {
  loadIdentity();
}

//------------------------------------------------------------------------------
Matrix::Matrix( const Matrix &t ) {
  for ( int i = 0; i < NUM_CELLS; ++i )
    data[i] = t.data[i];
}

//------------------------------------------------------------------------------
Matrix::Matrix(
  const float &c11, const float &c21, const float &c31, const float &c41,
  const float &c12, const float &c22, const float &c32, const float &c42,
  const float &c13, const float &c23, const float &c33, const float &c43,
  const float &c14, const float &c24, const float &c34, const float &c44
) {
  data[0]  = c11; data[1]  = c12; data[2]  = c13; data[3]  = c14;
  data[4]  = c21; data[5]  = c22; data[6]  = c23; data[7]  = c24;
  data[8]  = c31; data[9]  = c32; data[10] = c33; data[11] = c34;
  data[12] = c41; data[13] = c42; data[14] = c43; data[15] = c44;
}

//------------------------------------------------------------------------------
void Matrix::loadIdentity() {
  for ( int i = 0; i < NUM_CELLS; ++i )
    data[i] = 0;

  data[0]  = 1;
  data[5]  = 1;
  data[10] = 1;
  data[15] = 1;
}

//------------------------------------------------------------------------------
void Matrix::loadZero() {
  for ( int i = 0; i < NUM_CELLS; ++i )
    data[i] = 0;
}

//------------------------------------------------------------------------------
void Matrix::operator =(const Matrix &t) {
  for ( int i = 0; i < NUM_CELLS; ++i )
    data[i] = t.data[i];
}

//------------------------------------------------------------------------------
Matrix Matrix::operator *(const Matrix &p) {
  Matrix t; int c, r;

  for ( int i = 0; i < NUM_CELLS; ++i ) {
    t.data[i] = 0.0f;

    c = (int) i / 4; r = i % 4;
    for ( int j = 0; j < 4; j++ )
      t.data[i] += ( p.data[(c*4)+j] * data[ (j*4) + r ] );
  }

  return (t);
}

//------------------------------------------------------------------------------
void Matrix::setTranslation(const float &x, const float &y, const float &z) {
  loadIdentity();

  data[12] = x;
  data[13] = y;
  data[14] = z;
}

//------------------------------------------------------------------------------
void Matrix::setScale(const float &x, const float &y, const float &z) {
  loadIdentity();

  data[0]  = x;
  data[5]  = y;
  data[10] = z;
}

//------------------------------------------------------------------------------
void Matrix::transpose(Matrix &m) {
  for ( int j = 0; j < 3; j++ ) {
    for (int i = 0; i < (4-j); ++i) {
      m.data[(i*4)+j] = data[15-(j*4)-i];
      m.data[15-(j*4)-i] = data[(i*4)+j];
    }
  }
}

//------------------------------------------------------------------------------
void Matrix::transformVertex(float *v, float *newVertex) {
  float h, x, y, z;

  x = ( v[0] * data[0] ) + ( v[1] * data[4] ) + ( v[2] * data[8] ) + data[12];
  y = ( v[0] * data[1] ) + ( v[1] * data[5] ) + ( v[2] * data[9] ) + data[13];
  z = ( v[0] * data[2] ) + ( v[1] * data[6] ) + ( v[2] * data[10] ) + data[14];
  h = ( v[0] * data[3] ) + ( v[1] * data[7] ) + ( v[2] * data[11] ) + data[15];

  newVertex[0] = x; newVertex[1] = y; newVertex[2] = z;

  newVertex[0] /= h;
  newVertex[1] /= h;
  newVertex[2] /= h;
}

//------------------------------------------------------------------------------
void Matrix::setShadow(float lightPos[4], float plane[4]) {
  float temp = plane[0] * lightPos[0] +
               plane[1] * lightPos[1] +
               plane[1] * lightPos[2] +
               plane[3] * lightPos[3];

  data[0] = temp - lightPos[0] * plane[0];
  data[4] = 0.0f - lightPos[0] * plane[1];
  data[8] = 0.0f - lightPos[0] * plane[2];
  data[12] = 0.0f - lightPos[0] * plane[3];

  data[1] = 0.0f - lightPos[1] * plane[0];
  data[5] = temp - lightPos[1] * plane[1];
  data[9] = 0.0f - lightPos[1] * plane[2];
  data[13] = 0.0f - lightPos[1] * plane[3];

  data[2] = 0.0f - lightPos[2] * plane[0];
  data[6] = 0.0f - lightPos[2] * plane[1];
  data[10] = temp - lightPos[2] * plane[2];
  data[14] = 0.0f - lightPos[2] * plane[3];

  data[3] = 0.0f - lightPos[3] * plane[0];
  data[7] = 0.0f - lightPos[3] * plane[1];
  data[11] = 0.0f - lightPos[3] * plane[2];
  data[15] = temp - lightPos[3] * plane[3];
}

//------------------------------------------------------------------------------
void Matrix::display() {
#if DEBUG
  PRINT_DEBUG("Matrix dump:\n");
  for ( int i = 0; i < NUM_CELLS; ++i ) {
    PRINT_DEBUG("%.3f\t", data[i] );
    if ( (i + 1) % 4 == 0 )
      PRINT_DEBUG( "\n" );
  }
#endif
}
