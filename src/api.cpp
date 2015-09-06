/**
 * @file api.cpp
 * @brief Krig Game Engine Lua Scripting API
 *
 * API calls are split into seven classes of functions. The classes are grouped
 * by the type of functionality they provide. For example, the
 * krig.object.set_position function is supported for a game objects, while
 * krig.level.add_object function is supported for game levels. The seven classes 
 * of functions are: Game Object, Game Level, Text, Terrain, Camera, Vector Math,
 * and Game (Engine).
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

void luaopen_krigApi(lua_State *L) {
  luaopen_krigEngine(L);
  luaopen_krigLevel(L);
  luaopen_krigObject(L);
  luaopen_krigText(L);
  luaopen_krigTerrain(L);
  luaopen_krigCamera(L);
  luaopen_krigVector(L);
}
