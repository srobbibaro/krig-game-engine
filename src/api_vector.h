#ifndef _API_VECTOR_H_
#define _API_VECTOR_H_

extern "C" {
  #include "lua5.1/lua.h"
  #include "lua5.1/lualib.h"
  #include "lua5.1/lauxlib.h"
}

int luaopen_krigVector (lua_State *L);

#endif
