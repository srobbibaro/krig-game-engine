/**
 * @file Plane.h
 * @brief Representation of a plane.
 *
 * Store and perform operations on a plane.
 */
#ifndef _PLANE_H_
#define _PLANE_H_

#include "Matrix.h"
#include "Vector.h"

class Plane {
  public:
    Plane();
    ~Plane();

    void setPlane(const float&, const float&, const float&, const float&);
    void normalize();
    float distanceToPoint(const float&, const float&, const float&);
    int classifyPoint(const float&, const float&, const float&);
    void getNormalVector(Vector &);
    void getDefinition(float&, float&, float&, float&);

  private:
    float a_, b_, c_, d_;
};

#endif
