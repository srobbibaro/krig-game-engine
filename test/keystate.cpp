#include "catch.hpp"
#include "KeyState.h"

SCENARIO( "KeyState", "[keystate]" ) {
  GIVEN( "A key state" ) {
    KeyState ks;

    WHEN( "checking for key presses below bounds" ) {
      THEN( "it cannot confirm" ) {
        REQUIRE_FALSE(ks.testKeyPressed(-1));
      }
    }

    WHEN( "checking for key presses above bounds" ) {
      THEN( "it cannot confirm" ) {
        REQUIRE_FALSE(ks.testKeyPressed(MAX_KEYS + 1));
      }
    }

    WHEN( "checking for key presses that haven't been registered" ) {
      THEN( "it cannot confirm" ) {
        REQUIRE_FALSE(ks.testKeyPressed(0));
      }
    }

    WHEN( "checking for key presses that have been registered" ) {
      ks.keys[0] = KEY_STATE_PRESSED;
      THEN( "it only confirms the key press" ) {
        REQUIRE(ks.testKeyPressed(0));
        REQUIRE_FALSE(ks.testKeyReleased(0));
      }
    }

    WHEN( "checking for released keys below bounds" ) {
      THEN( "it cannot confirm" ) {
        REQUIRE_FALSE(ks.testKeyReleased(-1));
      }
    }

    WHEN( "checking for released keys above bounds" ) {
      THEN( "it cannot confirm" ) {
        REQUIRE_FALSE(ks.testKeyReleased(MAX_KEYS + 1));
      }
    }

    WHEN( "checking for released keys that haven't been registered" ) {
      THEN( "it cannot confirm" ) {
        REQUIRE_FALSE(ks.testKeyReleased(0));
      }
    }

    WHEN( "checking for released keys that have been registered" ) {
      ks.keys[10] = KEY_STATE_RELEASED;
      THEN( "it only confirms the key release" ) {
        REQUIRE(ks.testKeyReleased(10));
        REQUIRE_FALSE(ks.testKeyPressed(10));
      }
    }

    WHEN( "initializing the key states" ) {
      ks.keys[10] = KEY_STATE_RELEASED;
      ks.keys[20] = KEY_STATE_PRESSED;
      ks.initKeyState();

      THEN( "it restores the initial state for all keys" ) {
        REQUIRE(ks.keys[10] == KEY_STATE_INIT);
        REQUIRE(ks.keys[20] == KEY_STATE_INIT);
        REQUIRE(ks.keys[90] == KEY_STATE_INIT);
      }
    }
  }
}
