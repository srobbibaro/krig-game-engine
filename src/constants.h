//#include <windows.h>
#include <GL/glut.h>

#include <string>
#include <fstream>
#include <stdio.h>

#include "matrix.h"
#include "Quaternion.h"
#include "SoundFX.h"
#include "gametimer.h"
#include "KeyState.h"

using namespace std;

#ifndef _CONSTANTS_H_
#define _CONSTANTS_H_

#define DEMO        0
#define PLAY_DEMO   0

#define EDIT        1
#define DEBUG       0

// sound library constants //
#define BUFFER_SIZE 65536
/////////////////////////////

#define MAX_PATH_LEN        128

// object states ////////////////////////////
#define DEAD        0
#define NORMAL      1
/////////////////////////////////////////////

// Object Extended Scripting Language ////
#define SCRIPT_SET_STATE_VAL            137
#define SCRIPT_STORE_STATE_IN_MEM       138
//////////////////////////////////////////

// Text Extended Scripting Language ////
#define TEXT_SCRIPT_CLEAR_ALL       90
#define TEXT_SCRIPT_SET_VISIBLE     96
#define TEXT_SCRIPT_SET_TYPE        98
/////////////////////////////////////////

#endif
