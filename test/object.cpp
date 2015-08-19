#include "catch.hpp"
#include "object.h"

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

      THEN( "it has a negative type ID" ) {
        REQUIRE(o.getTypeId() == -1);
      }

      THEN( "it has no script" ) {
        REQUIRE(o.getScriptName() == "");
      }
    }
  }
}
