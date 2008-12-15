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
#include "EventQueue.h"
#include "Credits.h"
#include "Menu.h"

using namespace std;

#ifndef _CONSTANTS_H_
#define _CONSTANTS_H_

#define DEMO        0
#define PLAY_DEMO   0

#define EDIT        1

// sound library constants //
#define BUFFER_SIZE 65536
/////////////////////////////

// object types ////////////////
#define OBJECT_PLAYER       0
#define OBJECT_ENEMY_SHIP   1
#define OBJECT_ASTEROID     2
#define OBJECT_BEAM         3
#define OBJECT_SAIL_BOAT    4
#define OBJECT_BOATCANNON   5
#define OBJECT_BOSS         6
#define OBJECT_POWERUP_1    7
#define OBJECT_POWERUP_2    8
////////////////////////////////

// object states ////////////////////////////
#define DEAD        0
#define NORMAL      1
#define INVUL       2
#define EXPLOSION   3
#define BACKGROUND  4
/////////////////////////////////////////////
    
// animation times //////////////////////////
#define EXPLOSION_DURATION  2.0f
#define INVUL_DURATION      2.0f
/////////////////////////////////////////////

// Object Extended Scripting Language ////
#define SCRIPT_SET_TEST_ZONE_ENABLE_VAL 136
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
        
#define ACTIVE_VOLUME_LENGTH 400.0f

#define NO_SCRIPT           ""
#define SHOT_SPEED          25.0f

#define MAX_PATH_LEN        128

#endif
