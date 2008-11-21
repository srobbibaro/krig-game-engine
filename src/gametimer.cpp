//////////////////////////////////////////////////////////////////
// Authors     : Steve Robbibaro     sar2                       //
//             : Andrew Yochum       amyst14                    //
//                                                              //
// Filename    : gametimer.cpp                                  //
//                                                              //
// Description : Handles timing and delay during gameplay       //
//                                                              //
// Date        : 12-1-03                                        //
//////////////////////////////////////////////////////////////////
#include <windows.h>
#include "gametimer.h"

//------------------------------------------------------------------------------
bool GameTimer::init(void)
{
  if (!QueryPerformanceFrequency(&m_ticksPerSecond))
    return false;  // system doesn't support high-res timer
  
  else
  {
    QueryPerformanceCounter(&m_startTime);
    return true;
  }
}

//------------------------------------------------------------------------------
float GameTimer::getElapsedSeconds( unsigned long )
{
  static LARGE_INTEGER s_lastTime = m_startTime;
  LARGE_INTEGER currentTime;

  QueryPerformanceCounter(&currentTime);

  float seconds =  ((float)currentTime.QuadPart - (float)s_lastTime.QuadPart) / (float)m_ticksPerSecond.QuadPart;

  // reset the timer
  s_lastTime = currentTime;

  return seconds;
} 

//------------------------------------------------------------------------------
float GameTimer::getFPS(void)
{
  unsigned long elapsedFrames = 1;
  static LARGE_INTEGER s_lastTime = m_startTime;
  LARGE_INTEGER currentTime;

  QueryPerformanceCounter(&currentTime);

  float fps = (float)elapsedFrames * (float)m_ticksPerSecond.QuadPart / ((float)currentTime.QuadPart - (float)s_lastTime.QuadPart);

  // reset the timer
  s_lastTime = currentTime;

  return fps;
} // end GetFPS

