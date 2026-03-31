#include "catch.hpp"
#include "Engine.h"
#include "KeyState.h"

// gameCycle, initGL, initAL, loadGame, loadLevel, and renderText require a live
// GL/audio environment and are not exercised here.

SCENARIO( "Engine", "[Engine]" ) {
  GIVEN( "A game engine instance" ) {
    Engine e;

    WHEN( "first constructed" ) {
      THEN( "engine is running" ) {
        REQUIRE(e.getIsRunning() == true);
      }

      THEN( "key state pointers are valid (addresses of member structs)" ) {
        REQUIRE(e.getKeyState() != NULL);
        REQUIRE(e.getSpecialKeyState() != NULL);
      }

      THEN( "fps starts at zero" ) {
        REQUIRE(e.getFps() == Approx(0.0f));
      }

      THEN( "no level is loaded" ) {
        REQUIRE(e.getCurrentLevel() == NULL);
      }

      THEN( "mouse position starts at origin" ) {
        REQUIRE(e.getMouseX() == Approx(0.0f));
        REQUIRE(e.getMouseY() == Approx(0.0f));
      }
    }

    WHEN( "a normal key is pressed" ) {
      e.processNormalKeyDown('a');

      THEN( "keyState records it as pressed" ) {
        REQUIRE(e.getKeyState()->keys['a'] == KEY_STATE_PRESSED);
      }
    }

    WHEN( "a normal key is released" ) {
      e.processNormalKeyDown('a');
      e.processNormalKeyUp('a');

      THEN( "keyState records it as released" ) {
        REQUIRE(e.getKeyState()->keys['a'] == KEY_STATE_RELEASED);
      }
    }

    WHEN( "a special key is pressed" ) {
      e.processKeyDown(1);

      THEN( "specialKeyState records it as pressed" ) {
        REQUIRE(e.getSpecialKeyState()->keys[1] == KEY_STATE_PRESSED);
      }
    }

    WHEN( "a special key is released" ) {
      e.processKeyDown(1);
      e.processKeyUp(1);

      THEN( "specialKeyState records it as released" ) {
        REQUIRE(e.getSpecialKeyState()->keys[1] == KEY_STATE_RELEASED);
      }
    }

    // processCommands resets both key states to KEY_STATE_INIT each frame
    // (called at the start of gameCycle via updateGame).
    WHEN( "processCommands is called after a key is pressed" ) {
      e.processNormalKeyDown('a');
      e.processKeyDown(1);
      e.processCommands();

      THEN( "all key states are reset to KEY_STATE_INIT" ) {
        REQUIRE(e.getKeyState()->keys['a'] == KEY_STATE_INIT);
        REQUIRE(e.getSpecialKeyState()->keys[1] == KEY_STATE_INIT);
      }
    }

    WHEN( "processMouseMove is called" ) {
      e.processMouseMove(100, 200);

      THEN( "mouse position is updated" ) {
        REQUIRE(e.getMouseX() == Approx(100.0f));
        REQUIRE(e.getMouseY() == Approx(200.0f));
      }
    }

    // pause() toggles isPaused_ but there is no public getter; not testable here.
    WHEN( "shutdown is called" ) {
      e.shutdown();

      THEN( "engine is no longer running" ) {
        REQUIRE(e.getIsRunning() == false);
      }

      // shutdown() only sets isRunning_ = false — it does not close Lua, OpenAL,
      // or delete levels. Key state structs are Engine members and remain valid
      // until the Engine object is destroyed.
      THEN( "key state pointers remain valid" ) {
        REQUIRE(e.getKeyState() != NULL);
        REQUIRE(e.getSpecialKeyState() != NULL);
      }
    }
  }
}
