//////////////////////////////////////////////////////////////////
// Description : Handles timing and delay during gameplay       //
//////////////////////////////////////////////////////////////////
#ifdef _WIN32
#include <windows.h>
#endif

#include <sys/time.h>
#include "gametimer.h"

//------------------------------------------------------------------------------
bool GameTimer::init(void) {
#ifdef _WIN32
  if (!QueryPerformanceFrequency(&m_ticksPerSecond)) {
    // system doesn't support high-res timer
    return false;
  }

  QueryPerformanceCounter(&m_startTime);
#else
  timeval tv;
  gettimeofday(&tv, 0);
  //m_startTime = ((double)tv.tv_usec / 1000.0 + (double)tv.tv_sec * 1000.0);
  m_startTime = tv.tv_usec / 1000 + tv.tv_sec * 1000;

  fps = 0.0f;
#endif

  return true;
}

//------------------------------------------------------------------------------
float GameTimer::getElapsedSeconds(unsigned long) {
#ifdef _WIN32
  static LARGE_INTEGER s_lastTime = m_startTime;
  LARGE_INTEGER currentTime;

  QueryPerformanceCounter(&currentTime);

  float seconds = ((float)currentTime.QuadPart - (float)s_lastTime.QuadPart) /
    (float)m_ticksPerSecond.QuadPart;

  // reset the timer
  s_lastTime = currentTime;
#else
  float seconds = 0.0f;
  int tries = 0;

  fps = MAX_FPS;

  while (fps >= MAX_FPS || ++tries > 100) {
    timeval tv;
    gettimeofday(&tv, 0);

    //double currentTime = ((double)tv.tv_usec / 1000.0 + (double)tv.tv_sec * 1000.0);
    //double seconds = (currentTime - m_startTime) / 1000.0;

    long currentTime = (tv.tv_usec / 1000 + tv.tv_sec * 1000);
    seconds += float(currentTime - m_startTime) / 1000.0f;

    fps = 1.0f / seconds;

    PRINT_DEBUG_LVL(5, "Start time: %ld, current time: %ld\n", m_startTime, currentTime);
    PRINT_DEBUG_LVL(5, "Seconds: %f\n", seconds);

    m_startTime = currentTime;
  }
#endif

  return seconds;
}

//------------------------------------------------------------------------------
float GameTimer::getFPS(void) {
#ifdef _WIN32
  unsigned long elapsedFrames = 1;
  static LARGE_INTEGER s_lastTime = m_startTime;
  LARGE_INTEGER currentTime;

  QueryPerformanceCounter(&currentTime);

  float fps = (float)elapsedFrames * (float)m_ticksPerSecond.QuadPart /
    ((float)currentTime.QuadPart - (float)s_lastTime.QuadPart);

  // reset the timer
  s_lastTime = currentTime;

#endif
  /*
     long m_lastTime = m_startTime;

     timeval tv;
     gettimeofday(&tv, 0);
     long currentTime = tv.tv_usec / 1000 + (tv.tv_sec * 1000);

     float fps = 1.0f / (float)(currentTime - m_lastTime) * 1000.0f;
     */

  return fps;
}
