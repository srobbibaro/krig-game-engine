/**
 * @file Quaternion.h
 * @brief Quaternion representation for 3D rotations.
 *
 * Quaternions are used throughout the engine for all rotation operations.
 * The standard pipeline is: build with buildFromEuler or buildFromAxis,
 * compose rotations with operator*, then emit a column-major rotation
 * matrix with buildRotationMatrix for use with OpenGL.
 *
 * See: https://en.wikipedia.org/wiki/Quaternion
 */
#ifndef _QUATERNION_H_
#define _QUATERNION_H_

#include "Matrix.h"
#include "Vector.h"

class Quaternion {
  public:
    Quaternion();
    /** Builds from Euler angles stored in v (radians). */
    Quaternion(const Vector&);
    /** Builds from Euler angles x, y, z (radians). */
    Quaternion(const float&, const float&, const float&);
    /**
     * Builds from an axis-angle pair. axis must be a unit vector;
     * angle is in radians.
     */
    Quaternion(const Vector&, const float&);
    Quaternion(const float&, const float&, const float&, const float&);
    Quaternion(const Quaternion &);
    ~Quaternion() {}

    /** Scales to unit length. No-op if the quaternion has zero length. */
    void normalize();

    /** Sets this quaternion from Euler angles in radians (x=pitch, y=yaw, z=roll). */
    void buildFromEuler(const Vector&);
    /** Sets this quaternion from Euler angles in radians (x=pitch, y=yaw, z=roll). */
    void buildFromEuler(const float&, const float&, const float&);
    /**
     * Sets this quaternion from an axis-angle pair. axis must be a unit
     * vector; angle is in radians. Does not normalise the result.
     */
    void buildFromAxis(const Vector&, const float&);

    void loadMultIdentity();
    void loadAddIdentity();

    /**
     * Extracts Euler angles into v (radians). Uses a gimbal-lock fallback
     * when the rotation matrix element m.data[2] reaches ±1; in that case
     * v.z is forced to zero. Not a guaranteed roundtrip with buildFromEuler
     * for all inputs.
     */
    void getEulerAngles(Vector&);

    void buildRotationMatrix(Matrix&);

    /**
     * Spherical linear interpolation from sQ to eQ at parameter t.
     * t must be in [0, 1]; values outside that range leave this
     * quaternion unchanged.
     */
    void slerp(const Quaternion &, const float&, const Quaternion&);

    // accessor functions
    float getX() const { return(x); }
    float getY() const { return(y); }
    float getZ() const { return(z); }
    float getW() const { return(w); }

    // overloaded operators
    Quaternion operator+(const Quaternion&) const;
    /** Composes two quaternion rotations using the engine's multiplication convention. */
    Quaternion operator*(const Quaternion&) const;
    Quaternion&	operator=(const Quaternion&);

  private:
    float x, y, z, w;
};

#endif
