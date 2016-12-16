#ifndef _FRUSTUM_H_
#define _FRUSTUM_H_

#include "Plane.h"
#include "Sphere.h"

#define NUM_PLANES  6

class Frustum {
  public:
    Frustum();
    ~Frustum();

    void extractFromProjectionMatrix(const Matrix&);
    void getPlaneDefinition(const int&, float&, float&, float&, float&);

    int testSphere(const Sphere &sphere);
    int testBoundingBox();

  private:
    // 0 - left, 1 - right, 2 - down, 3 - up, 4 - far, 5 - near
    Plane planes[NUM_PLANES];
};

#endif
