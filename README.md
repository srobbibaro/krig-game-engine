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
$ apt-get install lua5.1 lua5.1-dev libalut-dev libvorbis-dev libglu1-mesa-dev freeglut3 freeglut3-dev g++
```

If you prefer to work with an IDE, you can optionally install CodeBlocks:

For example:

```bash
$ apt-get install codeblocks
```

##### Mac OSX

Install [Homebrew](http://brew.sh/) to help with package installation.

Install [Homebrew Cask](http://caskroom.io/) to help with application installation.

Install the following application:

* xquartz

For example:

```bash
$ brew install caskroom/cask/brew-cask
$ brew cask install xquartz
```

If you prefer to work with an IDE, you can optionally install CodeBlocks:

```bash
$ brew cask install codeblocks
```

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

After the required packages have been installed, the project can be built using
the provided makefile.

To build the main executable, run: `$ make` or `$ make build`.

#### Optional: Building with CodeBlocks
After the required packages have been installed, launch Code::Blocks and load the _krig.cbp_
project file. Krig should now build with Code::Blocks.

Krig can also be compiled (using Code::Blocks) from the command line.

For example:

```bash
$ codeblocks --build krig.cbp
```
#### Enable Debug Mode

To build the main executable in debug mode, run: `$ make build-debug`.

Note: You may need to run `$ make clean` prior to running the above command for it
to build the executable properly.

You can also change the setting in source, if you prefer:

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

Note: When you build with `$ make build-debug` a mid-range setting of `2` is assigned.

```c
#define MSG_LVL 0
```

#### Enable Edit Mode

To build the main executable in debug mode, run: `$ make build-edit`.

Note: You may need to run `$ make clean` prior to running the above command for it
to build the executable properly.

You can also change the setting in source, if you prefer:

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

Krig can be run from the command-line using a number of different methods.
The Krig executable is created in the base project directory and is named _krig_.
You must supply the path to the game to run as the first argument. The Krig
Game Engine comes with a demo game in the root directory, `krig3`.

For example:

```bash
$ ./krig krig3
```

The `run-demo` make target will also run the above command. For example:

```bash
$ make run-demo
```

Optionally the game can be run from within CodeBlocks, but the game directory `krig3`
must be specified for it to run properly.

### Run Tests

Unit tests can be run with the following command:

```bash
$ make run-tests
```

### Generate Doxygen Documentation

Doxygen documentation is built (in the `html` directory) using the following command:

```bash
$ make doc
```

## Game Scripting

The `lua` programming language is used to script games for the Krig Game Engine. The
engine provides an API designed to handle common game writing tasks. For example,
there are functions which add objects to a level, provide these objects properties,
and notify the script of important events, such as object collisions.

See the [API documentation](http://srobbibaro.github.io/krig-game-engine/Scripting_8h.html) for game scripting details.
