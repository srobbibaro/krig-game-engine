Krig Game Engine
================

Krig is an open source game engine and accompanying demo game (by the same name) 
created to run on the Linux operating system.

## Getting Started

### Build

At this time, Krig is known to build and run on 32-bit Debian-based Linux 
distributions running GNOME. Other distributions and configurations should work,
but have not been verified. Code::Blocks is used to build the Krig executable.

### Setup

Install the following packages:
* codeblocks
* lua5.1
* lua5.1-dev
* libalut-dev
* libvorbis-dev 
* libglu1-mesa-dev
* freeglut3
* freeglut3-dev
* g++

For example: 

```bash
$ apt-get install codeblocks lua5.1 lua5.1-dev libalut-dev libvorbis-dev libglu1-mesa-dev freeglut3 freeglut3-dev g++
```

### Compile

After the required packages have been installed, launch Code::Blocks and load the _krig.cbp_ 
project file. Krig should now build with Code::Blocks.

Krig can also be compiled (using Code::Blocks) from the command line.

For example: 

```bash
$ codeblocks --build krig.cbp
```
#### Enable Debug Mode

The DEBUG flag is located in _src/constants.h_ and is disabled by default (set to 0).
To enable debug mode, set this value to 1. Debug mode will output debug messages to
stdout in the console. Additionally, some information about the current level
will be output directly to the play screen (for example: player position, camera position, 
and FPS).

```c
#define DEBUG 0
```

The stdout debug message level is controlled by the MSG_LVL setting found in _src/constants.h_. 
By default, this value is set to 0 which will output minimal debug. This number 
can be set higher to see an increasingly larger number of messages.
Be careful - higher numbers will output a lot of information!

```c
#define MSG_LVL 0
```

#### Enable Edit Mode

The EDIT flag is located in _src/constants.h_ and is disabled by default (set to 0).
When the EDIT flag is enabled (set to 1), Krig will launch in edit mode. This mode is
designed to aid in level creation. Edit mode gives users the ability to move around
the level freely, manipulate the terrain, and experiment with certain level-specific
variables which can be tweaked on the fly.

```c
#define EDIT 0
```

### Run

Krig will run from either Code::Blocks or command line. The Krig executable is created in the base project directory and is named _krig_.

For example:

```bash
$ ./krig
```
