#ifndef _API_OBJECT_H_
#define _API_OBJECT_H_

extern "C" {
  #include "lua5.1/lua.h"
  #include "lua5.1/lualib.h"
  #include "lua5.1/lauxlib.h"
}

int luaopen_krigObject (lua_State *L);

extern const luaL_reg krigObjectLib[];

#endif
