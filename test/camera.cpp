#include "catch.hpp"
#include "Camera.h"
#include "Matrix.h"
#include "Vector.h"

SCENARIO( "Camera state without GL or Lua", "[Camera]" ) {
  GIVEN( "A default camera" ) {
    Camera cam;

    // Camera::update always rebuilds rotationMatrix unconditionally (Camera.cpp:121),
    // unlike Object which gates the rebuild on rotationChanged_.

    WHEN( "first constructed" ) {
      THEN( "frustum pointer is the embedded instance" ) {
        REQUIRE(cam.getFrustum() == &cam.frustum);
      }

      // Camera::initialize calls worldRotation.buildFromEuler(0,0,0) — identity.
      THEN( "worldRotation is identity" ) {
        REQUIRE(cam.worldRotation.getW() == Approx(1.0f));
        REQUIRE(cam.worldRotation.getX() == Approx(0.0f));
        REQUIRE(cam.worldRotation.getY() == Approx(0.0f));
        REQUIRE(cam.worldRotation.getZ() == Approx(0.0f));
      }
    }

    WHEN( "setCamera is called with kinematic values" ) {
      Vector pos(3.0f, -1.0f, 2.0f);
      Vector vel(-4.0f, 0.5f, 0.25f);
      Quaternion rot;
      rot.buildFromEuler(0.2f, -0.3f, 0.4f);
      rot.normalize();
      Vector rotVel(0.01f, 0.02f, 0.03f);
      cam.setCamera(pos, vel, rot, rotVel);

      THEN( "position, velocity, rotation, and rotationVelocity are copied" ) {
        Vector p = cam.getPosition();
        REQUIRE(p.x == Approx(3.0f));
        REQUIRE(p.y == Approx(-1.0f));
        REQUIRE(p.z == Approx(2.0f));

        Vector v = cam.getVelocity();
        REQUIRE(v.x == Approx(-4.0f));
        REQUIRE(v.y == Approx(0.5f));
        REQUIRE(v.z == Approx(0.25f));

        Quaternion r = cam.getRotation();
        REQUIRE(r.getX() == Approx(rot.getX()));
        REQUIRE(r.getY() == Approx(rot.getY()));
        REQUIRE(r.getZ() == Approx(rot.getZ()));
        REQUIRE(r.getW() == Approx(rot.getW()));

        Vector rv = cam.getRotationVelocity();
        REQUIRE(rv.x == Approx(0.01f));
        REQUIRE(rv.y == Approx(0.02f));
        REQUIRE(rv.z == Approx(0.03f));
      }
    }

    // setCamera assigns rotation_ directly without setting rotationChanged_, like
    // Object::setRotationQuaternion. Camera::update rebuilds unconditionally so this
    // does not affect rendering, but callers using isRotationChanged() as a change
    // signal should use setRotation() instead.
    WHEN( "setCamera is called after clearing rotationChanged" ) {
      Quaternion q;
      q.buildFromEuler(0.1f, 0.0f, 0.0f);
      cam.setRotationChanged(false);
      cam.setCamera(Vector(0.0f, 0.0f, 0.0f), Vector(0.0f, 0.0f, 0.0f), q,
                    Vector(0.0f, 0.0f, 0.0f));

      THEN( "rotationChanged remains false" ) {
        REQUIRE(cam.isRotationChanged() == false);
      }
    }

    WHEN( "update is called with zero elapsed time" ) {
      Quaternion q;
      q.buildFromEuler(0.1f, 0.15f, -0.05f);
      q.normalize();
      cam.setCamera(Vector(0.0f, 0.0f, 0.0f), Vector(0.0f, 0.0f, 0.0f), q,
                    Vector(0.0f, 0.0f, 0.0f));
      cam.update(0.0f);

      THEN( "rotation matrix matches the quaternion" ) {
        Matrix expected;
        q.buildRotationMatrix(expected);
        Matrix actual;
        cam.getRotationMatrix(actual);
        for (int i = 0; i < NUM_CELLS; ++i) {
          REQUIRE(actual.data[i] == Approx(expected.data[i]));
        }
      }
    }

    WHEN( "update is called with a non-zero velocity" ) {
      Quaternion q;
      q.buildFromEuler(0.0f, 0.0f, 0.0f);
      cam.setCamera(Vector(0.0f, 0.0f, 0.0f), Vector(2.0f, 0.0f, -1.0f), q,
                    Vector(0.0f, 0.0f, 0.0f));
      cam.update(0.5f);

      THEN( "position advances by velocity * elapsed time" ) {
        Vector p = cam.getPosition();
        REQUIRE(p.x == Approx(1.0f));   // 0 + 2.0 * 0.5
        REQUIRE(p.y == Approx(0.0f));   // velocity_.y == 0, unchanged
        REQUIRE(p.z == Approx(-0.5f));  // 0 + (-1.0) * 0.5
      }
    }

    WHEN( "update is called with a non-zero rotation velocity" ) {
      Quaternion q;
      q.buildFromEuler(0.0f, 0.0f, 0.0f);
      cam.setCamera(Vector(0.0f, 0.0f, 0.0f), Vector(0.0f, 0.0f, 0.0f), q,
                    Vector(0.0f, 1.0f, 0.0f));
      cam.setRotationChanged(false);
      cam.update(0.5f);

      THEN( "rotationChanged is set" ) {
        REQUIRE(cam.isRotationChanged() == true);
      }

      THEN( "rotation is no longer identity" ) {
        REQUIRE(cam.getRotation().getW() < 1.0f);
      }
    }

    // Camera::update rotates baseDirection_ by rotationMatrix then negates direction_.z
    // (Camera.cpp:124). With identity rotation and baseDirection_ (0,0,1), the result
    // is direction_ = (0, 0, -1). See docs/MATH_AND_TESTING_CONVENTIONS.md.
    WHEN( "update is called with identity rotation" ) {
      Quaternion q;
      q.buildFromEuler(0.0f, 0.0f, 0.0f);
      cam.setCamera(Vector(0.0f, 0.0f, 0.0f), Vector(0.0f, 0.0f, 0.0f), q,
                    Vector(0.0f, 0.0f, 0.0f));
      cam.update(0.0f);

      THEN( "direction z is negated relative to the rotated base direction" ) {
        Vector d = cam.getDirection();
        REQUIRE(d.x == Approx(0.0f));
        REQUIRE(d.y == Approx(0.0f));
        REQUIRE(d.z == Approx(-1.0f));
      }
    }
  }

  GIVEN( "A camera constructed with an id" ) {
    Camera cam(42);

    WHEN( "first constructed" ) {
      THEN( "id is set" ) {
        REQUIRE(cam.id_ == 42);
      }
    }
  }
}
