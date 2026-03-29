#include "catch.hpp"
#include "Sphere.h"
#include "Vector.h"

SCENARIO( "Sphere", "[Sphere]" ) {
  GIVEN( "Default sphere" ) {
    Sphere s;
    Vector o;
    s.getOriginVector(o);

    THEN( "origin and radius are zero" ) {
      REQUIRE(o.x == Approx(0.0f));
      REQUIRE(o.y == Approx(0.0f));
      REQUIRE(o.z == Approx(0.0f));
      REQUIRE(s.getRadius() == Approx(0.0f));
    }
  }

  GIVEN( "Sphere set via setSphere" ) {
    Sphere s;
    s.setSphere(1.0f, -2.0f, 3.5f, 4.25f);
    Vector o;
    s.getOriginVector(o);

    THEN( "origin and radius match" ) {
      REQUIRE(o.x == Approx(1.0f));
      REQUIRE(o.y == Approx(-2.0f));
      REQUIRE(o.z == Approx(3.5f));
      REQUIRE(s.getRadius() == Approx(4.25f));
    }
  }

  GIVEN( "setOriginVector from Vector" ) {
    Sphere s;
    Vector v(9.0f, 8.0f, 7.0f);
    s.setOriginVector(v);
    Vector o;
    s.getOriginVector(o);

    THEN( "origin copies vector and radius remains zero" ) {
      REQUIRE(o.x == Approx(9.0f));
      REQUIRE(o.y == Approx(8.0f));
      REQUIRE(o.z == Approx(7.0f));
      REQUIRE(s.getRadius() == Approx(0.0f));
    }
  }

  // ModelGameObject sets a radius via setSphere during model load, then
  // updates position via setOriginVector each frame. Radius must survive
  // the position update or Frustum::testSphere will use a stale value.
  GIVEN( "setSphere followed by setOriginVector" ) {
    Sphere s;
    s.setSphere(0.0f, 0.0f, 0.0f, 5.0f);
    Vector v(1.0f, 2.0f, 3.0f);
    s.setOriginVector(v);
    Vector o;
    s.getOriginVector(o);

    THEN( "origin is updated and radius is preserved" ) {
      REQUIRE(o.x == Approx(1.0f));
      REQUIRE(o.y == Approx(2.0f));
      REQUIRE(o.z == Approx(3.0f));
      REQUIRE(s.getRadius() == Approx(5.0f));
    }
  }
}
