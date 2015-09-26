/**
 * @file api_camera.cpp
 * @brief Krig Game Engine Lua Scripting API - Camera
 *
 * Camera focused API functionality.
 */
#include "api.h"
#include "api_camera.h"

#if DOXYGEN_ONLY
/**
 * Return the definition of the specified plane as a list containing:
 * A, B, C and D, where: Ax + By + Cz + D = 0.
 * @param int plane number (0-6), where: 0 - left, 1 - right, 2 - down, 3 - up, 4 - far, and 5 - near.
 * @return Vector4 - definition of plane.
 */
Vector4 get_frustum_plane(int);
#endif
static int get_frustum_plane(lua_State *L) {
  luaL_checktype(L, 1, LUA_TTABLE);
  Camera *camera = static_cast<Camera*>(loadObject(L, 1));

  int plane_num = lua_tonumber(L, 2);
  float a, b, c, d;

  camera->getFrustum()->getPlaneDefinition(plane_num, a, b, c, d);

  lua_newtable(L);

  lua_pushnumber(L, 1);
  lua_pushnumber(L, a);
  lua_rawset(L, -3);

  lua_pushnumber(L, 2);
  lua_pushnumber(L, b);
  lua_rawset(L, -3);

  lua_pushnumber(L, 3);
  lua_pushnumber(L, c);
  lua_rawset(L, -3);

  lua_pushnumber(L, 4);
  lua_pushnumber(L, d);
  lua_rawset(L, -3);

  return 1;
}

const luaL_reg krigCameraLib[] = {
  {"get_frustum_plane", get_frustum_plane},
  {NULL, NULL}
};
