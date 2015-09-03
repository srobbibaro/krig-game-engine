#include "catch.hpp"
#include "Vector.h"
#include "api_vector.h"
#include <cstring>

static Vector loadVector(lua_State *L, int index) {
  Vector t;

  // x
  lua_pushnumber(L, 1);
  lua_gettable(L, index - 1);
  t.x = (float)lua_tonumber(L, -1);
  lua_pop(L, 1);

  // y
  lua_pushnumber(L, 2);
  lua_gettable(L, index - 1);
  t.y = (float)lua_tonumber(L, -1);
  lua_pop(L, 1);

  // z
  lua_pushnumber(L, 3);
  lua_gettable(L, index - 1);
  t.z = (float)lua_tonumber(L, -1);
  lua_pop(L, 1);

  return t;
}

SCENARIO( "Using Lua API", "[API]" ) {
  GIVEN( "A Lua state with the API loaded" ) {
    lua_State *L = lua_open();
    REQUIRE(L != NULL);

    luaL_openlibs(L);
    luaopen_krigVector(L);

    WHEN( "vector_normalize is called" ) {
      const char* buffer = "v = krig.vector.normalize({1,2,3})\n";
      luaL_loadbuffer(L, buffer, strlen(buffer), "tests") ||
      lua_pcall(L, 0, 0, 0);
      THEN( "It returns a normalized vector" ) {
        lua_getglobal(L, "v");
        REQUIRE(lua_istable(L, -1));
        Vector v = loadVector(L, -1);
        REQUIRE(v.x == Approx(0.26726f));
        REQUIRE(v.y == Approx(0.53452f));
        REQUIRE(v.z == Approx(0.80178f));
      }
    }

    WHEN( "vector_get_scalar is called" ) {
      const char* buffer = "v = krig.vector.get_scalar({1,2,3}, {4,5,6})\n";
      luaL_loadbuffer(L, buffer, strlen(buffer), "tests") ||
      lua_pcall(L, 0, 0, 0);
      THEN( "It returns scalar value between two vectors" ) {
        lua_getglobal(L, "v");
        REQUIRE(lua_isnumber(L, -1));
        REQUIRE((float)lua_tonumber(L, -1) == Approx(32.0f));
      }
    }

    WHEN( "vector_dot_product is called" ) {
      const char* buffer = "v = krig.vector.dot_product({1,2,3}, {4,5,6})\n";
      luaL_loadbuffer(L, buffer, strlen(buffer), "tests") ||
      lua_pcall(L, 0, 0, 0);
      THEN( "It returns dot product for the two vectors" ) {
        lua_getglobal(L, "v");
        REQUIRE(lua_isnumber(L, -1));
        REQUIRE((float)lua_tonumber(L, -1) == Approx(32.0f));
      }
    }

    WHEN( "vector_cross_product is called" ) {
      const char* buffer = "v = krig.vector.cross_product({1,2,3}, {4,5,6})\n";
      luaL_loadbuffer(L, buffer, strlen(buffer), "tests") ||
      lua_pcall(L, 0, 0, 0);
      THEN( "It returns cross product for the two vectors" ) {
        lua_getglobal(L, "v");
        REQUIRE(lua_istable(L, -1));
        Vector v = loadVector(L, -1);
        REQUIRE(v.x == Approx(-3.0f));
        REQUIRE(v.y == Approx(6.0f));
        REQUIRE(v.z == Approx(-3.0f));
      }
    }
  }
}
