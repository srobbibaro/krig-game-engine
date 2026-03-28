#include "catch.hpp"
#include "gametimer.h"

SCENARIO( "GameTimer", "[GameTimer]" ) {
  GIVEN( "A game timer" ) {
    GameTimer t;

    WHEN( "init is called" ) {
      bool ok = t.init();

      THEN( "initialization succeeds on this platform" ) {
        REQUIRE(ok == true);
      }

      // On non-Windows, getFPS returns the fps member last set by
      // getElapsedSeconds. Before any getElapsedSeconds() call it is 0.0f.
      // (Windows is not currently supported; guard here for when it is.)
#ifndef _WIN32
      THEN( "getFPS returns 0.0f before any elapsed-seconds call (non-Windows)" ) {
        REQUIRE(t.getFPS() == Approx(0.0f));
      }
#endif
    }

    WHEN( "getElapsedSeconds is called after init" ) {
      t.init();
      float elapsed = t.getElapsedSeconds();

      THEN( "returns a positive elapsed time" ) {
        REQUIRE(elapsed > 0.0f);
      }

      // Mirrors the Engine frame loop: getElapsedSeconds then getFPS each frame.
      THEN( "getFPS returns a positive value after getElapsedSeconds" ) {
        REQUIRE(t.getFPS() > 0.0f);
      }
    }

    // getElapsedSeconds updates m_startTime each call so successive calls
    // each measure incremental elapsed time, not total since init.
    WHEN( "getElapsedSeconds is called twice" ) {
      t.init();
      float first  = t.getElapsedSeconds();
      float second = t.getElapsedSeconds();

      THEN( "both calls return positive elapsed time" ) {
        REQUIRE(first  > 0.0f);
        REQUIRE(second > 0.0f);
      }
    }

    WHEN( "init is called twice" ) {
      THEN( "both calls succeed" ) {
        REQUIRE(t.init() == true);
        REQUIRE(t.init() == true);
      }
    }
  }
}
