#!/bin/bash

brew install homebrew/x11/freeglut
brew install freealut libvorbis lua51 doxygen

ln -s /System/Library/Frameworks/OpenAL.framework/Headers/ src/AL
ln -s /System/Library/Frameworks/OpenGL.framework/Headers/ src/GL

ln -s /System/Library/Frameworks/OpenAL.framework/OpenAL /usr/local/lib/libopenal.a
ln -s /System/Library/Frameworks/OpenGL.framework/OpenGL /usr/local/lib/libGL.a
ln -s /System/Library/Frameworks/OpenGL.framework/Libraries/libGLU.dylib /usr/local/lib/libGLU.a
