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
#include "api_camera.h"
#include "api_engine.h"
#include "api_level.h"
#include "api_object.h"
#include "api_rotation.h"
#include "api_terrain.h"
#include "api_vector.h"

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

void returnVector(lua_State *L, const Vector &t) {
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

void loadArray(lua_State *L, float array[], int len, int index) {
  if (lua_istable(L, index)) {
    for (int i = 0; i < len; ++i) {
      /*
      lua_pushnumber(L, (i + 1));
      lua_gettable(L, index);
      array[i] = (float)lua_tonumber(L, -1);
      lua_pop(L, 1);
      */
      array[i] = 1.0f;
    }
  }
}

void returnArray(lua_State *L, float array[], int len) {
  lua_newtable(L);
  for (int i = 0; i < len; ++i) {
    lua_pushnumber(L, (i + 1));
    lua_pushnumber(L, array[i]);
    lua_rawset(L, -3);
  }
}

Quaternion loadQuaternion(lua_State *L, const int &index) {
  float x, y, z, w;

  if (lua_istable(L, index)) {
    // x
    lua_pushnumber(L, 1);
    lua_gettable(L, index);
    x = (float)lua_tonumber(L, -1);
    lua_pop(L, 1);

    // y
    lua_pushnumber(L, 2);
    lua_gettable(L, index);
    y = (float)lua_tonumber(L, -1);
    lua_pop(L, 1);

    // z
    lua_pushnumber(L, 3);
    lua_gettable(L, index);
    z = (float)lua_tonumber(L, -1);
    lua_pop(L, 1);

    // w
    lua_pushnumber(L, 4);
    lua_gettable(L, index);
    w = (float)lua_tonumber(L, -1);
    lua_pop(L, 1);
  }

  return Quaternion(x, y, z, w);
}

void returnQuaternion(lua_State *L, const Quaternion &t) {
  lua_newtable(L);

  lua_pushnumber(L, 1);
  lua_pushnumber(L, t.getX());
  lua_rawset(L, -3);

  lua_pushnumber(L, 2);
  lua_pushnumber(L, t.getY());
  lua_rawset(L, -3);

  lua_pushnumber(L, 3);
  lua_pushnumber(L, t.getZ());
  lua_rawset(L, -3);

  lua_pushnumber(L, 4);
  lua_pushnumber(L, t.getW());
  lua_rawset(L, -3);
}

Object* loadObject(lua_State *L, const int &index) {
  return loadObject(L, g_KRIG_ENGINE->getCurrentLevel(), index);
}

Object* loadObject(lua_State *L, GameLevel *gameLevel, const int &index) {
  int id = -1;
  if (lua_istable(L, index)) {
    lua_pushstring(L, "id");
    lua_gettable(L, index);
    id = lua_tonumber(L, -1);
    lua_pop(L, 1);
  }
  return gameLevel->getObjectFromId(id);
}

GameLevel* loadGameLevel(lua_State *L, const int &index) {
  int id = -1;
  if (lua_istable(L, index)) {
    lua_pushstring(L, "id");
    lua_gettable(L, index);
    id = lua_tonumber(L, -1);
    lua_pop(L, 1);
  }
  return g_KRIG_ENGINE->getGameLevelFromId(id);
}

void returnObject(lua_State *L, Object* object) {
  lua_newtable(L);

  lua_pushstring(L, "id");
  object != NULL ?
    lua_pushnumber(L, object->getGameLevelId()) :
    lua_pushnumber(L, -1);
  lua_rawset(L, -3);
  luaL_setfuncs(L, krigObjectLib, 0);
}

void returnGameLevel(lua_State *L, GameLevel* gameLevel) {
  lua_newtable(L);

  lua_pushstring(L, "id");
  gameLevel != NULL ?
    lua_pushnumber(L, gameLevel->getId()) :
    lua_pushnumber(L, -1);
  lua_rawset(L, -3);
  luaL_setfuncs(L, krigLevelLib, 0);
}

void luaopen_krigApi(lua_State *L) {
  luaopen_krigEngine(L);
  luaopen_krigLevel(L);
  luaopen_krigTerrain(L);
  luaopen_krigVector(L);
  luaopen_krigRotation(L);
}
