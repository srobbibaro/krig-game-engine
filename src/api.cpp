/**
 * @file api.cpp
 * @brief Krig Game Engine Lua Scripting API
 *
 * API calls are split into four classes of functions. The classes are defined
 * by the type of script where they are available for use. For example, the
 * playBgMusic function is supported for a game level script and would not be
 * available to use for an object. The four classes of functions are: Game Object,
 * Text, Game Level, and Game (Engine).
 */
#include "api.h"
#include "api_engine.h"
#include "api_level.h"
#include "api_text.h"
#include "api_terrain.h"
#include "api_object.h"
#include "api_vector.h"
#include "api_camera.h"

Vector loadVector(lua_State *L) {
  int index = 2;
  return loadVector(L, index);
}

Vector loadVector(lua_State *L, int &index) {
  Vector t;

  if (lua_istable(L, index)) {
    // x
    lua_pushnumber(L, 1);
    lua_gettable(L, index);
    t.x = (float)lua_tonumber(L, -1);
    lua_pop(L, 1);

    // y
    lua_pushnumber(L, 2);
    lua_gettable(L, index);
    t.y = (float)lua_tonumber(L, -1);
    lua_pop(L, 1);

    // z
    lua_pushnumber(L, 3);
    lua_gettable(L, index);
    t.z = (float)lua_tonumber(L, -1);
    lua_pop(L, 1);

    index++;
  }
  else {
    t.setVector(
      lua_tonumber(L, index),
      lua_tonumber(L, index + 1),
      lua_tonumber(L, index + 2)
    );

    index += 3;
  }

  return t;
}

void returnVector(lua_State *L, Vector &t) {
  lua_newtable(L);

  lua_pushnumber(L, 1);
  lua_pushnumber(L, t.x);
  lua_rawset(L, -3);

  lua_pushnumber(L, 2);
  lua_pushnumber(L, t.y);
  lua_rawset(L, -3);

  lua_pushnumber(L, 3);
  lua_pushnumber(L, t.z);
  lua_rawset(L, -3);
}

void registerFunctions(lua_State *L, int level) {
  luaopen_krigEngine(L);
  luaopen_krigLevel(L);
  luaopen_krigObject(L);
  luaopen_krigText(L);
  luaopen_krigTerrain(L);
  luaopen_krigCamera(L);
  luaopen_krigVector(L);
}
