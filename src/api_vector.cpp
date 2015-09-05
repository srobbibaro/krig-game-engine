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
float get_scalar(Vector, Vector);
#endif
static int get_scalar(lua_State *L) {
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

static const luaL_reg krigVectorLib[] = {
  {"get_scalar", get_scalar},
  {"normalize", normalize},
  {"dot_product", dot_product},
  {"cross_product", cross_product},
  {NULL, NULL}
};

int luaopen_krigVector (lua_State *L) {
  luaL_openlib(L, "krig.vector", krigVectorLib, 0);
  return 1;
}
