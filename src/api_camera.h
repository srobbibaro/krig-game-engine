#ifndef _API_CAMERA_H_
#define _API_CAMERA_H_

extern "C" {
  #include "lua5.1/lua.h"
  #include "lua5.1/lualib.h"
  #include "lua5.1/lauxlib.h"
}

int luaopen_krigCamera (lua_State *L);

#endif
