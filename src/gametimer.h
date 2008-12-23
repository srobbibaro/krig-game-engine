//////////////////////////////////////////////////////////////////
// Authors     : Steve Robbibaro     sar2                       //
//             : Andrew Yochum       amyst14                    //
//                                                              //
// Filename    : gametimer.h                                    //
//                                                              //
// Description : Class for handling game timing and delay       //
//                                                              //
// Date        : 12-1-03                                        //
//////////////////////////////////////////////////////////////////
#include <windows.h>

#ifndef _GAME_TIMER_H_
#define _GAME_TIMER_H_

class GameTimer
{
    public:
        bool init(void);
        float getElapsedSeconds(unsigned long );
        float getFPS(void);
    
    private:
        LARGE_INTEGER   m_startTime;
        LARGE_INTEGER   m_ticksPerSecond;
};

#endif
