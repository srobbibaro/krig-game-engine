#ifndef _FRUSTUM_H_
#define _FRUSTUM_H_

#include "Plane.h"
#include "Sphere.h"

using namespace std;

class Frustum
{
    public:
        Frustum(void);
        ~Frustum(void);

        void extractFromProjectionMatrix(const Matrix &m);
        int testSphere(Sphere sphere);
        int testBoundingBox(void);

        void getPlaneDefinition(int num, float &a, float &b, float &c, float &d);

    private:
        // 0 - left, 1 - right, 2 - down, 3 - up, 4 - far, 5 - near
        Plane planes[6];
};

#endif
