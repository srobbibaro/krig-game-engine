#include "Sphere.h"

//------------------------------------------------------------------------------
Sphere::Sphere(void) {
  x = y = z = radius = 0.0f;
}

//------------------------------------------------------------------------------
Sphere::~Sphere(void) {}

//------------------------------------------------------------------------------
void Sphere::setSphere(float tX, float tY, float tZ, float tRadius) {
  x = tX;
  y = tY;
  z = tZ;

  radius = tRadius;
}

//------------------------------------------------------------------------------
void Sphere::getOriginVector(Vector &v) {
  v.setVector(x, y, z);
}

//------------------------------------------------------------------------------
float Sphere::getRadius(void) const {
  return (radius);
}
