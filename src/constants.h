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

// CONTROL COMMANDS //
#define NO_COMMAND          0
#define VEL_UP_KEY_DOWN     1
#define VEL_DOWN_KEY_DOWN   2
#define VEL_LEFT_KEY_DOWN   3
#define VEL_RIGHT_KEY_DOWN  4
#define VEL_UP_KEY_UP       5
#define VEL_DOWN_KEY_UP     6
#define VEL_LEFT_KEY_UP     7
#define VEL_RIGHT_KEY_UP    8
#define SHOOT_DOWN          9
#define TOGGLE_MENU         13
#define MOVE_CURSOR         14
#define SELECT_OPTION       15
#define QUIT_GAME           16
#define TOGGLE_LOD          17
#define TOGGLE_PARTICLE     18
#define TOGGLE_TERRAIN      19
#define MISSILE_DOWN        20
////////////////////////

#define DEMO        0
#define PLAY_DEMO   0

#define EDIT        1

//terrain parameters //
#define X_SIZE 512         // 20
#define Z_SIZE 256         // 7
#define SCALE_FACTOR 5     //7
///////////////////////

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
#define SCRIPT_ADD_SCALE_MEM        130
#define SCRIPT_ADD_SCALE_RATE_MEM   131

#define SCRIPT_ADD_SCALE_VAL        134
#define SCRIPT_ADD_SCALE_RATE_VAL   135


#define SCRIPT_SET_TEST_ZONE_ENABLE_VAL 136
#define SCRIPT_SET_STATE_VAL            137
#define SCRIPT_STORE_STATE_IN_MEM       138

#define SCRIPT_STORE_SCALE_IN_MEM       141
#define SCRIPT_STORE_SCALE_RATE_IN_MEM  142
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
