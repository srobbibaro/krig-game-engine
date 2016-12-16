#ifndef _SPHERE_H_
#define _SPHERE_H_

#include "matrix.h"
#include "Vector.h"

class Sphere {
  public:
    Sphere();
    ~Sphere();

    void setSphere(const float&, const float&, const float&, const float&);

    void getOriginVector(Vector&) const;
    float getRadius() const;
    void setOriginVector(const Vector &v) { x = v.x; y = v.y; z = v.z; }

  private:
    float x, y, z;  // origin
    float radius;
};

#endif
