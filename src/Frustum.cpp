#include "Frustum.h"

//------------------------------------------------------------------------------
Frustum::Frustum(void) {}

//------------------------------------------------------------------------------
Frustum::~Frustum(void) {}

//------------------------------------------------------------------------------
void Frustum::extractFromProjectionMatrix(const Matrix &m)
{
    // left clipping plane
    planes[0].setPlane(
        (m.data[3] + m.data[0]),
        (m.data[7] + m.data[4]),
        (m.data[11] + m.data[8]),
        (m.data[15] + m.data[12])
    );

    // right clipping plane
    planes[1].setPlane(
        (m.data[3] - m.data[0]),
        (m.data[7] - m.data[4]),
        (m.data[11] - m.data[8]),
        (m.data[15] - m.data[12])
    );

    // bottom clipping plane
    planes[2].setPlane(
        (m.data[3] + m.data[1]),
        (m.data[7] + m.data[5]),
        (m.data[11] + m.data[9]),
        (m.data[15] + m.data[13])
    );

    // top clipping plane
    planes[3].setPlane(
        (m.data[3] - m.data[1]),
        (m.data[7] - m.data[5]),
        (m.data[11] - m.data[9]),
        (m.data[15] - m.data[13])
    );

    // far clipping plane
    planes[4].setPlane(
        (m.data[3] - m.data[2]),
        (m.data[7] - m.data[6]),
        (m.data[11] - m.data[10]),
        (m.data[15] - m.data[14])
    );

    // near clipping plane
    planes[5].setPlane(
        (m.data[3] + m.data[2]),
        (m.data[7] + m.data[6]),
        (m.data[11] + m.data[10]),
        (m.data[15] + m.data[14])
    );

    for (int i = 0; i < NUM_PLANES; i++) {
        planes[i].normalize();
    }
}

//------------------------------------------------------------------------------
int Frustum::testSphere(Sphere sphere)
{
    float distance;
    Vector origin;

    // calculate distance between sphere and each plane //
	for(int i = 0; i < NUM_PLANES; i++) {
        sphere.getOriginVector(origin);

        distance = planes[i].distanceToPoint(origin.x, origin.y, origin.z);

		// if this distance is < -sphere.radius, we are outside
		if(distance < -sphere.getRadius())
			return(-1);

		// else if the distance is between +- radius, then we intersect
		if((float)fabs(distance) < sphere.getRadius())
			return(0);
	}

	// otherwise we are fully in view
	return(1);
}

//------------------------------------------------------------------------------
int Frustum::testBoundingBox(void) {}

//------------------------------------------------------------------------------
void Frustum::getPlaneDefinition(int num, float &a, float &b, float &c, float &d)
{
    if (num >= 0 && num < NUM_PLANES) {
        planes[num].getDefinition(a, b, c, d);
    }
}
