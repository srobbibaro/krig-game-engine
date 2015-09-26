#ifndef _API_ROTATION_H_
#define _API_ROTATION_H_

extern "C" {
  #include "lua5.1/lua.h"
  #include "lua5.1/lualib.h"
  #include "lua5.1/lauxlib.h"
}

static int add(lua_State *L);
static int copy(lua_State *L);
static int from_axis(lua_State *L);
static int from_euler(lua_State *L);
static int to_euler(lua_State *L);

int luaopen_krigRotation (lua_State *L);

#endif
