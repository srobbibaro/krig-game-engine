#include "catch.hpp"
#include "GameLevel.h"
#include "Object.h"
#include "Vector.h"

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

      THEN( "elapsed time is zero" ) {
        REQUIRE(l.getElapsedTime() == Approx(0.0f));
      }

      THEN( "id is zero" ) {
        REQUIRE(l.getId() == 0);
      }

      THEN( "music path is empty" ) {
        REQUIRE(l.getMusicPath() == "");
      }

      THEN( "objects list is accessible" ) {
        REQUIRE(l.getObjects() != NULL);
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

    // setCamera registers the camera at idToObjectMap_[0] regardless of whether
    // a level has been loaded.
    WHEN( "setCamera is called before loading" ) {
      Camera c;
      l.setCamera(&c);

      THEN( "getCamera returns the camera" ) {
        REQUIRE(l.getCamera() == &c);
      }

      THEN( "camera is accessible at object id 0" ) {
        REQUIRE(l.getObjectFromId(0) == (Object*)&c);
      }
    }

    // setCamera must be called before loadLevelFromBuffer: finishLevelLoad calls
    // exit(1) if camera_ == NULL (GameLevel.cpp:213).
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

      THEN( "camera is registered at object id 0" ) {
        REQUIRE(l.getObjectFromId(0) == (Object*)&c);
      }

      // finishLevelLoad registers player at id 1 and terrain at id 2 (GameLevel.cpp:197-198).
      THEN( "player is registered at object id 1" ) {
        REQUIRE(l.getObjectFromId(1) == (Object*)l.getPlayer());
      }

      THEN( "terrain is registered at object id 2" ) {
        REQUIRE(l.getObjectFromId(2) == (Object*)l.getTerrain());
      }
    }

    // prepLevelLoad returns false if luaState_ != NULL, so a second load on
    // the same instance silently fails and leaves the level state unchanged.
    WHEN( "loadLevelFromBuffer is called a second time on the same instance" ) {
      Camera c;
      l.setCamera(&c);
      l.loadLevelFromBuffer("test=1");
      bool result = l.loadLevelFromBuffer("test=1");

      THEN( "it will not load" ) {
        REQUIRE(result == false);
      }

      THEN( "state from first load is preserved" ) {
        REQUIRE(l.getPlayer() != NULL);
      }
    }

    WHEN( "setComplete is called with true" ) {
      l.setComplete(true);

      THEN( "checkComplete returns true" ) {
        REQUIRE(l.checkComplete() == true);
      }
    }

    WHEN( "setElapsedTime is called" ) {
      l.setElapsedTime(2.25f);

      THEN( "getElapsedTime returns the value" ) {
        REQUIRE(l.getElapsedTime() == Approx(2.25f));
      }
    }

    WHEN( "setId is called" ) {
      l.setId(99);

      THEN( "getId returns the value" ) {
        REQUIRE(l.getId() == 99);
      }
    }

    WHEN( "setLightDirection is called" ) {
      l.setLightDirection(3.0f, 4.0f, 0.0f);
      Vector* dir = l.getLightDirection();

      THEN( "light direction is normalised" ) {
        REQUIRE(dir->x == Approx(0.6f));  // 3 / 5
        REQUIRE(dir->y == Approx(0.8f));  // 4 / 5
        REQUIRE(dir->z == Approx(0.0f));
      }
    }

    WHEN( "setMusicPath is called" ) {
      l.setMusicPath("test/music.ogg");

      THEN( "getMusicPath returns the value" ) {
        REQUIRE(l.getMusicPath() == "test/music.ogg");
      }
    }

    WHEN( "getObjectFromId is called with an unregistered id" ) {
      THEN( "returns NULL" ) {
        REQUIRE(l.getObjectFromId(1) == NULL);
      }
    }

    // getObjectFromId returns NULL for ids outside [0, MAX_LEVEL_OBJECTS).
    WHEN( "getObjectFromId is called with an out-of-bounds id" ) {
      THEN( "negative id returns NULL" ) {
        REQUIRE(l.getObjectFromId(-1) == NULL);
      }

      THEN( "id >= MAX_LEVEL_OBJECTS returns NULL" ) {
        REQUIRE(l.getObjectFromId(MAX_LEVEL_OBJECTS) == NULL);
      }
    }
  }
}
