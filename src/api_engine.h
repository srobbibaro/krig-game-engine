#ifndef _API_ENGINE_H_
#define _API_ENGINE_H_

extern "C" {
  #include "lua5.1/lua.h"
  #include "lua5.1/lualib.h"
  #include "lua5.1/lauxlib.h"
}

static int display_text(lua_State *L);
static int get_camera(lua_State *L);
static int get_fps(lua_State *L);
static int get_mouse_coordinates(lua_State *L);
static int get_player(lua_State *L);
static int get_script_value(lua_State *L);
static int play_sound(lua_State *L);
static int render_text(lua_State *L);
static int set_script_value(lua_State *L);
static int shutdown(lua_State *L);
static int test_debug_enabled(lua_State *L);
static int test_key_pressed(lua_State *L);
static int test_key_released(lua_State *L);
static int test_special_key_pressed(lua_State *L);
static int test_special_key_released(lua_State *L);

int luaopen_krigEngine (lua_State *L);

#endif
