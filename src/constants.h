//#include <windows.h>
#include <GL/glut.h>

#include <string>
#include <cstring>
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

#define EDIT        0
#define DEBUG       0
#define MSG_LVL     0

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

#define PRINT_DEBUG(format, ...) \
    PRINT_DEBUG_LVL(0, format, ##__VA_ARGS__)

#define PRINT_DEBUG_LVL(level, format, ...) \
    if (DEBUG) PRINT_MESSAGE(level, format, "DEBUG", ##__VA_ARGS__)

#define PRINT_ERROR(format, ...) \
    PRINT_MESSAGE(0, format, "ERROR", ##__VA_ARGS__)

#define PRINT_MESSAGE(level, format, type, ...) \
    if (level <= MSG_LVL) \
        printf("[%s|%s:%s] " format, type, \
                strrchr(__FILE__, '/') + 1, \
                __FUNCTION__, ##__VA_ARGS__);

#endif
