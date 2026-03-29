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
