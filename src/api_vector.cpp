/**
 * @file api_vector.cpp
 * @brief Krig Game Engine Lua Scripting API - Vector
 *
 * Vector focused API functionality.
 */
#include "api.h"
#include "api_vector.h"

#if DOXYGEN_ONLY
/**
 * Calculate scalar value between two vectors.
 * @param Vector
 * @param Vector
 * @return float
 */
float scalar(Vector, Vector);
#endif
static int scalar(lua_State *L) {
  int index = 1;
  Vector t = loadVector(L, index);
  Vector u = loadVector(L, index);
  lua_pushnumber(L, t.getScaler(u));
  return 1;
}

#if DOXYGEN_ONLY
/**
 * Normalize vector.
 * @param Vector
 * @return Vector
 */
Vector normalize(Vector);
#endif
static int normalize(lua_State *L) {
  int index = 1;
  Vector t = loadVector(L, index);
  t.normalize();
  returnVector(L, t);
  return 1;
}

#if DOXYGEN_ONLY
/**
 * Calculate dot product between two vectors.
 * @param Vector
 * @param Vector
 * @return float - dot product.
 */
float dot_product(Vector, Vector);
#endif
static int dot_product(lua_State *L) {
  int index = 1;
  Vector t = loadVector(L, index);
  Vector u = loadVector(L, index);
  float angle = t.dotProduct(u);
  lua_pushnumber(L, angle);
  return 1;
}

#if DOXYGEN_ONLY
/**
 * Calculate cross product between two vectors.
 * @param Vector
 * @param Vector
 * @return Vector
 */
Vector cross_product(Vector, Vector);
#endif
static int cross_product(lua_State *L) {
  int index = 1;
  Vector t = loadVector(L, index);
  Vector u = loadVector(L, index);
  Vector result;
  result.crossProduct(t, u);
  returnVector(L, result);
  return 1;
}

#if DOXYGEN_ONLY
/**
 * Calculate cross product between two vectors.
 * @param Vector
 * @param Vector
 * @return Vector
 */
Vector average(Vector, Vector);
#endif
static int average(lua_State *L) {
  int index = 1;
  Vector t = loadVector(L, index);
  Vector u = loadVector(L, index);
  Vector result;
  result.average(t, u);
  returnVector(L, result);
  return 1;
}

#if DOXYGEN_ONLY
/**
 * Calculate cross product between two vectors.
 * @param Vector
 * @param Vector
 * @return Vector
 */
Vector add(Vector, Vector);
#endif
static int add(lua_State *L) {
  int index = 1;
  Vector t = loadVector(L, index);
  Vector u = loadVector(L, index);
  t += u;
  returnVector(L, t);
  return 1;
}

#if DOXYGEN_ONLY
/**
 * Calculate cross product between two vectors.
 * @param Vector
 * @param Vector
 * @return Vector
 */
Vector subtract(Vector, Vector);
#endif
static int subtract(lua_State *L) {
  int index = 1;
  Vector t = loadVector(L, index);
  Vector u = loadVector(L, index);
  t -= u;
  returnVector(L, t);
  return 1;
}

#if DOXYGEN_ONLY
/**
 * Calculate cross product between two vectors.
 * @param Vector
 * @param Vector
 * @return Vector
 */
Vector scale(Vector, float);
#endif
static int scale(lua_State *L) {
  int index = 1;
  Vector t = loadVector(L, index);
  int f = (int)lua_tonumber(L,index);
  t = t * f;
  returnVector(L, t);
  return 1;
}

#if DOXYGEN_ONLY
/**
 * Calculate cross product between two vectors.
 * @param Vector
 * @param Vector
 * @return Vector
 */
Vector copy(Vector);
#endif
static int copy(lua_State *L) {
  int index = 1;
  Vector t = loadVector(L, index);
  returnVector(L, t);
  return 1;
}

#if DOXYGEN_ONLY
/**
 * Calculate cross product between two vectors.
 * @param Vector
 * @param Vector
 * @return Vector
 */
float distance(Vector, Vector);
#endif
static int distance(lua_State *L) {
  int index = 1;
  Vector t = loadVector(L, index);
  Vector u = loadVector(L, index);
  float distance = t.getDistance(u);
  lua_pushnumber(L, distance);
  return 1;
}

#if DOXYGEN_ONLY
/**
 * Calculate scalar value between two vectors.
 * @param Vector
 * @param Vector
 * @return float
 */
float sum(Vector, Vector);
#endif
static int sum(lua_State *L) {
  int index = 1;
  Vector t = loadVector(L, index);
  lua_pushnumber(L, t.getSum());
  return 1;
}

static const luaL_reg krigVectorLib[] = {
  {"add", add},
  {"average", average},
  {"copy", copy},
  {"cross_product", cross_product},
  {"distance", distance},
  {"dot_product", dot_product},
  {"normalize", normalize},
  {"scalar", scalar},
  {"scale", scale},
  {"subtract", subtract},
  {"sum", sum},
  {NULL, NULL}
};

int luaopen_krigVector (lua_State *L) {
  luaL_openlib(L, "krig.vector", krigVectorLib, 0);
  return 1;
}
