#ifndef _CONSTANTS_H_
#define _CONSTANTS_H_

#include <cstring>

// Switches ///////////////////////////////////////////////////////////////////
/*
The FULL_SCREEN flag is disabled by default (set to 0).
When the this flag is enabled (set to 1), Krig will launch with full screen
rendering. Full screen rendering is not compatible with the EDIT flag and will
be ignored when EDIT is also enabled.
*/
#define FULL_SCREEN   0
/*
The EDIT flag is disabled by default (set to 0).
When the EDIT flag is enabled (set to 1), Krig will launch in edit mode. This
mode is designed to aid in level creation. Edit mode gives users the ability to
move around the level freely, manipulate the terrain, and experiment with
certain level-specific variables which can be tweaked on the fly.
*/
#ifndef DEBUG
#define EDIT          0
/*
The DEBUG flag is disabled by default (set to 0).
To enable debug mode, set this value to 1. Debug mode will output debug messages
to stdout in the console. Additionally, the state of debug flag can be queried
from game scripts to provide debug-specific behavior.
*/
#define DEBUG         0
/*
The MSG_LVL setting is used with the DEBUG flag and is set to 0 by default.
Lower values will output minimal debug. While higher values will output an
increasingly larger number of messages.
Be careful - higher numbers will output a lot of information!
*/
#define MSG_LVL       0
#endif

// General Constants //////////////////////////////////////////////////////////
#define MAX_PATH_LEN  128

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
