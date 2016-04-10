#ifndef _API_LEVEL_H_
#define _API_LEVEL_H_

extern "C" {
  #include "lua5.1/lua.h"
  #include "lua5.1/lualib.h"
  #include "lua5.1/lauxlib.h"
}

int luaopen_krigLevel (lua_State *L);

static int add_object(lua_State *L);
static int add_text(lua_State *L);
static int add_sprite(lua_State *L);
static int find_object_of_type(lua_State *L);
static int get_camera_id(lua_State *L);
static int get_complete(lua_State *L);
static int get_id(lua_State *L);
static int get_light_direction(lua_State *L);
static int load(lua_State *L);
static int load_from_buffer(lua_State *L);
static int pause(lua_State *L);
static int pause_music(lua_State *L);
static int play_music(lua_State *L);
static int remove_object(lua_State *L);
static int set_complete(lua_State *L);
static int set_id(lua_State *L);
static int set_light_direction(lua_State *L);
static int set_sky_box(lua_State *L);
static int set_terrain(lua_State *L);
static int stop_music(lua_State *L);
static int swap(lua_State *L);

#endif
