/**
 * @file api_vector.cpp
 * @brief Krig Game Engine Lua Scripting API - Rotation Library.
 *
 * Quaternion-based (4-D vector) rotation API functionality.
 */
#include "api.h"
#include "api_rotation.h"

#if DOXYGEN_ONLY
/**
 * Add two rotations represented as quaternions.
 * @param Vector4
 * @param Vector4
 * @return Vector4
 */
Vector4 add(Vector4, Vector4);
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
 * Copy a quaternion.
 * @param Vector4
 * @return Vector4
 */
Vector4 copy(Vector4);
#endif
static int copy(lua_State *L) {
  Quaternion t = loadQuaternion(L, 1);
  returnQuaternion(L, t);
  return 1;
}

#if DOXYGEN_ONLY
/**
 * Convert rotation represented as an axis and an angle into a quaternion.
 * @param Vector3
 * @param float
 * @return Vector4
 */
Vector4 from_axis(Vector3, float);
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
 * Convert rotation represented as euler angles into a quaternion.
 * @param Vector3
 * @return Vector4
 */
Vector4 from_euler(Vector3);
#endif
static int from_euler(lua_State *L) {
  int index = 1;
  Vector t  = loadVector(L, index);

  Quaternion qt;
  qt.buildFromEuler(t);

  returnQuaternion(L, qt);
  return 1;
}

#if DOXYGEN_ONLY
/**
 * Convert quaternion to euler angles.
 * @param Vector4
 * @return Vector3
 */
Vector3 to_euler(Vector4);
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
