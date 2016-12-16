#include "Sphere.h"

//------------------------------------------------------------------------------
Sphere::Sphere() {
  x = y = z = radius = 0.0f;
}

//------------------------------------------------------------------------------
Sphere::~Sphere() {}

//------------------------------------------------------------------------------
void Sphere::setSphere(const float &tX, const float &tY, const float &tZ, const float &tRadius) {
  x = tX;
  y = tY;
  z = tZ;

  radius = tRadius;
}

//------------------------------------------------------------------------------
void Sphere::getOriginVector(Vector &v) const {
  v.setVector(x, y, z);
}

//------------------------------------------------------------------------------
float Sphere::getRadius() const {
  return (radius);
}
