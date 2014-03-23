//////////////////////////////////////////////////////////////////
// Description : Class for handling game timing and delay       //
//////////////////////////////////////////////////////////////////
#ifndef _GAME_TIMER_H_
#define _GAME_TIMER_H_

#include "constants.h"

#ifdef _WIN32
    #include <windows.h>
#endif

#define LARGE_INTEGER long
#define MAX_FPS       60.0f

class GameTimer
{
    public:
        bool init(void);
        float getElapsedSeconds(unsigned long);
        float getFPS(void);

    private:
        long m_startTime;
        LARGE_INTEGER m_ticksPerSecond;
        float fps;
};

#endif
