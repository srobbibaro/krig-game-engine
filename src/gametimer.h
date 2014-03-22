//////////////////////////////////////////////////////////////////
// Description : Class for handling game timing and delay       //
//////////////////////////////////////////////////////////////////
//#include <windows.h>

#define LARGE_INTEGER long

#ifndef _GAME_TIMER_H_
#define _GAME_TIMER_H_

class GameTimer
{
    public:
        bool init(void);
        float getElapsedSeconds(unsigned long );
        float getFPS(void);

    private:
        long m_startTime;
        LARGE_INTEGER   m_ticksPerSecond;
        float fps;
};

#endif
