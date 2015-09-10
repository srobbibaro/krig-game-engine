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
 * Add to the camera's rotation.
 * @param GameObjectReference
 * @param float x rotation component.
 * @param float y rotation component.
 * @param float z rotation component.
 * @return n/a
 */
void add_rotation(CameraObjectReference, float, float, float);
#endif
static int add_rotation(lua_State *L) {
  luaL_checktype(L, 1, LUA_TTABLE);
  Camera *object = static_cast<Camera*>(loadObject(L, 1));

  Quaternion qt;
  qt.buildFromEuler(lua_tonumber(L, 2),lua_tonumber(L, 3),lua_tonumber(L, 4));
  object->worldRotation = object->worldRotation * qt;

  return 0;
}

#if DOXYGEN_ONLY
/**
 * Return the definition of he specified plane, a list containing A, B, C and D, where: Ax + By + Cz + D = 0.
 * @param GameObjectReference
 * @param int plane number (0-6), where: 0 - left, 1 - right, 2 - down, 3 - up, 4 - far, and 5 - near.
 * @return array - definition of plane.
 */
array get_frustum_plane(int);
#endif
static int get_frustum_plane(lua_State *L) {
  int plane_num = lua_tonumber(L,1);
  float a, b, c, d;

  Camera *camera = static_cast<Camera*>(g_KRIG_ENGINE->getCurrentLevel()->getCamera());
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

static const luaL_reg krigCameraLib[] = {
  {"add_rotation", add_rotation},
  {"get_frustum_plane", get_frustum_plane},
  {NULL, NULL}
};

int luaopen_krigCamera (lua_State *L) {
  luaL_openlib(L, "krig.camera", krigCameraLib, 0);
  return 1;
}
