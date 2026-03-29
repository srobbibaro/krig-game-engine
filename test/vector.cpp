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
        REQUIRE(c.y == Approx(22.0f));
        REQUIRE(c.z == Approx(33.0f));
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

  // intersectBox: `this` is the ray direction; rayPosition is the ray origin.
  // collisionBox[0] = min corner, collisionBox[1] = max corner (expanded by extend).
  // Returns the entry intersection point in hitPoint (4-arg overload).
  // Returns false when no slab intersection is found for the ray direction.
  GIVEN( "intersectBox with a unit box at the origin" ) {
    Vector box[2];
    box[0].setVector(-1.0f, -1.0f, -1.0f);
    box[1].setVector( 1.0f,  1.0f,  1.0f);

    WHEN( "an X-directed ray approaches the box from the left" ) {
      Vector dir(1.0f, 0.0f, 0.0f);
      Vector origin(-5.0f, 0.0f, 0.0f);

      THEN( "the hit test returns true" ) {
        REQUIRE(dir.intersectBox(origin, box, 0.0f));
      }

      THEN( "hitPoint is placed at the entry (near) face" ) {
        Vector hitPoint;
        dir.intersectBox(origin, box, 0.0f, hitPoint);
        REQUIRE(hitPoint.x == Approx(-1.0f));
        REQUIRE(hitPoint.y == Approx( 0.0f));
        REQUIRE(hitPoint.z == Approx( 0.0f));
      }
    }

    WHEN( "an X-directed ray approaches the box from the right" ) {
      Vector dir(-1.0f, 0.0f, 0.0f);
      Vector origin(5.0f, 0.0f, 0.0f);

      THEN( "the hit test returns true" ) {
        REQUIRE(dir.intersectBox(origin, box, 0.0f));
      }

      THEN( "hitPoint is placed at the entry (near) face" ) {
        Vector hitPoint;
        dir.intersectBox(origin, box, 0.0f, hitPoint);
        REQUIRE(hitPoint.x == Approx( 1.0f));
        REQUIRE(hitPoint.y == Approx( 0.0f));
        REQUIRE(hitPoint.z == Approx( 0.0f));
      }
    }

    WHEN( "an X-directed ray passes above the box" ) {
      Vector dir(1.0f, 0.0f, 0.0f);
      Vector origin(-5.0f, 2.0f, 0.0f);

      THEN( "the hit test returns false" ) {
        REQUIRE_FALSE(dir.intersectBox(origin, box, 0.0f));
      }
    }

    WHEN( "a Y-directed ray approaches the box from below" ) {
      Vector dir(0.0f, 1.0f, 0.0f);
      Vector origin(0.0f, -5.0f, 0.0f);

      THEN( "the hit test returns true" ) {
        REQUIRE(dir.intersectBox(origin, box, 0.0f));
      }

      THEN( "hitPoint is placed at the entry (near) face" ) {
        Vector hitPoint;
        dir.intersectBox(origin, box, 0.0f, hitPoint);
        REQUIRE(hitPoint.x == Approx( 0.0f));
        REQUIRE(hitPoint.y == Approx(-1.0f));
        REQUIRE(hitPoint.z == Approx( 0.0f));
      }
    }

    WHEN( "a Y-directed ray approaches the box from above" ) {
      Vector dir(0.0f, -1.0f, 0.0f);
      Vector origin(0.0f, 5.0f, 0.0f);

      THEN( "the hit test returns true" ) {
        REQUIRE(dir.intersectBox(origin, box, 0.0f));
      }

      THEN( "hitPoint is placed at the entry (near) face" ) {
        Vector hitPoint;
        dir.intersectBox(origin, box, 0.0f, hitPoint);
        REQUIRE(hitPoint.x == Approx( 0.0f));
        REQUIRE(hitPoint.y == Approx( 1.0f));
        REQUIRE(hitPoint.z == Approx( 0.0f));
      }
    }

    WHEN( "a Y-directed ray passes beside the box in Z" ) {
      Vector dir(0.0f, 1.0f, 0.0f);
      Vector origin(0.0f, -5.0f, 2.0f);

      THEN( "the hit test returns false" ) {
        REQUIRE_FALSE(dir.intersectBox(origin, box, 0.0f));
      }
    }

    WHEN( "a Z-directed ray approaches the box from the front" ) {
      Vector dir(0.0f, 0.0f, 1.0f);
      Vector origin(0.0f, 0.0f, -5.0f);

      THEN( "the hit test returns true" ) {
        REQUIRE(dir.intersectBox(origin, box, 0.0f));
      }

      THEN( "hitPoint is placed at the entry (near) face" ) {
        Vector hitPoint;
        dir.intersectBox(origin, box, 0.0f, hitPoint);
        REQUIRE(hitPoint.x == Approx( 0.0f));
        REQUIRE(hitPoint.y == Approx( 0.0f));
        REQUIRE(hitPoint.z == Approx(-1.0f));
      }
    }

    WHEN( "a Z-directed ray approaches the box from behind" ) {
      Vector dir(0.0f, 0.0f, -1.0f);
      Vector origin(0.0f, 0.0f, 5.0f);

      THEN( "the hit test returns true" ) {
        REQUIRE(dir.intersectBox(origin, box, 0.0f));
      }

      THEN( "hitPoint is placed at the entry (near) face" ) {
        Vector hitPoint;
        dir.intersectBox(origin, box, 0.0f, hitPoint);
        REQUIRE(hitPoint.x == Approx( 0.0f));
        REQUIRE(hitPoint.y == Approx( 0.0f));
        REQUIRE(hitPoint.z == Approx( 1.0f));
      }
    }

    WHEN( "the ray direction is zero" ) {
      Vector dir(0.0f, 0.0f, 0.0f);
      Vector origin(0.0f, 0.0f, 0.0f);

      THEN( "the hit test returns false" ) {
        REQUIRE_FALSE(dir.intersectBox(origin, box, 0.0f));
      }
    }

    // A ray that starts past the box and points further away has a negative
    // parametric distance s to the near face. It should not register as a hit.
    WHEN( "the ray origin is past the box and the ray points away" ) {
      Vector dir(1.0f, 0.0f, 0.0f);
      Vector origin(5.0f, 0.0f, 0.0f);

      THEN( "the hit test returns false" ) {
        REQUIRE_FALSE(dir.intersectBox(origin, box, 0.0f));
      }
    }

    WHEN( "a ray passes just outside the Y bound of the box" ) {
      Vector dir(1.0f, 0.0f, 0.0f);
      Vector origin(-5.0f, 1.5f, 0.0f);

      THEN( "with no extension it misses" ) {
        REQUIRE_FALSE(dir.intersectBox(origin, box, 0.0f));
      }

      THEN( "with enough extension to cover the gap it hits" ) {
        REQUIRE(dir.intersectBox(origin, box, 1.0f));
      }
    }

    // A ray parallel to an axis whose origin lies outside the box on that axis
    // must miss, even though the parallel axis slab test is skipped entirely.
    // The hitPoint bounds check on the skipped axis catches this correctly.
    WHEN( "a Y-directed ray has its origin outside the box on X" ) {
      Vector dir(0.0f, 1.0f, 0.0f);
      Vector origin(50.0f, -5.0f, 0.0f);

      THEN( "the hit test returns false" ) {
        REQUIRE_FALSE(dir.intersectBox(origin, box, 0.0f));
      }
    }

    WHEN( "a Z-directed ray has its origin outside the box on X" ) {
      Vector dir(0.0f, 0.0f, 1.0f);
      Vector origin(50.0f, 0.0f, -5.0f);

      THEN( "the hit test returns false" ) {
        REQUIRE_FALSE(dir.intersectBox(origin, box, 0.0f));
      }
    }

    // A ray whose origin is already inside the box trivially intersects it.
    // hitPoint is set to the ray origin.
    WHEN( "an X-directed ray starts inside the box" ) {
      Vector dir(1.0f, 0.0f, 0.0f);
      Vector origin(0.0f, 0.0f, 0.0f);

      THEN( "the hit test returns true" ) {
        REQUIRE(dir.intersectBox(origin, box, 0.0f));
      }

      THEN( "hitPoint is set to the ray origin" ) {
        Vector hitPoint;
        dir.intersectBox(origin, box, 0.0f, hitPoint);
        REQUIRE(hitPoint.x == Approx(0.0f));
        REQUIRE(hitPoint.y == Approx(0.0f));
        REQUIRE(hitPoint.z == Approx(0.0f));
      }
    }

    WHEN( "a Y-directed ray starts inside the box" ) {
      Vector dir(0.0f, 1.0f, 0.0f);
      Vector origin(0.0f, 0.0f, 0.0f);

      THEN( "the hit test returns true" ) {
        REQUIRE(dir.intersectBox(origin, box, 0.0f));
      }
    }
  }
}
