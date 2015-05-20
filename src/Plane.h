#ifndef _PLANE_H_
#define _PLANE_H_

#include "matrix.h"
#include "Vector.h"

class Plane {
  public:
    Plane(void);
    ~Plane(void);

    void setPlane(float a, float b, float c, float d);
    void normalize(void);
    float distanceToPoint(const float &x, const float &y, const float &z);
    int classifyPoint(const float &x, const float &y, const float &z);
    void getNormalVector(Vector &v);
    void getDefinition(float &a, float &b, float &c, float &d);

  private:
    float a_, b_, c_, d_;
};

#endif
