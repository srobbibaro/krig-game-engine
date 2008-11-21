#include "Plane.h"

//------------------------------------------------------------------------------
Plane::Plane(void)
{
    a = 1.0f;
    b = 0.0f;
    c = 0.0f;
    d = 0.0f;
}

//------------------------------------------------------------------------------
Plane::~Plane(void)
{
}

//------------------------------------------------------------------------------
void Plane::setPlane(float tA, float tB, float tC, float tD)
{
    a = tA;
    b = tB;
    c = tC;
    d = tD;
}

//------------------------------------------------------------------------------
void Plane::normalize(void)
{
    float len = sqrt((a * a) + (b * b) + (c * c));
    
    if (len != 0.0f) {
        a = (a / len);
        b = (b / len);
        c = (c / len);
        d = (d / len);
    }
}

//------------------------------------------------------------------------------
float Plane::distanceToPoint(const float &x, const float &y, const float &z)
{
    return ((a * x) + (b * y) + (c * z) + d);
}

//------------------------------------------------------------------------------
int Plane::classifyPoint(const float &x, const float &y, const float &z)
{
    float len = (a * x) + (b * y) + (c * z) + d;
    
    if (len < 0) 
        return (-1);
    else if (len > 0) 
        return (1);
    else
        return (0);
}

//------------------------------------------------------------------------------
void Plane::getNormalVector(Vector &v)
{
    v.setVector(a, b, c);
}



