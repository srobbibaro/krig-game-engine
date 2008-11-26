#ifndef _SCRIPTING_H_
#define _SCRIPTING_H_

extern "C" {
    #include "lua/lua.h"
    #include "lua/lualib.h"
    #include "lua/lauxlib.h"
}

class Scripting
{
    private:
        lua_State* L;

     public:
        Scripting() {}
        ~Scripting( void ) {}
};
       
#endif
