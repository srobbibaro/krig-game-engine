#ifndef _SPHERE_H_
#define _SPHERE_H_

#include "Matrix.h"

class Sphere
{
    public:
        Sphere(void);
        ~Sphere(void);
        
        void setSphere(float tX, float tY, float tZ, float tRadius);
        
        void getOriginVector(Vector &v);
        float getRadius(void) const;
        
    private:
        float x, y, z;  // origin
        float radius;
};


#endif

