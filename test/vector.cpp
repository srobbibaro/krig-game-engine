#include "catch.hpp"
#include "Vector.h"
#include "Matrix.h"

SCENARIO( "Manipulating a vector", "[Vector]" ) {
  GIVEN( "A set vector" ) {
    Vector v;
    v.setVector(1.0f, 2.0f, 3.0f);

    WHEN( "the normalize method is called" ) {
      v.normalize();

      THEN( "the vector is normalized" ) {
        REQUIRE(v.x == Approx(0.26726f));
        REQUIRE(v.y == Approx(0.53452f));
        REQUIRE(v.z == Approx(0.80178f));
      }
    }
  }

  GIVEN( "Default and parameterized constructors" ) {
    Vector a;
    Vector b(3.0f, 4.0f, 0.0f);

    THEN( "default is zero" ) {
      REQUIRE(a.x == 0.0f);
      REQUIRE(a.y == 0.0f);
      REQUIRE(a.z == 0.0f);
    }

    THEN( "constructor sets components" ) {
      REQUIRE(b.x == 3.0f);
      REQUIRE(b.y == 4.0f);
      REQUIRE(b.z == 0.0f);
    }
  }

  GIVEN( "Vectors for dot and cross product" ) {
    Vector u;
    u.setVector(1.0f, 2.0f, 3.0f);
    Vector v;
    v.setVector(4.0f, 5.0f, 6.0f);

    THEN( "dotProduct matches hand calculation" ) {
      REQUIRE(u.dotProduct(v) == Approx(32.0f));
    }

    WHEN( "crossProduct stores u x v" ) {
      Vector w;
      w.crossProduct(u, v);
      THEN( "result is orthogonal to both" ) {
        REQUIRE(w.x == Approx(-3.0f));
        REQUIRE(w.y == Approx(6.0f));
        REQUIRE(w.z == Approx(-3.0f));
      }
    }
  }

  GIVEN( "Vectors for distance, length, and scale" ) {
    Vector p(0.0f, 0.0f, 0.0f);
    Vector q(2.0f, 0.0f, 0.0f);

    THEN( "getDistance is Euclidean" ) {
      REQUIRE(p.getDistance(q) == Approx(2.0f));
    }

    THEN( "getLength uses current components" ) {
      REQUIRE(q.getLength() == Approx(2.0f));
    }

    WHEN( "scale multiplies components" ) {
      q.scale(3.0f);
      THEN( "components scale uniformly" ) {
        REQUIRE(q.x == Approx(6.0f));
        REQUIRE(q.y == Approx(0.0f));
        REQUIRE(q.z == Approx(0.0f));
      }
    }
  }

  GIVEN( "getScaler and getSum" ) {
    Vector a(1.0f, 2.0f, 3.0f);
    Vector b(4.0f, 5.0f, 6.0f);

    THEN( "getScaler is component-wise multiply then sum" ) {
      REQUIRE(a.getScaler(b) == Approx(32.0f));
    }

    THEN( "getSum adds components" ) {
      REQUIRE(a.getSum() == Approx(6.0f));
    }
  }

  GIVEN( "Operator assignments and arithmetic" ) {
    Vector a(1.0f, 2.0f, 3.0f);
    Vector b(10.0f, 20.0f, 30.0f);

    WHEN( "operator=" ) {
      a = b;
      THEN( "values copy" ) {
        REQUIRE(a.x == 10.0f);
        REQUIRE(a.y == 20.0f);
        REQUIRE(a.z == 30.0f);
      }
    }

    WHEN( "operator+ and operator*" ) {
      Vector c = a + b;
      Vector d = a * 2.0f;
      THEN( "addition and scalar multiply" ) {
        REQUIRE(c.x == Approx(11.0f));
        REQUIRE(d.x == Approx(2.0f));
        REQUIRE(d.y == Approx(4.0f));
        REQUIRE(d.z == Approx(6.0f));
      }
    }

    WHEN( "operator+= and operator-=" ) {
      a += b;
      a -= Vector(1.0f, 2.0f, 3.0f);
      THEN( "in-place add and subtract" ) {
        REQUIRE(a.x == Approx(10.0f));
        REQUIRE(a.y == Approx(20.0f));
        REQUIRE(a.z == Approx(30.0f));
      }
    }
  }

  GIVEN( "Normalizing a zero vector" ) {
    Vector z(0.0f, 0.0f, 0.0f);
    z.normalize();
    THEN( "components stay zero" ) {
      REQUIRE(z.x == 0.0f);
      REQUIRE(z.y == 0.0f);
      REQUIRE(z.z == 0.0f);
    }
  }

  GIVEN( "calcNorm from three triangle corners" ) {
    Vector p1(0.0f, 0.0f, 0.0f);
    Vector p2(1.0f, 0.0f, 0.0f);
    Vector p3(0.0f, 1.0f, 0.0f);
    Vector n;
    n.calcNorm(p1, p2, p3);
    THEN( "normal points along +Z for XY triangle" ) {
      REQUIRE(n.x == Approx(0.0f));
      REQUIRE(n.y == Approx(0.0f));
      REQUIRE(n.z == Approx(1.0f));
    }
  }

  GIVEN( "rotateVector with rotation around Z" ) {
    Matrix m;
    const float halfPi = 1.57079632679f;
    m.setRotation(halfPi, 0.0f, 0.0f, 1.0f);
    Vector v(1.0f, 0.0f, 0.0f);
    Vector out;
    out.rotateVector(m, v);
    THEN( "X axis rotates toward -Y for this setRotation convention" ) {
      REQUIRE(out.x == Approx(0.0f).epsilon(1e-4f));
      REQUIRE(out.y == Approx(-1.0f).epsilon(1e-4f));
      REQUIRE(out.z == Approx(0.0f).epsilon(1e-4f));
    }
  }

  GIVEN( "transformVector with translation" ) {
    Matrix m;
    m.setTranslation(1.0f, 2.0f, 3.0f);
    Vector v(10.0f, 20.0f, 30.0f);
    Vector out;
    out.transformVector(m, v);
    THEN( "affine part accumulates translation" ) {
      REQUIRE(out.x == Approx(11.0f));
      REQUIRE(out.y == Approx(22.0f));
      REQUIRE(out.z == Approx(33.0f));
    }
  }

  GIVEN( "average of two vectors" ) {
    Vector a(1.0f, 0.0f, 0.0f);
    Vector b(0.0f, 1.0f, 0.0f);
    Vector mid;
    mid.average(a, b);
    THEN( "result is normalized sum" ) {
      const float invSqrt2 = 0.70710678118f;
      REQUIRE(mid.x == Approx(invSqrt2));
      REQUIRE(mid.y == Approx(invSqrt2));
      REQUIRE(mid.z == Approx(0.0f));
    }
  }
}
