#include <windows.h>
#include <GL/glut.h>

#include <string>
#include <fstream>
#include <iostream>
#include <stdio.h>

#include "matrix.h"
#include "Quaternion.h"
#include "sound.h"
#include "GameTimer.h"
#include "KeyState.h"

using namespace std;

#ifndef _CONSTANTS_H_
#define _CONSTANTS_H_

#define DEMO        0
#define PLAY_DEMO   0

#define EDIT        1

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
#define TEXT_SCRIPT_SET_VELOCITY    91
#define TEXT_SCRIPT_SET_POSITION    92
#define TEXT_SCRIPT_SET_COLOR       93
#define TEXT_SCRIPT_SET_FADE        94
#define TEXT_SCRIPT_SET_ALPHA       95
#define TEXT_SCRIPT_SET_VISIBLE     96
#define TEXT_SCRIPT_SET_SIZE        97
#define TEXT_SCRIPT_SET_TYPE        98
/////////////////////////////////////////            

#endif
