#!/bin/bash

brew install freeglut freealut libvorbis lua51 doxygen

ln -s /System/Library/Frameworks/OpenAL.framework/Headers/ src/AL
ln -s /System/Library/Frameworks/OpenGL.framework/Headers/ src/GL

sudo ln -s /System/Library/Frameworks/OpenAL.framework/OpenAL /usr/lib/libopenal.a
sudo ln -s /System/Library/Frameworks/OpenGL.framework/OpenGL /usr/lib/libGL.a
sudo ln -s /System/Library/Frameworks/OpenGL.framework/Libraries/libGLU.dylib /usr/lib/libGLU.a
