#ifndef _API_OBJECT_H_
#define _API_OBJECT_H_

extern "C" {
  #include "luajit-2.1/lua.h"
  #include "luajit-2.1/lualib.h"
  #include "luajit-2.1/lauxlib.h"
}

extern const luaL_Reg krigObjectLib[];

static int add_particle_system(lua_State *L);
static int load(lua_State *L);
static int orient_on_terrain(lua_State *L);
static int save(lua_State *L);
static int set_height_from_terrain(lua_State *L);
static int set_model(lua_State *L);
static int set_script(lua_State *L);
static int setup_interpolation(lua_State *L);
static int suspend(lua_State *L);
static int update_interpolation_value(lua_State *L);

#endif
