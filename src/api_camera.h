#ifndef _API_CAMERA_H_
#define _API_CAMERA_H_

extern "C" {
  #include "lua5.1/lua.h"
  #include "lua5.1/lualib.h"
  #include "lua5.1/lauxlib.h"
}

static int get_frustum_plane(lua_State *L);

extern const luaL_reg krigCameraLib[];

#endif
