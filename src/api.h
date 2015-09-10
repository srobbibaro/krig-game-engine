#ifndef _API_H_
#define _API_H_

extern "C" {
  #include "lua5.1/lua.h"
  #include "lua5.1/lualib.h"
  #include "lua5.1/lauxlib.h"
}

#include "Engine.h"
#include "GameLevel.h"
#include "Object.h"
#include "matrix.h"

// Macros /////////////////////////////////////////////////////////////////////
#define SCRIPT_CALLBACK_ON_LOAD        "on_load"
#define SCRIPT_CALLBACK_ON_UPDATE      "on_update"
#define SCRIPT_CALLBACK_ON_DRAW        "on_draw"
#define SCRIPT_CALLBACK_ON_DRAW_SCREEN "on_draw_screen"
#define SCRIPT_CALLBACK_ON_COLLISION   "on_collision"
#define SCRIPT_CALLBACK_ON_UNLOAD      "on_unload"

// Global variables ///////////////////////////////////////////////////////////
extern Engine* g_KRIG_ENGINE;

// Functions /////// ///////////////////////////////////////////////////////////
Vector loadVector(lua_State *L);
Vector loadVector(lua_State *L, int &index);
Object* loadObject(lua_State *L, const int &index);
void returnVector(lua_State *L, Vector &t);
void returnObject(lua_State *L, Object* object);
void luaopen_krigApi(lua_State *L);

#if !defined LUA_VERSION_NUM || LUA_VERSION_NUM==501
/*
 * Adapted from Lua 5.2.0
 */
static void luaL_setfuncs (lua_State *L, const luaL_Reg *l, int nup) {
  luaL_checkstack(L, nup+1, "too many upvalues");
  for (; l->name != NULL; l++) {  /* fill the table with given functions */
    int i;
    lua_pushstring(L, l->name);
    for (i = 0; i < nup; i++)  /* copy upvalues to the top */
      lua_pushvalue(L, -(nup+1));
    lua_pushcclosure(L, l->func, nup);  /* closure with those upvalues */
    lua_settable(L, -(nup + 3));
  }
  lua_pop(L, nup);  /* remove upvalues */
}
#endif
#endif
