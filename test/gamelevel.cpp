#include "catch.hpp"
#include "GameLevel.h"
#include <stdio.h>

SCENARIO( "GameLevel", "[GameLevel]" ) {
  GIVEN( "A game level instance" ) {
    GameLevel l(1);

    WHEN( "instantiated" ) {
      THEN( "it is not complete" ) {
        REQUIRE(l.checkComplete() == false);
      }

      THEN( "it has no player" ) {
        REQUIRE(l.getPlayer() == NULL);
      }

      THEN( "it has no camera" ) {
        REQUIRE(l.getCamera() == NULL);
      }

      THEN( "it has no terrain" ) {
        REQUIRE(l.getTerrain() == NULL);
      }
    }

    WHEN( "loading a level without specifying a file" ) {
      bool result = l.loadLevel("");

      THEN( "it will not load" ) {
        REQUIRE(result == false);
      }
    }

    WHEN( "loading a level without specifying a buffer" ) {
      bool result = l.loadLevelFromBuffer("");

      THEN( "it will not load" ) {
        REQUIRE(result == false);
      }
    }

    WHEN( "loading a level with a valid buffer" ) {
      Camera c;
      l.setCamera(&c);
      bool result = l.loadLevelFromBuffer("test=1");

      THEN( "it will load" ) {
        REQUIRE(result == true);
      }

      THEN( "it has a player" ) {
        REQUIRE(l.getPlayer() != NULL);
      }

      THEN( "it has a camera" ) {
        REQUIRE(l.getCamera() == &c);
      }

      THEN( "it has a terrain" ) {
        REQUIRE(l.getTerrain() != NULL);
      }
    }
  }
}
