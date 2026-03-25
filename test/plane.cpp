#include "catch.hpp"
#include "Plane.h"
#include "Vector.h"

SCENARIO( "Plane operations", "[Plane]" ) {
  GIVEN( "Default plane" ) {
    Plane p;
    float a, b, c, d;
    p.getDefinition(a, b, c, d);

    THEN( "default is X-normal through origin" ) {
      REQUIRE(a == Approx(1.0f));
      REQUIRE(b == Approx(0.0f));
      REQUIRE(c == Approx(0.0f));
      REQUIRE(d == Approx(0.0f));
    }
  }

  GIVEN( "setPlane and getDefinition round-trip" ) {
    Plane p;
    p.setPlane(2.0f, -3.0f, 4.0f, -5.0f);
    float a, b, c, d;
    p.getDefinition(a, b, c, d);

    THEN( "coefficients match" ) {
      REQUIRE(a == Approx(2.0f));
      REQUIRE(b == Approx(-3.0f));
      REQUIRE(c == Approx(4.0f));
      REQUIRE(d == Approx(-5.0f));
    }
  }

  GIVEN( "Normalized plane through origin" ) {
    Plane p;
    p.setPlane(3.0f, 4.0f, 0.0f, 0.0f);
    p.normalize();
    float a, b, c, d;
    p.getDefinition(a, b, c, d);
    const float invLen = 0.2f;

    THEN( "normal is unit length" ) {
      REQUIRE(a == Approx(3.0f * invLen));
      REQUIRE(b == Approx(4.0f * invLen));
      REQUIRE(c == Approx(0.0f));
      REQUIRE(d == Approx(0.0f));
    }

    THEN( "distanceToPoint is signed distance" ) {
      REQUIRE(p.distanceToPoint(5.0f, 0.0f, 0.0f) == Approx(3.0f * invLen * 5.0f));
      REQUIRE(p.distanceToPoint(0.0f, 0.0f, 0.0f) == Approx(0.0f));
    }

    THEN( "classifyPoint distinguishes sides" ) {
      REQUIRE(p.classifyPoint(10.0f, 0.0f, 0.0f) == 1);
      REQUIRE(p.classifyPoint(-10.0f, 0.0f, 0.0f) == -1);
      REQUIRE(p.classifyPoint(0.0f, 0.0f, 0.0f) == 0);
    }

    THEN( "getNormalVector matches abc" ) {
      Vector n;
      p.getNormalVector(n);
      REQUIRE(n.x == Approx(a));
      REQUIRE(n.y == Approx(b));
      REQUIRE(n.z == Approx(c));
    }
  }

  GIVEN( "Normalizing a degenerate normal" ) {
    Plane p;
    p.setPlane(0.0f, 0.0f, 0.0f, 5.0f);
    p.normalize();
    float a, b, c, d;
    p.getDefinition(a, b, c, d);

    THEN( "coefficients are unchanged when length is zero" ) {
      REQUIRE(a == Approx(0.0f));
      REQUIRE(b == Approx(0.0f));
      REQUIRE(c == Approx(0.0f));
      REQUIRE(d == Approx(5.0f));
    }
  }
}
