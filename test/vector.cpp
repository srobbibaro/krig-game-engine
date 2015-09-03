#include "catch.hpp"
#include "Vector.h"

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
}
