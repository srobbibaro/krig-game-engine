#include "catch.hpp"
#include "Quaternion.h"
#include "Vector.h"
#include <cmath>

SCENARIO( "Quaternion basics", "[Quaternion]" ) {
  GIVEN( "Default quaternion" ) {
    Quaternion q;
    THEN( "all components are zero" ) {
      REQUIRE(q.getX() == 0.0f);
      REQUIRE(q.getY() == 0.0f);
      REQUIRE(q.getZ() == 0.0f);
      REQUIRE(q.getW() == 0.0f);
    }
  }

  GIVEN( "loadMultIdentity" ) {
    Quaternion q(1.0f, 2.0f, 3.0f, 4.0f);
    q.loadMultIdentity();
    THEN( "is multiplicative identity" ) {
      REQUIRE(q.getX() == 0.0f);
      REQUIRE(q.getY() == 0.0f);
      REQUIRE(q.getZ() == 0.0f);
      REQUIRE(q.getW() == 1.0f);
    }
  }

  GIVEN( "loadAddIdentity" ) {
    Quaternion q(1.0f, 2.0f, 3.0f, 4.0f);
    q.loadAddIdentity();
    THEN( "is additive zero quaternion" ) {
      REQUIRE(q.getX() == 0.0f);
      REQUIRE(q.getY() == 0.0f);
      REQUIRE(q.getZ() == 0.0f);
      REQUIRE(q.getW() == 0.0f);
    }
  }

  GIVEN( "Quaternion from explicit components" ) {
    Quaternion q(0.0f, 0.0f, 0.0f, 2.0f);
    q.normalize();
    THEN( "normalize yields unit w" ) {
      REQUIRE(q.getW() == Approx(1.0f));
      REQUIRE(q.getX() == Approx(0.0f));
      REQUIRE(q.getY() == Approx(0.0f));
      REQUIRE(q.getZ() == Approx(0.0f));
    }
  }

  GIVEN( "normalize of a zero-length quaternion" ) {
    Quaternion q;  // default: all components zero, len == 0
    q.normalize();
    THEN( "all components remain zero — no-op guard prevents divide by zero" ) {
      REQUIRE(q.getX() == 0.0f);
      REQUIRE(q.getY() == 0.0f);
      REQUIRE(q.getZ() == 0.0f);
      REQUIRE(q.getW() == 0.0f);
    }
  }

  GIVEN( "float constructor delegates to buildFromEuler" ) {
    Quaternion fromCtor(0.0f, 1.0f, 0.0f);  // Quaternion(float, float, float)
    Quaternion fromMethod;
    fromMethod.buildFromEuler(0.0f, 1.0f, 0.0f);
    THEN( "constructor and explicit buildFromEuler produce identical results" ) {
      REQUIRE(fromCtor.getX() == Approx(fromMethod.getX()));
      REQUIRE(fromCtor.getY() == Approx(fromMethod.getY()));
      REQUIRE(fromCtor.getZ() == Approx(fromMethod.getZ()));
      REQUIRE(fromCtor.getW() == Approx(fromMethod.getW()));
    }
  }

  GIVEN( "Vector constructor delegates to buildFromEuler" ) {
    Vector v(0.0f, 1.0f, 0.0f);
    Quaternion fromCtor(v);  // Quaternion(const Vector&)
    Quaternion fromMethod;
    fromMethod.buildFromEuler(0.0f, 1.0f, 0.0f);
    THEN( "constructor and explicit buildFromEuler produce identical results" ) {
      REQUIRE(fromCtor.getX() == Approx(fromMethod.getX()));
      REQUIRE(fromCtor.getY() == Approx(fromMethod.getY()));
      REQUIRE(fromCtor.getZ() == Approx(fromMethod.getZ()));
      REQUIRE(fromCtor.getW() == Approx(fromMethod.getW()));
    }
  }

  GIVEN( "buildFromEuler with zero angles" ) {
    Quaternion q;
    q.buildFromEuler(0.0f, 0.0f, 0.0f);
    THEN( "matches identity rotation" ) {
      REQUIRE(q.getW() == Approx(1.0f));
      REQUIRE(q.getX() == Approx(0.0f));
      REQUIRE(q.getY() == Approx(0.0f));
      REQUIRE(q.getZ() == Approx(0.0f));
    }
  }

  GIVEN( "buildRotationMatrix from multiplicative identity" ) {
    Quaternion q;
    q.loadMultIdentity();
    Matrix m;
    q.buildRotationMatrix(m);

    THEN( "result is the identity matrix" ) {
      REQUIRE(m.data[0]  == Approx(1.0f));
      REQUIRE(m.data[5]  == Approx(1.0f));
      REQUIRE(m.data[10] == Approx(1.0f));
      REQUIRE(m.data[15] == Approx(1.0f));
      REQUIRE(m.data[1]  == Approx(0.0f));
      REQUIRE(m.data[2]  == Approx(0.0f));
      REQUIRE(m.data[4]  == Approx(0.0f));
      REQUIRE(m.data[6]  == Approx(0.0f));
      REQUIRE(m.data[9]  == Approx(0.0f));
    }
  }

  GIVEN( "buildFromAxis on X by 90 degrees" ) {
    Vector axis(1.0f, 0.0f, 0.0f);
    const float halfPi = 1.57079632679f;
    Quaternion q(axis, halfPi);
    q.normalize();
    Matrix m;
    q.buildRotationMatrix(m);
    Vector euler;
    q.getEulerAngles(euler);

    THEN( "rotation matrix for 90 deg about X maps Y to Z" ) {
      REQUIRE(m.data[0] == Approx(1.0f));
      REQUIRE(m.data[5] == Approx(0.0f));
      REQUIRE(m.data[9] == Approx(-1.0f));
      REQUIRE(m.data[6] == Approx(1.0f));
    }

    THEN( "euler extraction returns finite angles" ) {
      REQUIRE(std::isfinite(euler.x));
      REQUIRE(std::isfinite(euler.y));
      REQUIRE(std::isfinite(euler.z));
    }
  }

  GIVEN( "getEulerAngles at 90 degrees about Y (gimbal lock)" ) {
    // 90 deg about Y produces m.data[2] = -1, which is outside the epsilon
    // band and enters the gimbal-lock else branch.  v.z is forced to 0;
    // v.y and v.x are computed from atan2 with exact inputs.
    const float halfPi = 1.57079632679f;
    Quaternion q(Vector(0.0f, 1.0f, 0.0f), halfPi);
    q.normalize();
    Vector euler;
    q.getEulerAngles(euler);

    THEN( "euler matches analytic gimbal-lock result (0, pi/2, 0)" ) {
      REQUIRE(euler.x == Approx(0.0f));
      REQUIRE(euler.y == Approx(halfPi));
      REQUIRE(euler.z == Approx(0.0f));
    }
  }

  GIVEN( "Copy constructor and assignment" ) {
    Quaternion a(1.0f, 2.0f, 3.0f, 4.0f);
    Quaternion b(a);
    Quaternion c;
    c = a;

    THEN( "copies match source" ) {
      REQUIRE(b.getX() == a.getX());
      REQUIRE(c.getW() == a.getW());
    }
  }

  GIVEN( "Quaternion multiplication composes rotations" ) {
    Quaternion q1;
    q1.buildFromEuler(0.0f, 0.0f, 0.0f);
    Quaternion q2;
    q2.loadMultIdentity();
    Quaternion r = q1 * q2;
    THEN( "identity times identity" ) {
      REQUIRE(r.getW() == Approx(1.0f));
      REQUIRE(r.getX() == Approx(0.0f));
    }
  }

  GIVEN( "90 degrees about X composed with 90 degrees about Y" ) {
    const float halfPi = 1.57079632679f;
    Quaternion q1(Vector(1.0f, 0.0f, 0.0f), halfPi);
    Quaternion q2(Vector(0.0f, 1.0f, 0.0f), halfPi);
    Quaternion r = q1 * q2;

    THEN( "result matches analytic composition (0.5, 0.5, -0.5, 0.5)" ) {
      REQUIRE(r.getX() == Approx(0.5f));
      REQUIRE(r.getY() == Approx(0.5f));
      REQUIRE(r.getZ() == Approx(-0.5f));
      REQUIRE(r.getW() == Approx(0.5f));
    }
  }

  GIVEN( "operator+ adds components" ) {
    Quaternion a(1.0f, 2.0f, 3.0f, 4.0f);
    Quaternion b(10.0f, 20.0f, 30.0f, 40.0f);
    Quaternion c = a + b;
    THEN( "sum is per-component" ) {
      REQUIRE(c.getX() == Approx(11.0f));
      REQUIRE(c.getY() == Approx(22.0f));
      REQUIRE(c.getZ() == Approx(33.0f));
      REQUIRE(c.getW() == Approx(44.0f));
    }
  }

  GIVEN( "slerp with t < 0" ) {
    Quaternion qStart;
    qStart.loadMultIdentity();
    Quaternion qEnd;
    qEnd.buildFromEuler(0.0f, 1.0f, 0.0f);
    qEnd.normalize();

    Quaternion result;  // all-zero default
    result.slerp(qStart, -0.5f, qEnd);

    THEN( "result is unchanged — slerp is a no-op for out-of-range t" ) {
      REQUIRE(result.getX() == 0.0f);
      REQUIRE(result.getY() == 0.0f);
      REQUIRE(result.getZ() == 0.0f);
      REQUIRE(result.getW() == 0.0f);
    }
  }

  GIVEN( "slerp with t > 1" ) {
    Quaternion qStart;
    qStart.loadMultIdentity();
    Quaternion qEnd;
    qEnd.buildFromEuler(0.0f, 1.0f, 0.0f);
    qEnd.normalize();

    Quaternion result;  // all-zero default
    result.slerp(qStart, 2.0f, qEnd);

    THEN( "result is unchanged — slerp is a no-op for out-of-range t" ) {
      REQUIRE(result.getX() == 0.0f);
      REQUIRE(result.getY() == 0.0f);
      REQUIRE(result.getZ() == 0.0f);
      REQUIRE(result.getW() == 0.0f);
    }
  }

  GIVEN( "slerp with negated end quaternion (negative dot product branch)" ) {
    // Negating all components of a quaternion yields the same rotation.
    // When dotQ < 0, slerp flips eQ to take the short arc.  At t=1 the
    // result should still land on the positive form of the end rotation.
    const float halfPi = 1.57079632679f;
    const float s = std::sin(halfPi / 2.0f);  // sin(pi/4) = sqrt(2)/2
    const float c = std::cos(halfPi / 2.0f);  // cos(pi/4) = sqrt(2)/2

    Quaternion qStart;
    qStart.loadMultIdentity();
    Quaternion qEndNeg(0.0f, -s, 0.0f, -c);  // negated 90 deg about Y

    Quaternion result;
    result.slerp(qStart, 1.0f, qEndNeg);

    THEN( "result lands on positive 90 deg about Y — short arc was taken" ) {
      REQUIRE(result.getX() == Approx(0.0f));
      REQUIRE(result.getY() == Approx(s));
      REQUIRE(result.getZ() == Approx(0.0f));
      REQUIRE(result.getW() == Approx(c));
    }
  }

  GIVEN( "slerp between identical quaternions (linear fallback branch)" ) {
    // When start == end, dotQ == 1 and (1 - dotQ) == 0 <= 0.05 tolerance,
    // so the linear path (scale1 = 1-t, scale2 = t) is used.
    // The result must equal the input quaternion for any t.
    const float halfPi = 1.57079632679f;
    Quaternion q(Vector(0.0f, 1.0f, 0.0f), halfPi);  // 90 deg about Y

    Quaternion result;
    result.slerp(q, 0.5f, q);

    THEN( "result equals the shared start/end quaternion" ) {
      REQUIRE(result.getX() == Approx(q.getX()));
      REQUIRE(result.getY() == Approx(q.getY()));
      REQUIRE(result.getZ() == Approx(q.getZ()));
      REQUIRE(result.getW() == Approx(q.getW()));
    }
  }

  GIVEN( "slerp endpoints" ) {
    Quaternion qStart;
    qStart.loadMultIdentity();
    Quaternion qEnd;
    qEnd.buildFromEuler(0.0f, 1.0f, 0.0f);
    qEnd.normalize();

    Quaternion atZero;
    atZero.slerp(qStart, 0.0f, qEnd);
    Quaternion atOne;
    atOne.slerp(qStart, 1.0f, qEnd);

    THEN( "t=0 matches start and t=1 matches end" ) {
      REQUIRE(atZero.getX() == Approx(qStart.getX()));
      REQUIRE(atZero.getY() == Approx(qStart.getY()));
      REQUIRE(atZero.getZ() == Approx(qStart.getZ()));
      REQUIRE(atZero.getW() == Approx(qStart.getW()));
      REQUIRE(atOne.getX() == Approx(qEnd.getX()));
      REQUIRE(atOne.getY() == Approx(qEnd.getY()));
      REQUIRE(atOne.getZ() == Approx(qEnd.getZ()));
      REQUIRE(atOne.getW() == Approx(qEnd.getW()));
    }
  }
}
