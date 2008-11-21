#include <windows.h>
#include <GL/glut.h>

#include <string>
#include <fstream>
#include <iostream>
#include <stdio.h>

#include "matrix.h"
#include "Quaternion.h"
#include "Script.h"
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

// scripting constants //////////////////////
#define MAX_SCRIPT_COMMANDS             256
#define MAX_COMMANDS_PER_CYCLE          50

#define MEM_LEN                         40

#define MEM_SCRIPT_VAR_INDEX            0
#define MEM_NEXT_SCRIPT_INDEX           38
#define MEM_NEXT_SCRIPT_COMMAND_INDEX   39             

// relational operators //
#define OP_EQ   0
#define OP_NE   1
#define OP_LT   2
#define OP_GT   3
#define OP_LE   4
#define OP_GE   5

// arithmetic operators //
#define OP_ADD  6
#define OP_SUB  7
#define OP_MUL  8
#define OP_DIV  9
#define OP_MOD  10


// Base Scripting Language /////////////////
#define NUM_BASE_COMMANDS               128

#define SCRIPT_NO_OP                    0

#define SCRIPT_ARITH_VAL_AOP_VAL        1
#define SCRIPT_ARITH_VAL_AOP_MEM        2
#define SCRIPT_ARITH_MEM_AOP_VAL        3
#define SCRIPT_ARITH_MEM_AOP_MEM        4

#define SCRIPT_BRANCH_VAL_REL_VAL       5
#define SCRIPT_BRANCH_VAL_REL_MEM       6
#define SCRIPT_BRANCH_MEM_REL_VAL       7
#define SCRIPT_BRANCH_MEM_REL_MEM       8

#define SCRIPT_STORE_VAL_IN_MEM         9
#define SCRIPT_STORE_MEM_IN_MEM         10

#define SCRIPT_JUMP                     11

#define SCRIPT_SET_SCRIPT               12

// 13-30 reserved for expansion //

// end base scripting language //////////

// extended base scripting language /////
#define SCRIPT_SET_POSITION_MEM         30 
#define SCRIPT_SET_ROTATION_MEM         31
#define SCRIPT_SET_VELOCITY_MEM         32
#define SCRIPT_SET_SPEED_MEM            33
#define SCRIPT_SET_ROT_VEL_MEM          34

#define SCRIPT_SET_POSITION_VAL         35 
#define SCRIPT_SET_ROTATION_VAL         36
#define SCRIPT_SET_VELOCITY_VAL         37
#define SCRIPT_SET_SPEED_VAL            38
#define SCRIPT_SET_ROT_VEL_VAL          39

// shortcuts /////
#define SCRIPT_ADD_POSITION_MEM         40
#define SCRIPT_ADD_ROTATION_MEM         41
#define SCRIPT_ADD_VELOCITY_MEM         42
#define SCRIPT_ADD_SPEED_MEM            43
#define SCRIPT_ADD_ROT_VEL_MEM          44

#define SCRIPT_ADD_POSITION_VAL         45
#define SCRIPT_ADD_ROTATION_VAL         46
#define SCRIPT_ADD_VELOCITY_VAL         47
#define SCRIPT_ADD_SPEED_VAL            48
#define SCRIPT_ADD_ROT_VEL_VAL          49
//////////////////

#define SCRIPT_STORE_POSITION_IN_MEM    50
#define SCRIPT_STORE_ROTATION_IN_MEM    51
#define SCRIPT_STORE_VELOCITY_IN_MEM    52
#define SCRIPT_STORE_SPEED_IN_MEM       53
#define SCRIPT_STORE_ROT_VEL_IN_MEM     54

#define SCRIPT_STORE_TIMER_IN_MEM       55

#define SCRIPT_SET_INTERP_ROTATION_START_MEM    56
#define SCRIPT_SET_INTERP_ROTATION_START_VAL    57

#define SCRIPT_SET_INTERP_ROTATION_END_MEM      58
#define SCRIPT_SET_INTERP_ROTATION_END_VAL      59

#define SCRIPT_SET_INTERP_VAR_VAL_VAL           60
#define SCRIPT_SET_INTERP_VAR_VAL_MEM           61
#define SCRIPT_SET_INTERP_VAR_MEM_VAL           62
#define SCRIPT_SET_INTERP_VAR_MEM_MEM           63

#define SCRIPT_SET_INTERP_ENABLE                64


#define SCRIPT_STORE_PLR_POSITION_IN_MEM 65
#define SCRIPT_STORE_PLR_ROTATION_IN_MEM 66
#define SCRIPT_STORE_PLR_VELOCITY_IN_MEM 67
#define SCRIPT_STORE_PLR_SPEED_IN_MEM    68
#define SCRIPT_STORE_PLR_ROT_VEL_IN_MEM  69

#define SCRIPT_STORE_CAM_POSITION_IN_MEM 70
#define SCRIPT_STORE_CAM_ROTATION_IN_MEM 71
#define SCRIPT_STORE_CAM_VELOCITY_IN_MEM 72
#define SCRIPT_STORE_CAM_SPEED_IN_MEM    73
#define SCRIPT_STORE_CAM_ROT_VEL_IN_MEM  74

#define SCRIPT_STORE_RND_IN_MEM         75
#define SCRIPT_STORE_SIN_IN_MEM         76
#define SCRIPT_STORE_COS_IN_MEM         77
#define SCRIPT_STORE_TAN_IN_MEM         78
#define SCRIPT_STORE_ASIN_IN_MEM        79
#define SCRIPT_STORE_ACOS_IN_MEM        80
#define SCRIPT_STORE_ATAN_IN_MEM        81

#define SCRIPT_STORE_DIR_IN_MEM         82
#define SCRIPT_STORE_PLR_DIR_IN_MEM     83
#define SCRIPT_STORE_CAM_DIR_IN_MEM     84

#define SCRIPT_STORE_ORTH_DIR_IN_MEM    85
#define SCRIPT_STORE_PLR_ORTH_DIR_IN_MEM    86
#define SCRIPT_STORE_CAM_ORTH_DIR_IN_MEM    87

#define SCRIPT_STORE_UP_DIR_IN_MEM          88
#define SCRIPT_STORE_PLR_UP_DIR_IN_MEM      89
#define SCRIPT_STORE_CAM_UP_DIR_IN_MEM      90

#define SCRIPT_SET_ROT_VEL_AXIS_MEM         91
#define SCRIPT_SET_ROT_VEL_AXIS_VAL         92

#define SCRIPT_SET_ROTATION_AXIS_MEM        93
#define SCRIPT_SET_ROTATION_AXIS_VAL        94

#define SCRIPT_ADD_ROTATION_AXIS_MEM        95
#define SCRIPT_ADD_ROTATION_AXIS_VAL        96

#define SCRIPT_SET_INTERP_ROTATION_START_AXIS_MEM    97
#define SCRIPT_SET_INTERP_ROTATION_START_AXIS_VAL    98

#define SCRIPT_SET_INTERP_ROTATION_END_AXIS_MEM      99
#define SCRIPT_SET_INTERP_ROTATION_END_AXIS_VAL      100










// 73-90 reserved for expansion

// Object Extended Scripting Language ////
#define SCRIPT_SET_SCALE_MEM        128
#define SCRIPT_SET_SCALE_RATE_MEM   129
#define SCRIPT_ADD_SCALE_MEM        130
#define SCRIPT_ADD_SCALE_RATE_MEM   131

#define SCRIPT_SET_SCALE_VAL        132
#define SCRIPT_SET_SCALE_RATE_VAL   133
#define SCRIPT_ADD_SCALE_VAL        134
#define SCRIPT_ADD_SCALE_RATE_VAL   135


#define SCRIPT_SET_TEST_ZONE_ENABLE_VAL 136
#define SCRIPT_SET_STATE_VAL            137
#define SCRIPT_STORE_STATE_IN_MEM       138
#define SCRIPT_STORE_LIFE_IN_MEM        139

#define SCRIPT_FIRE_WEAPON              140

#define SCRIPT_STORE_SCALE_IN_MEM       141
#define SCRIPT_STORE_SCALE_RATE_IN_MEM  142
//////////////////////////////////////////

// Camera Extended Scripting Language ////
#define SCRIPT_SET_PLR_POSITION_MEM         128 
#define SCRIPT_SET_PLR_ROTATION_MEM         129
#define SCRIPT_SET_PLR_VELOCITY_MEM         130
#define SCRIPT_SET_PLR_SPEED_MEM            131
#define SCRIPT_SET_PLR_ROT_VEL_MEM          132

#define SCRIPT_SET_PLR_POSITION_VAL         133 
#define SCRIPT_SET_PLR_ROTATION_VAL         134
#define SCRIPT_SET_PLR_VELOCITY_VAL         135
#define SCRIPT_SET_PLR_SPEED_VAL            136
#define SCRIPT_SET_PLR_ROT_VEL_VAL          137

// shortcuts /////
#define SCRIPT_ADD_PLR_POSITION_MEM         138
#define SCRIPT_ADD_PLR_ROTATION_MEM         139
#define SCRIPT_ADD_PLR_VELOCITY_MEM         140
#define SCRIPT_ADD_PLR_SPEED_MEM            141
#define SCRIPT_ADD_PLR_ROT_VEL_MEM          142

#define SCRIPT_ADD_PLR_POSITION_VAL         143
#define SCRIPT_ADD_PLR_ROTATION_VAL         144
#define SCRIPT_ADD_PLR_VELOCITY_VAL         145
#define SCRIPT_ADD_PLR_SPEED_VAL            146
#define SCRIPT_ADD_PLR_ROT_VEL_VAL          147

#define SCRIPT_SET_PLR_ROT_VEL_AXIS_MEM         148
#define SCRIPT_SET_PLR_ROT_VEL_AXIS_VAL         149

#define SCRIPT_SET_PLR_ROTATION_AXIS_MEM        150
#define SCRIPT_SET_PLR_ROTATION_AXIS_VAL        151

#define SCRIPT_ADD_PLR_ROTATION_AXIS_MEM        152
#define SCRIPT_ADD_PLR_ROTATION_AXIS_VAL        153

/////////////////////////////////////////////

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
        

// models ///////////////////////////
#define NUM_MODELS 22
                    
#define MODEL_PLAYER        "Ship.mdl"
#define MODEL_ENEMY_SHIP    "Enemy.mdl"
#define MODEL_ASTEROID      "Asteroid.mdl"
#define MODEL_BEAM          "Beam.mdl"
#define MODEL_BOSS_A        "Boss.mdl"
#define MODEL_SAIL_BOAT     "SailBoat.mdl"
#define MODEL_BOAT_CANNON   "BoatCannon.mdl"
#define MODEL_ARWING        "Arwing.mdl"
#define MODEL_NESKIMOS      "NeskimosLogo.mdl"
#define MODEL_EXPLOSION     "Explosion.mdl"
#define MODEL_BLUE_SHOT     "blueshot.mdl"
#define MODEL_GREEN_SHOT    "GreenShot.mdl"
#define MODEL_RED_MISSLE    "Missle.mdl"
#define MODEL_CANNON_BALL   "CannonBall.mdl"
#define MODEL_POWERUP_1     "Powerup.mdl"
#define MODEL_POWERUP_2     "Powerup2.mdl"
#define MODEL_BUILDING      "Building.mdl"
#define MODEL_FISH_BOSS     "FishBoss.mdl"
#define MODEL_SNOW_BOSS     "snowboss.mdl"
#define MODEL_CACTUS_1      "Cactus1.mdl"
#define MODEL_CACTUS_2      "Cactus2.mdl"
#define MODEL_ENEMY_MISSLE  "EnemyMissle.mdl"
////////////////////////////////////

#define ACTIVE_VOLUME_LENGTH 400.0f

#define NO_SCRIPT           ""
#define SHOT_SPEED          25.0f

#define MAX_PATH_LEN        128


#endif
