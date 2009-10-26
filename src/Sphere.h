#ifndef _SPHERE_H_
#define _SPHERE_H_

#include "matrix.h"
#include "Vector.h"

class Sphere
{
    public:
        Sphere(void);
        ~Sphere(void);

        void setSphere(float tX, float tY, float tZ, float tRadius);

        void getOriginVector(Vector &v);
        float getRadius(void) const;
        void setOriginVector(Vector &v) { x = v.x; y = v.y; z = v.z; }

    private:
        float x, y, z;  // origin
        float radius;
};


#endif

