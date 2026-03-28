/**
 * @file Frustum.h
 * @brief Representation of the view's Frustum.
 *
 * Used for testing whether game objects are viewable.
 */
#ifndef _FRUSTUM_H_
#define _FRUSTUM_H_

#include "Plane.h"
#include "Sphere.h"

#define NUM_PLANES  6

class Frustum {
  public:
    Frustum();
    ~Frustum();

    void extractFromProjectionMatrix(const Matrix& m);
    void getPlaneDefinition(const int&, float&, float&, float&, float&);

    /**
     * Tests whether a sphere is inside, intersecting, or outside the frustum.
     * Returns 1 (inside all planes), 0 (intersecting one or more planes), or
     * -1 (outside at least one plane). Both engine callers treat r != -1 as
     * visible. The check is conservative: the loop returns 0 on the first
     * intersecting plane without testing remaining planes, so a sphere that
     * intersects one plane but is outside another may return 0 instead of -1,
     * causing it to be rendered rather than culled.
     */
    int testSphere(const Sphere& sphere);
    int testBoundingBox();

  private:
    // 0 - left, 1 - right, 2 - bottom, 3 - top, 4 - far, 5 - near
    Plane planes[NUM_PLANES];
};

#endif
