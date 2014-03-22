//////////////////////////////////////////////////////////////////
// Description : Handles timing and delay during gameplay       //
//////////////////////////////////////////////////////////////////
//#include <windows.h>

#include <sys/time.h>
#include "gametimer.h"


using namespace std;

//------------------------------------------------------------------------------
bool GameTimer::init(void)
{
    /*
  if (!QueryPerformanceFrequency(&m_ticksPerSecond))

    return false;  // system doesn't support high-res timer

  else
  {
    QueryPerformanceCounter(&m_startTime);
    return true;
  }
  */
  timeval tv;
  gettimeofday(&tv, 0);
  //m_startTime = ((double)tv.tv_usec / 1000.0 + (double)tv.tv_sec * 1000.0);
  m_startTime = tv.tv_usec / 1000 + tv.tv_sec * 1000;

  fps = 0.0f;

  return true;

}

//------------------------------------------------------------------------------
float GameTimer::getElapsedSeconds( unsigned long )
{
    /*
  static LARGE_INTEGER s_lastTime = m_startTime;
  LARGE_INTEGER currentTime;

  QueryPerformanceCounter(&currentTime);

  float seconds =  ((float)currentTime.QuadPart - (float)s_lastTime.QuadPart) / (float)m_ticksPerSecond.QuadPart;

  // reset the timer
  s_lastTime = currentTime;

  return seconds;
  */

  fps = 100.0f;
  float seconds = 0.0f;

  int tries = 0;

  while (fps >= 60.0f || ++tries > 100) {
    timeval tv;
    gettimeofday(&tv, 0);
    //double currentTime = ((double)tv.tv_usec / 1000.0 + (double)tv.tv_sec * 1000.0);

    //double seconds = (currentTime - m_startTime) / 1000.0;

    long currentTime = (tv.tv_usec / 1000 + tv.tv_sec * 1000);

    seconds += float(currentTime - m_startTime) / 1000.0f;


    fps = 1.0f / seconds;
    //  cout << "s: " << m_startTime << " c: " << currentTime << endl;
    //cout << "seconds: " << seconds << endl;

    m_startTime = currentTime;
  }

  return seconds;
}

//------------------------------------------------------------------------------
float GameTimer::getFPS(void)
{
    /*
  unsigned long elapsedFrames = 1;
  static LARGE_INTEGER s_lastTime = m_startTime;
  LARGE_INTEGER currentTime;

  QueryPerformanceCounter(&currentTime);

  float fps = (float)elapsedFrames * (float)m_ticksPerSecond.QuadPart / ((float)currentTime.QuadPart - (float)s_lastTime.QuadPart);

  // reset the timer

  s_lastTime = currentTime;

  return fps;
  */

  /*

  long m_lastTime = m_startTime;

  timeval tv;
  gettimeofday(&tv, 0);
  long currentTime = tv.tv_usec / 1000 + (tv.tv_sec * 1000);

  float fps = 1.0f / (float)(currentTime - m_lastTime) * 1000.0f;
  */

  return fps;
} // end GetFPS

