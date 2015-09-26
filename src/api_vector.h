#ifndef _API_VECTOR_H_
#define _API_VECTOR_H_

extern "C" {
  #include "lua5.1/lua.h"
  #include "lua5.1/lualib.h"
  #include "lua5.1/lauxlib.h"
}

int luaopen_krigVector(lua_State *L);

static int add(lua_State *L);
static int average(lua_State *L);
static int copy(lua_State *L);
static int cross_product(lua_State *L);
static int distance(lua_State *L);
static int dot_product(lua_State *L);
static int normalize(lua_State *L);
static int scale(lua_State *L);
static int scalar(lua_State *L);
static int subtract(lua_State *L);
static int sum(lua_State *L);

#endif
