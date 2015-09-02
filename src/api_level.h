#ifndef _API_LEVEL_H_
#define _API_LEVEL_H_

extern "C" {
  #include "lua5.1/lua.h"
  #include "lua5.1/lualib.h"
  #include "lua5.1/lauxlib.h"
}

int luaopen_krigLevel (lua_State *L);
#endif
