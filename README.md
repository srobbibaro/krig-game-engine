Krig Game Engine
================

Krig is a cross-platform, open source game engine with accompanying demo game
(by the same name).

## Getting Started

At this time, Krig is known to build and run on a number of *nix operating systems.

The following configurations are known to work:
* 32/64-bit Debian-based Linux distributions with GNOME or XFCE (native)
* 32-bit Debian-based Linux distributions with GNOME or XFCE (VM with hardware acceleration)
* 64-bit Debian-based Linux distributions with GNOME or XFCE (VM without hardware acceleration)
* Mac OSX Yosemite

Other distributions and configurations should work, but have not been verified.
Code::Blocks is used to build the Krig executable.

### Setup

#### Vagrant

The Krig Game Engine has a Vagrant project which helps automate setup of the
development environment. For information,
see: https://github.com/srobbibaro/krig-game-engine-vagrant

#### Manual

##### Linux

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

##### Mac OSX

Install XQuartz

See instructions, here: http://xquartz.macosforge.org/landing/

Use [Homebrew](http://brew.sh/) to help with package installation.

Install the following packages:
* freeglut
* freealut
* libvorbis
* lua51

For example:

```bash
$ brew install freeglut freealut libvorbis lua51
```

Install GCC:

```bash
$ brew tap homebrew/versions
$ brew install gcc46
```

Use [Homebrew Cask](http://caskroom.io/) to help with CodeBlocks installation.

For example:

```bash
$ brew install caskroom/cask/brew-cask
$ brew cask install codeblocks
```

Create links to required header files:

```bash
$ ln -s /System/Library/Frameworks/OpenAL.framework/Headers/ src/AL
$ ln -s /System/Library/Frameworks/OpenGL.framework/Headers/ src/GL
```

Create links to required library files:

```bash
$ sudo ln -s /System/Library/Frameworks/OpenAL.framework/OpenAL /usr/lib/libopenal.a
$ sudo ln -s /System/Library/Frameworks/OpenGL.framework/OpenGL /usr/lib/libGL.a
$ sudo ln -s /System/Library/Frameworks/OpenGL.framework/Libraries/libGLU.dylib /usr/lib/libGLU.a
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

The `DEBUG` flag is located in _src/constants.h_ and is disabled by default (set to 0).
To enable debug mode, set this value to 1. Debug mode will output debug messages to
stdout in the console. Additionally, some information about the current level
will be output directly to the play screen (for example: player position, camera position,
and FPS). Additionally, the state of debug flag can be queried from game scripts
to provide debug-specific behavior.

```c
#define DEBUG 0
```

The `MSG_LVL` setting found in _src/constants.h_ is used with the `DEBUG` flag and controls
the volume of debug information output to stdout. By default, this value is set
to 0 which will output minimal debug. This number can be set higher to see an
increasingly larger number of messages.
Be careful - higher numbers will output a lot of information!

```c
#define MSG_LVL 0
```

#### Enable Edit Mode

The `EDIT` flag is located in _src/constants.h_ and is disabled by default (set to 0).
When the `EDIT` flag is enabled (set to 1), Krig will launch in edit mode. This mode is
designed to aid in level creation. Edit mode gives users the ability to move around
the level freely, manipulate the terrain, and experiment with certain level-specific
variables which can be tweaked on the fly.

```c
#define EDIT 0
```

#### Enable Full Screen Rendering

The `FULL_SCREEN` flag is located in _src/constants.h_ and is disabled by default (set to 0).
When the this flag is enabled (set to 1), Krig will launch in full screen rendering.
Full screen rendering is not compatible with the `EDIT` flag and will be ignored when
`EDIT` is also enabled.

```c
#define FULL_SCREEN 0
```

### Run

Krig will run from either Code::Blocks or command line.
The Krig executable is created in the base project directory and is named _krig_.
You must supply the path to the game to run as the first argument. The Krig
Game Engine comes with a demo game in the root directory, `krig3`.

For example:

```bash
$ ./krig krig3
```

## Game Scripting

The `lua` programming language is used to script games for the Krig Game Engine. The
engine provides an API designed to handle common game writing tasks. For example,
there are functions which add objects to a level, provide these objects properties,
and notify the script of important events, such as object collisions.

### API

API calls are split into four classes of functions. The classes are defined by
the type of script where they are available for use. For example, the `playBgMusic`
function is supported for a game level script and would not be available to use
for an object. The four classes of functions are: `Object`, `Text`, `Game Level`, and `Game`.

### Object

* addObject
* addParticleSystem
* addPosition
* addPositionv
* addRotation
* addRotationAxis
* addRotationAxisv
* addRotationv
* addRotationVelocity
* addRotationVelocityv
* addScale
* addScaleRate
* addScaleRatev
* addScalev
* addSpeed
* addSpeedv
* addText
* addVelocity
* addVelocityv
* camera_addRotation
* camera_getFrustumPlane
* disableAlwaysLit
* disableCollisionDetection
* disableDraw
* enableAlwaysLit
* enableCollisionDetection
* enableDraw
* engine_getMouseCoordinates
* engine_testDebugEnabled
* engine_testKeyPressed
* engine_testKeyReleased
* engine_testSpecialKeyPressed
* engine_testSpecialKeyReleased
* getActive
* getBoundingSphereRadius
* getCamera
* getCollisionDetectionEnabled
* getDirection
* getDrawEnabled
* getInView
* getOrthogonal
* getPlayer
* getPosition
* getRotation
* getRotationVelocity
* getScale
* getScaleRate
* getScriptValue
* getSpeed
* getTimer
* getTypeId
* getUp
* getVelocity
* level_findObjectOfType
* orientOnTerrain
* playSound
* removeObject
* setActive
* setHeightFromTerrain
* setInactive
* setInterpolationEnable
* setInterpolationRotationEnd
* setInterpolationRotationEndAxis
* setInterpolationRotationEndAxisv
* setInterpolationRotationEndv
* setInterpolationRotationStart
* setInterpolationRotationStartAxis
* setInterpolationRotationStartAxisv
* setInterpolationRotationStartv
* setInterpolationVariableBeginValue
* setInterpolationVariableCurrentValue
* setInterpolationVariableEndValue
* setModel
* setPosition
* setPositionv
* setRotation
* setRotationAxis
* setRotationAxisv
* setRotationv
* setRotationVelocity
* setRotationVelocityAxis
* setRotationVelocityv
* setScale
* setScaleRate
* setScaleRatev
* setScalev
* setScript
* setScriptValue
* setSpeed
* setSpeedv
* setTypeId
* setVelocity
* setVelocityv
* suspend
* terrain_getHeight
* terrain_getVertexColor
* terrain_getVertexHeight
* terrain_getVertexType
* terrain_setVertexColor
* terrain_setVertexHeight
* terrain_setVertexType
* vector_crossProduct
* vector_dotProduct
* vector_getScalar
* vector_normalize

#### Text

* getAlpha
* getColor
* getFadeRate
* getWidth
* setAlpha
* setColor
* setColorv
* setFadeRate
* setWidth

#### Game Level

* displayText
* getCameraId
* getFps
* getLightDirection
* pauseBgMusic
* playBgMusic
* renderText
* setComplete
* setLightDirection
* setLightDirectionv
* setSkyBox
* setTerrain
* shutdown
* stopBgMusic

#### Game

* getLevelId
* loadLevel
* loadLevelFromBuffer
* pause
* setLevelId
* swapLevel
* testLevelComplete
