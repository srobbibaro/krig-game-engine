#ifndef _API_ROTATION_H_
#define _API_ROTATION_H_

extern "C" {
  #include "lua5.1/lua.h"
  #include "lua5.1/lualib.h"
  #include "lua5.1/lauxlib.h"
}

int luaopen_krigRotation (lua_State *L);

#endif
