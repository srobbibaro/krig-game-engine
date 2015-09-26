/**
 * @file api_terrain.cpp
 * @brief Krig Game Engine Lua Scripting API - Terrain
 *
 * Terrain API functionality.
 */
#include "api.h"
#include "api_terrain.h"
#include "GameLevel.h"

#if DOXYGEN_ONLY
/**
 * Get height of terrain at specified x,z grid coordinates.
 * @param int x coordinate.
 * @param int z coordinate.
 * @return float
 */
float get_vertex_height(int, int);
#endif
static int get_vertex_height(lua_State *L) {
  int x = lua_tonumber(L, 1);
  int z = lua_tonumber(L, 2);

  float height = (float)g_KRIG_ENGINE->getCurrentLevel()->getTerrain()->getVertexHeight(x, z);

  lua_pushnumber(L, height);
  return 1;
}

#if DOXYGEN_ONLY
/**
 * Set height of terrain at specified x,z grid coordinates.
 * @param int x coordinate.
 * @param int z coordinate.
 * @param float height
 * @return n/a.
 */
void set_vertex_height(int, int, float);
#endif
static int set_vertex_height(lua_State *L) {
  int x = lua_tonumber(L, 1);
  int z = lua_tonumber(L, 2);
  float height = lua_tonumber(L, 3);

  g_KRIG_ENGINE->getCurrentLevel()->getTerrain()->setVertexHeight(x, z, height);
  return 0;
}

#if DOXYGEN_ONLY
/**
 * Get color of terrain at specified x,z grid coordinates.
 * @param int x coordinate.
 * @param int z coordinate.
 * @return Vector3
 */
Vector3 get_vertex_color(int, int);
#endif
static int get_vertex_color(lua_State *L) {
  int x = lua_tonumber(L, 1);
  int z = lua_tonumber(L, 2);

  Vector color = g_KRIG_ENGINE->getCurrentLevel()->getTerrain()->getVertexColor(x, z);

  returnVector(L, color);
  return 1;
}

#if DOXYGEN_ONLY
/**
 * Set type of terrain at specified x,z grid coordinates.
 * @param int x coordinate.
 * @param int z coordinate.
 * @param Vector3 containing r,g,b values.
 * @return n/a.
 */
void set_vertex_color(int, int, Vector3);
#endif
static int set_vertex_color(lua_State *L) {
  int x = lua_tonumber(L, 1);
  int z = lua_tonumber(L, 2);
  int index = 3;
  Vector color = loadVector(L, index);

  g_KRIG_ENGINE->getCurrentLevel()->getTerrain()->setVertexColor(x, z, color);
  return 0;
}

#if DOXYGEN_ONLY
/**
 * Get type of terrain at specified x,z grid coordinates.
 * @param int x coordinate.
 * @param int z coordinate.
 * @return float - type.
 */
float get_vertex_type(int, int);
#endif
static int get_vertex_type(lua_State *L) {
  int x = lua_tonumber(L, 1);
  int z = lua_tonumber(L, 2);

  float type = g_KRIG_ENGINE->getCurrentLevel()->getTerrain()->getVertexType(x, z);

  lua_pushnumber(L, type);
  return 1;
}

#if DOXYGEN_ONLY
/**
 * Set type of terrain at specified x,z grid coordinates.
 * @param int x coordinate.
 * @param int z coordinate.
 * @param int type.
 * @return n/a.
 */
void set_vertex_type(int, int, int);
#endif
static int set_vertex_type(lua_State *L) {
  int x = lua_tonumber(L, 1);
  int z = lua_tonumber(L, 2);
  int type = lua_tonumber(L,3);

  g_KRIG_ENGINE->getCurrentLevel()->getTerrain()->setVertexType(x, z, type);
  return 0;
}

#if DOXYGEN_ONLY
/**
 * Get height of terrain at specified x,z world coordinates.
 * @param float x coordinate.
 * @param float z coordinate.
 * @return float - height.
 */
float get_height(float, float);
#endif
static int get_height(lua_State *L) {
  float x = lua_tonumber(L, 1);
  float z = lua_tonumber(L, 2);

  float height = (float)g_KRIG_ENGINE->getCurrentLevel()->getTerrain()->getHeight(x, z);

  lua_pushnumber(L, height);
  return 1;
}

static const luaL_reg krigTerrainLib[] = {
  {"get_height", get_height},
  {"get_vertex_color", get_vertex_color},
  {"get_vertex_height", get_vertex_height},
  {"get_vertex_type", get_vertex_type},
  {"set_vertex_color", set_vertex_color},
  {"set_vertex_height", set_vertex_height},
  {"set_vertex_type", set_vertex_type},
  {NULL, NULL}
};

int luaopen_krigTerrain (lua_State *L) {
  luaL_openlib(L, "krig.terrain", krigTerrainLib, 0);
  return 1;
}
