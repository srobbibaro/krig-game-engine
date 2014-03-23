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

// Switches ///////////////////////////////////////////////////////////////////
#define DEMO        0
#define PLAY_DEMO   0

#define EDIT        0
#define DEBUG       0
#define MSG_LVL     0

// Sound Library Constants ////////////////////////////////////////////////////
#define BUFFER_SIZE 65536

// General Constants //////////////////////////////////////////////////////////
#define MAX_PATH_LEN    128

// Object States //////////////////////////////////////////////////////////////
#define DEAD        0
#define NORMAL      1

// Object Extended Scripting Language /////////////////////////////////////////
// TODO: These values may no longer be used--verify and remove.
#define SCRIPT_SET_STATE_VAL            137
#define SCRIPT_STORE_STATE_IN_MEM       138

// Text Extended Scripting Language ///////////////////////////////////////////
// TODO: These values may no longer be used--verify and remove.
#define TEXT_SCRIPT_CLEAR_ALL       90
#define TEXT_SCRIPT_SET_VISIBLE     96
#define TEXT_SCRIPT_SET_TYPE        98

// Logging Macros /////////////////////////////////////////////////////////////
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
