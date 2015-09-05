#include "catch.hpp"
#include "Engine.h"

SCENARIO( "Engine", "[Engine]" ) {
  GIVEN( "A game engine instance" ) {
    Engine e;
    REQUIRE(e.getIsRunning() == true);

    WHEN( "shutdown is called" ) {
      e.shutdown();

      THEN( "it no longer reports to be running" ) {
        REQUIRE(e.getIsRunning() == false);
      }
    }
  }
}
