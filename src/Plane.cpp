#include "Plane.h"

//------------------------------------------------------------------------------
Plane::Plane() {
  a_ = 1.0f;
  b_ = 0.0f;
  c_ = 0.0f;
  d_ = 0.0f;
}

//------------------------------------------------------------------------------
Plane::~Plane() {}

//------------------------------------------------------------------------------
void Plane::setPlane(const float &a, const float &b, const float &c, const float &d) {
  a_ = a; b_ = b; c_ = c; d_ = d;
}

//------------------------------------------------------------------------------
void Plane::normalize() {
  float len = sqrt((a_ * a_) + (b_ * b_) + (c_ * c_));

  if (len != 0.0f) {
    a_ = (a_ / len);
    b_ = (b_ / len);
    c_ = (c_ / len);
    d_ = (d_ / len);
  }
}

//------------------------------------------------------------------------------
float Plane::distanceToPoint(const float &x, const float &y, const float &z) {
  return ((a_ * x) + (b_ * y) + (c_ * z) + d_);
}

//------------------------------------------------------------------------------
int Plane::classifyPoint(const float &x, const float &y, const float &z) {
  float len = (a_ * x) + (b_ * y) + (c_ * z) + d_;

  if (len < 0)
    return (-1);
  else if (len > 0)
    return (1);
  else
    return (0);
}

//------------------------------------------------------------------------------
void Plane::getNormalVector(Vector &v) const {
  v.setVector(a_, b_, c_);
}

//------------------------------------------------------------------------------
void Plane::getDefinition(float &a, float &b, float &c, float &d) const {
  a = a_; b = b_; c = c_; d = d_;
}
