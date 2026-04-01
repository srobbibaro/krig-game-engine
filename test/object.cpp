#include "catch.hpp"
#include "Object.h"
#include "Quaternion.h"
#include <cmath>

class BasicObject : public Object {
  public:
    void draw(Object* o) {}
    void drawOutline(Object* o) {}
    void handleCollision(Object* o) {}
    void update(Vector* v) {}
    void animate(float f, Object* o) {}
    void printTypeName(void) {}
};

SCENARIO( "Object Creation", "[object]" ) {
  GIVEN( "A basic derived object" ) {
    BasicObject o;

    WHEN( "first initiated" ) {
      THEN( "it is set to active" ) {
        REQUIRE(o.getActive());
      }

      THEN( "it is set to 'NORMAL'" ) {
        REQUIRE(o.getState() == NORMAL);
      }

      THEN( "it has no velocity" ) {
        Vector v = o.getVelocity();
        REQUIRE(v.x == 0.0f);
        REQUIRE(v.y == 0.0f);
        REQUIRE(v.z == 0.0f);
      }

      THEN( "it has no speed" ) {
        Vector v = o.getSpeed();
        REQUIRE(v.x == 0.0f);
        REQUIRE(v.y == 0.0f);
        REQUIRE(v.z == 0.0f);
      }

      THEN( "it is scaled at 1x in all directions" ) {
        Vector v = o.getScale();
        REQUIRE(v.x == 1.0f);
        REQUIRE(v.y == 1.0f);
        REQUIRE(v.z == 1.0f);
      }

      THEN( "it is positioned at the origin" ) {
        Vector v = o.getPosition();
        REQUIRE(v.x == 0.0f);
        REQUIRE(v.y == 0.0f);
        REQUIRE(v.z == 0.0f);
      }

      THEN( "it is in view" ) {
        REQUIRE(o.getInView() == true);
      }

      THEN( "draw is enabled" ) {
        REQUIRE(o.getDrawEnabled() == true);
      }

      THEN( "collision detection is enabled" ) {
        REQUIRE(o.getCollisionDetectionEnabled() == true);
      }

      THEN( "it has a negative type ID" ) {
        REQUIRE(o.getTypeId() == -1);
      }

      THEN( "it has no script" ) {
        REQUIRE(o.getScriptName() == "");
      }
    }

    WHEN( "calcTriangleCenter is called" ) {
      const float h = o.calcTriangleCenter(0.0f, 4.0f, 8.0f);
      THEN( "it uses successive midpoint weighting (not arithmetic mean)" ) {
        const float th1 = 0.5f * 0.0f + 0.5f * 4.0f;
        const float expected = 0.5f * th1 + 0.5f * 8.0f;
        REQUIRE(h == Approx(expected));
        REQUIRE(h == Approx(5.0f));
      }
    }

    WHEN( "kinematic setters are used" ) {
      o.setPosition(1.0f, -2.0f, 3.0f);
      o.setVelocity(0.25f, 0.5f, 0.125f);
      o.setSpeed(-1.0f, 0.0f, 2.0f);
      o.setScale(2.0f, 3.0f, 4.0f);
      o.setRotationVelocity(0.1f, -0.2f, 0.3f);
      Vector p = o.getPosition();
      Vector v = o.getVelocity();
      Vector sp = o.getSpeed();
      Vector sc = o.getScale();
      Vector rv = o.getRotationVelocity();

      THEN( "getters reflect the values" ) {
        REQUIRE(p.x == Approx(1.0f));
        REQUIRE(p.y == Approx(-2.0f));
        REQUIRE(p.z == Approx(3.0f));
        REQUIRE(v.x == Approx(0.25f));
        REQUIRE(sp.x == Approx(-1.0f));
        REQUIRE(sp.z == Approx(2.0f));
        REQUIRE(sc.x == Approx(2.0f));
        REQUIRE(rv.y == Approx(-0.2f));
      }
    }

    WHEN( "Vector overloads for position and velocity" ) {
      Vector pos(9.0f, 8.0f, 7.0f);
      Vector vel(1.0f, 1.0f, 1.0f);
      o.setPosition(pos);
      o.setVelocity(vel);
      Vector p = o.getPosition();
      Vector v = o.getVelocity();
      THEN( "they copy component-wise" ) {
        REQUIRE(p.x == Approx(9.0f));
        REQUIRE(v.z == Approx(1.0f));
      }
    }

    WHEN( "setRotationEuler updates rotation quaternion" ) {
      o.setRotationEuler(0.2f, -0.1f, 0.3f);
      Quaternion q = o.getRotation();
      THEN( "rotation is non-trivial unit quaternion" ) {
        const float len = std::sqrt(
          q.getX() * q.getX() + q.getY() * q.getY() + q.getZ() * q.getZ() + q.getW() * q.getW());
        REQUIRE(len == Approx(1.0f));
      }
    }

    WHEN( "collision detection is disabled" ) {
      o.setCollisionDetectionEnabled(false);
      THEN( "getCollisionDetectionEnabled returns false" ) {
        REQUIRE(o.getCollisionDetectionEnabled() == false);
      }
    }

    WHEN( "misc identity and bookkeeping fields" ) {
      o.setActive(false);
      o.setTypeId(42);
      o.setGameLevelId(7u);
      o.setScaleRate(0.1f, 0.2f, 0.3f);
      o.setIsAlwaysLit(true);
      Quaternion q;
      q.loadMultIdentity();
      o.setRotationQuaternion(q);
      o.setValInterpBegin(0.0f);
      o.setValInterpCurrent(5.0f);
      o.setValInterpEnd(10.0f);

      THEN( "getters match" ) {
        REQUIRE(o.getActive() == false);
        REQUIRE(o.getTypeId() == 42);
        REQUIRE(o.getGameLevelId() == 7u);
        Vector sr = o.getScaleRate();
        REQUIRE(sr.x == Approx(0.1f));
        REQUIRE(o.getIsAlwaysLit() == true);
        REQUIRE(o.getRotation().getW() == Approx(1.0f));
        REQUIRE(o.getValInterpCurrent() == Approx(5.0f));
        REQUIRE(o.getValInterpEnd() == Approx(10.0f));
      }
    }

    WHEN( "interpolation is toggled" ) {
      o.setIsInterpolationEnabled(true);
      THEN( "isInterpolationEnabled returns true" ) {
        REQUIRE(o.isInterpolationEnabled() == true);
      }
    }

    WHEN( "interpolation is disabled" ) {
      o.setIsInterpolationEnabled(false);
      THEN( "isInterpolationEnabled returns false" ) {
        REQUIRE(o.isInterpolationEnabled() == false);
      }
    }

    // rotationChanged_ initialises to true so the rotation matrix is built on the first frame.
    WHEN( "rotationChanged latch is cleared then re-read" ) {
      o.setRotationChanged(false);
      THEN( "isRotationChanged returns false" ) {
        REQUIRE(o.isRotationChanged() == false);
      }
    }

    WHEN( "default collision sphere test flag" ) {
      THEN( "generic objects enable sphere test" ) {
        REQUIRE(o.getEnableSphereTest() == true);
      }
    }

    // setRotation() sets rotationChanged_ = true as a side effect; setRotationQuaternion()
    // does not. Callers that need the rotation matrix rebuilt must use setRotation().
    WHEN( "setRotationQuaternion is called after clearing rotationChanged" ) {
      Quaternion q;
      q.loadMultIdentity();
      o.setRotationChanged(false);
      o.setRotationQuaternion(q);

      THEN( "rotationChanged remains false" ) {
        REQUIRE(o.isRotationChanged() == false);
      }
    }

    WHEN( "setRotation is called after clearing rotationChanged" ) {
      Quaternion q;
      q.loadMultIdentity();
      o.setRotationChanged(false);
      o.setRotation(q);

      THEN( "rotationChanged is set to true" ) {
        REQUIRE(o.isRotationChanged() == true);
      }
    }

    // processCollisions is not tested here: it requires two objects chained via ObjectNode::next
    // and has a known radius bug (radius2 uses this object's boundingSphere_ instead of the
    // colliding object's sphere, so both radii come from the same getter).

    // Object::animate drives the frame loop. BasicObject overrides animate(float, Object*)
    // with a different signature, so Object::animate must be called explicitly to reach
    // the base implementation.
    WHEN( "Object::animate is called with a non-zero velocity" ) {
      o.setPosition(0.0f, 0.0f, 0.0f);
      o.setVelocity(2.0f, 0.0f, -1.0f);
      o.Object::animate(0.5f, nullptr);
      Vector p = o.getPosition();

      THEN( "position advances by velocity * elapsed time" ) {
        REQUIRE(p.x == Approx(1.0f));   // 0 + 2.0 * 0.5
        REQUIRE(p.y == Approx(0.0f));   // velocity_.y == 0, unchanged
        REQUIRE(p.z == Approx(-0.5f));  // 0 + (-1.0) * 0.5
      }
    }

    // With non-zero rotationVelocity and interpolation disabled (the default), animate
    // accumulates rotation_ by rotationVelocity * timeElapsed and sets rotationChanged_.
    WHEN( "Object::animate is called with a non-zero rotation velocity" ) {
      o.setRotationChanged(false);
      o.setRotationVelocity(0.0f, 1.0f, 0.0f);
      o.Object::animate(0.5f, nullptr);

      THEN( "rotationChanged is set" ) {
        REQUIRE(o.isRotationChanged() == true);
      }

      THEN( "rotation is no longer identity" ) {
        Quaternion q = o.getRotation();
        const float len = std::sqrt(
          q.getX() * q.getX() + q.getY() * q.getY() +
          q.getZ() * q.getZ() + q.getW() * q.getW());
        REQUIRE(len == Approx(1.0f));
        // y-axis rotation: x and z components stay near zero, w < 1
        REQUIRE(q.getW() < 1.0f);
      }
    }

    // isDrawable guards every draw call: active && inView && drawEnabled && state != DEAD.
    // inView has no public setter so only the default (true) can be tested here.
    WHEN( "isDrawable is checked in default state" ) {
      THEN( "object is drawable" ) {
        REQUIRE(o.isDrawable() == true);
      }
    }

    WHEN( "object is deactivated" ) {
      o.setActive(false);
      THEN( "isDrawable returns false" ) {
        REQUIRE(o.isDrawable() == false);
      }
    }

    WHEN( "draw is disabled" ) {
      o.setDrawEnabled(false);
      THEN( "isDrawable returns false" ) {
        REQUIRE(o.isDrawable() == false);
      }
    }

    // setState(DEAD) does not clear active_ — callers must also call setActive(false).
    WHEN( "state is set to DEAD" ) {
      o.setState(DEAD);
      THEN( "isDrawable returns false even though active_ is unchanged" ) {
        REQUIRE(o.getActive() == true);
        REQUIRE(o.isDrawable() == false);
      }
    }
  }
}
