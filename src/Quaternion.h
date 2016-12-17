#ifndef _QUATERNION_H_
#define _QUATERNION_H_

#include "Matrix.h"
#include "Vector.h"

class Quaternion {
  public:
    Quaternion();
    Quaternion(const Vector&);
    Quaternion(const float&, const float&, const float&);
    Quaternion(const Vector&, const float&);
    Quaternion(const float&, const float&, const float&, const float&);
    Quaternion(const Quaternion &);
    ~Quaternion() {}

    void normalize();

    void buildFromEuler(const Vector&);
    void buildFromEuler(const float&, const float&, const float&);
    void buildFromAxis(const Vector&, const float&);
    void buildFromAxis(const float&, const float&, const float&);

    void loadMultIdentity();
    void loadAddIdentity();

    void getEulerAngles(Vector&);

    void buildRotationMatrix(Matrix&);
    void rotate(const float&);
    void slerp (const Quaternion &, const float&, const Quaternion&);

    // accessor functions
    float getX() const { return(x); }
    float getY() const { return(y); }
    float getZ() const { return(z); }
    float getW() const { return(w); }

    // overloaded operators
    Quaternion operator+(const Quaternion&);
    Quaternion operator*(const Quaternion&);
    Quaternion&	operator=(const Quaternion&);

  private:
    float x, y, z, w;
};

#endif
