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
* freeglut3-dev
* g++

For example, run this command: 

```bash
$ apt-get install codeblocks lua5.1 lua5.1-dev libalut-dev libvorbis-dev libglu1-mesa-dev freeglut3 freeglut3-dev g++
```

### Compile

Ater the required packages have been installed, launch Code::Blocks and load the _krig.cbp_ 
project file. Krig should now build with Code::Blocks.

Krig can also be compiled (using Code::Blocks) from the command line.

For example: 

```bash
$ codeblocks --build krig.cbp
```

### Run

Krig will run from either Code::Blocks or command line. The Krig executable is created in the base project directory and is named _krig_.

```bash
$ ./krig
```
