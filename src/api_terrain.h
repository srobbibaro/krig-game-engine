#ifndef _API_TERRAIN_H_
#define _API_TERRAIN_H_

extern "C" {
  #include "luajit-2.1/lua.h"
  #include "luajit-2.1/lualib.h"
  #include "luajit-2.1/lauxlib.h"
}

static int get_height(lua_State *L);
static int get_vertex_color(lua_State *L);
static int get_vertex_height(lua_State *L);
static int get_vertex_type(lua_State *L);
static int set_vertex_color(lua_State *L);
static int set_vertex_height(lua_State *L);
static int set_vertex_type(lua_State *L);

int luaopen_krigTerrain (lua_State *L);

#endif
