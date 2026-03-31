//////////////////////////////////////////////////////////////////
// Description : Class for handling game timing and delay       //
//////////////////////////////////////////////////////////////////
#ifndef _GAME_TIMER_H_
#define _GAME_TIMER_H_

#include "constants.h"

#ifdef _WIN32
  #include <windows.h>
#endif

// On non-Windows, alias LARGE_INTEGER to long to share member declarations
// with the Windows path without requiring platform ifdefs at each use site.
#define LARGE_INTEGER long
#define MAX_FPS       30.0f

class GameTimer {
  public:
    bool init();

    /**
     * Returns the elapsed time in seconds since the last call (or since
     * init() on the first call). On non-Windows, spins until at least
     * 1/MAX_FPS seconds have elapsed to enforce the frame rate cap —
     * callers should expect this to block for up to ~33 ms per frame.
     */
    float getElapsedSeconds();

    /**
     * Returns the frame rate measured during the last getElapsedSeconds()
     * call. On non-Windows, only valid after at least one getElapsedSeconds()
     * call; returns 0.0f before that. On Windows, computes independently
     * from its own timing query.
     */
    float getFPS();

  private:
    long m_startTime;

#ifdef _WIN32
    LARGE_INTEGER m_ticksPerSecond;
#endif

    float fps;
};

#endif
