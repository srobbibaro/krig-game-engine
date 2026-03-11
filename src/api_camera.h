#ifndef _API_CAMERA_H_
#define _API_CAMERA_H_

extern "C" {
  #include "luajit-2.1/lua.h"
  #include "luajit-2.1/lualib.h"
  #include "luajit-2.1/lauxlib.h"
}

static int get_frustum_plane(lua_State *L);

extern const luaL_Reg krigCameraLib[];

#endif
