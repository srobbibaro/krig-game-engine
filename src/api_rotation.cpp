/**
 * @file api_vector.cpp
 * @brief Krig Game Engine Lua Scripting API - Vector
 *
 * Vector focused API functionality.
 */
#include "api.h"
#include "api_rotation.h"

#if DOXYGEN_ONLY
/**
 * Calculate scalar value between two vectors.
 * @param Vector
 * @param Vector
 * @return float
 */
float add(Vector, Vector);
#endif
static int add(lua_State *L) {
  Quaternion t = loadQuaternion(L, 1);
  Quaternion u = loadQuaternion(L, 2);

  t = t * u;

  returnQuaternion(L, t);
  return 1;
}

#if DOXYGEN_ONLY
/**
 * Calculate scalar value between two vectors.
 * @param Vector
 * @param Vector
 * @return float
 */
float copy(Vector, Vector);
#endif
static int copy(lua_State *L) {
  Quaternion t = loadQuaternion(L, 1);
  returnQuaternion(L, t);
  return 1;
}

#if DOXYGEN_ONLY
/**
 * Calculate scalar value between two vectors.
 * @param Vector
 * @param Vector
 * @return float
 */
float from_axis(Vector, Vector);
#endif
static int from_axis(lua_State *L) {
  int index = 1;
  Vector t = loadVector(L, index);
  float f  = lua_tonumber(L, index);

  Quaternion qt;
  qt.buildFromAxis(t, f);

  returnQuaternion(L, qt);
  return 1;
}

#if DOXYGEN_ONLY
/**
 * Calculate scalar value between two vectors.
 * @param Vector
 * @param Vector
 * @return float
 */
float from_euler(Vector, Vector);
#endif
static int from_euler(lua_State *L) {
  int index = 1;
  Vector t = loadVector(L, index);

  Quaternion qt;
  qt.buildFromEuler(t);

  returnQuaternion(L, qt);
  return 1;
}

#if DOXYGEN_ONLY
/**
 * Calculate scalar value between two vectors.
 * @param Vector
 * @param Vector
 * @return float
 */
float to_euler(Vector, Vector);
#endif
static int to_euler(lua_State *L) {
  Quaternion t = loadQuaternion(L, 1);
  
  Vector v; 
  t.getEulerAngles(v);

  returnVector(L, v);
  return 1;
}

static const luaL_reg krigRotationLib[] = {
  {"add", add},
  {"copy", copy},
  {"from_axis", from_axis},
  {"from_euler", from_euler},
  {"to_euler", to_euler},
  {NULL, NULL}
};

int luaopen_krigRotation (lua_State *L) {
  luaL_openlib(L, "krig.rotation", krigRotationLib, 0);
  return 1;
}
