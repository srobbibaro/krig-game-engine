#ifndef _PLANE_H_
#define _PLANE_H_

#include "Matrix.h"

class Plane
{
    public:
        Plane(void);
        ~Plane(void);
        
        void setPlane(float tA, float tB, float tC, float tD);
        void normalize(void);
        float distanceToPoint(const float &x, const float &y, const float &z);
        int classifyPoint(const float &x, const float &y, const float &z);
        void getNormalVector(Vector &v);
    
    private:
        float a, b, c, d;
};


#endif
