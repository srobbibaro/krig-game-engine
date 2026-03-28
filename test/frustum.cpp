// Uses a simplified clip matrix, not a full gluPerspective-style projection.
// With identity + data[10]=-1 (column-major), extractFromProjectionMatrix
// produces six axis-aligned planes that bound the unit cube [-1,1]^3:
//   left  (1,0,0,1), right (-1,0,0,1), bottom (0,1,0,1),
//   top   (0,-1,0,1), far  (0,0,1,1),  near   (0,0,-1,1)
// All plane distances from the origin equal 1, so testSphere results are
// analytically predictable and are asserted as specific codes below.
#include "catch.hpp"
#include "Frustum.h"
#include "Matrix.h"
#include "Sphere.h"
#include <cmath>

SCENARIO( "Frustum", "[Frustum]" ) {
  GIVEN( "Frustum from a non-degenerate projection-like matrix" ) {
    Matrix proj;
    proj.loadIdentity();
    // Simple symmetric adjustment so clip planes are well-defined (like glOrtho depth).
    proj.data[10] = -1.0f;

    Frustum f;
    f.extractFromProjectionMatrix(proj);

    WHEN( "reading plane definitions" ) {
      THEN( "all six planes are finite and normalized" ) {
        for (int i = 0; i < NUM_PLANES; ++i) {
          float a, b, c, d;
          f.getPlaneDefinition(i, a, b, c, d);
          REQUIRE(std::isfinite(a));
          REQUIRE(std::isfinite(b));
          REQUIRE(std::isfinite(c));
          REQUIRE(std::isfinite(d));
          const float len = std::sqrt(a * a + b * b + c * c);
          REQUIRE(len == Approx(1.0f));
        }
      }
    }

    // getPlaneDefinition silently does nothing for out-of-range indices,
    // leaving output parameters unchanged (both above and below valid range).
    WHEN( "reading an out-of-range plane index (too high)" ) {
      float a = 99.0f, b = 99.0f, c = 99.0f, d = 99.0f;
      f.getPlaneDefinition(NUM_PLANES, a, b, c, d);
      THEN( "output parameters are left unchanged" ) {
        REQUIRE(a == Approx(99.0f));
        REQUIRE(b == Approx(99.0f));
        REQUIRE(c == Approx(99.0f));
        REQUIRE(d == Approx(99.0f));
      }
    }

    WHEN( "reading an out-of-range plane index (negative)" ) {
      float a = 99.0f, b = 99.0f, c = 99.0f, d = 99.0f;
      f.getPlaneDefinition(-1, a, b, c, d);
      THEN( "output parameters are left unchanged" ) {
        REQUIRE(a == Approx(99.0f));
        REQUIRE(b == Approx(99.0f));
        REQUIRE(c == Approx(99.0f));
        REQUIRE(d == Approx(99.0f));
      }
    }

    WHEN( "testBoundingBox stub" ) {
      THEN( "returns zero" ) {
        REQUIRE(f.testBoundingBox() == 0);
      }
    }

    // testSphere return codes: 1 = inside all planes, 0 = intersecting one or
    // more planes, -1 = outside at least one plane. Both engine callers
    // (ModelGameObject::animate and QuadTree::buildDisplayList) treat r != -1
    // as "visible" — only -1 triggers culling.
    //
    // Note: testSphere is conservative. It returns 0 on the first intersecting
    // plane without checking remaining planes, so a sphere that intersects one
    // plane but is outside another may return 0 (render) instead of -1 (cull).

    // With this matrix every plane is distance 1 from the origin.
    // A sphere at the origin with radius 0.5: all distances = 1 > 0.5 → inside.
    WHEN( "sphere at origin with radius 0.5" ) {
      Sphere s;
      s.setSphere(0.0f, 0.0f, 0.0f, 0.5f);
      THEN( "returns 1 (fully inside, r != -1 so rendered)" ) {
        REQUIRE(f.testSphere(s) == 1);
      }
    }

    // Sphere far outside on X: right-plane distance = -1*5+1 = -4 < -0.1 → outside.
    WHEN( "sphere at (5,0,0) with radius 0.1" ) {
      Sphere s;
      s.setSphere(5.0f, 0.0f, 0.0f, 0.1f);
      THEN( "returns -1 (outside, culled)" ) {
        REQUIRE(f.testSphere(s) == -1);
      }
    }

    // Sphere straddling the right face: right-plane distance = -1*0.95+1 = 0.05 < radius 0.1.
    WHEN( "sphere at (0.95,0,0) with radius 0.1" ) {
      Sphere s;
      s.setSphere(0.95f, 0.0f, 0.0f, 0.1f);
      THEN( "returns 0 (intersecting, r != -1 so rendered)" ) {
        REQUIRE(f.testSphere(s) == 0);
      }
    }

    // A zero-radius sphere is a point. |distance| < 0 is never true, so a
    // point can never return 0 (intersecting) — it is either inside or outside.
    WHEN( "zero-radius sphere (point) inside the frustum" ) {
      Sphere s;
      s.setSphere(0.0f, 0.0f, 0.0f, 0.0f);
      THEN( "returns 1 (inside, not intersecting)" ) {
        REQUIRE(f.testSphere(s) == 1);
      }
    }

    WHEN( "zero-radius sphere (point) outside the frustum" ) {
      Sphere s;
      s.setSphere(5.0f, 0.0f, 0.0f, 0.0f);
      THEN( "returns -1 (outside, not intersecting)" ) {
        REQUIRE(f.testSphere(s) == -1);
      }
    }
  }
}
