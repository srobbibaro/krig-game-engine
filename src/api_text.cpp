/**
 * @file api_text.cpp
 * @brief Krig Game Engine Lua Scripting API - Text
 *
 * Text object focused API functionality.
 */
#include "api.h"
#include "api_text.h"

#if DOXYGEN_ONLY
/**
 * Fade text between fully seen and unseen incrementally each game cycle by
 * giving it a fade rate. This method sets the specified text object's fade rate.
 * @param TextObjectReference
 * @param float fade rate - can be positive or negative (numbers approaching
 *        zero fade more slowly than numbes with a higher magnitude).
 * @return n/a
 */
void set_fade_rate(TextObjectReference, float);
#endif
static int set_fade_rate(lua_State *L) {
  luaL_checknumber(L, 1);
  ScriptTextType *object = static_cast<ScriptTextType*>(loadObject(L, 1));
  object->fadeRate = lua_tonumber(L, 2);
  return 0;
}

#if DOXYGEN_ONLY
/**
 * Fade text between fully seen and unseen incrementally each game cycle by
 * giving it a fade rate. This method returns the specified text object's fade rate.
 * @param TextObjectReference
 * @return float fade rate
 */
float get_fade_rate(TextObjectReference);
#endif
static int get_fade_rate(lua_State *L) {
  luaL_checknumber(L, 1);
  ScriptTextType *object = static_cast<ScriptTextType*>(loadObject(L, 1));
  lua_pushnumber(L, object->fadeRate);
  return 1;
}

#if DOXYGEN_ONLY
/**
 * The alpha setting controls the degree of transparency for the text object.
 * It can be not transparent (1.0), partially transparent (< 1.0 - > 0.0),
 * or fully transparent (0.0). This method sets the alpha value for the specified
 * text object.
 * @param TextObjectReference
 * @param float alpha
 * @return n/a
 */
void set_alpha(TextObjectReference, float);
#endif
static int set_alpha(lua_State *L) {
  luaL_checknumber(L, 1);
  ScriptTextType *object = static_cast<ScriptTextType*>(loadObject(L, 1));
  object->color[3] = lua_tonumber(L, 2);
  return 0;
}

#if DOXYGEN_ONLY
/**
 * The alpha setting controls the degree of transparency for the text object.
 * It can be not transparent (1.0), partially transparent (< 1.0 - > 0.0),
 * or fully transparent (0.0). This method returns the alpha value for the specified
 * text object.
 * @param TextObjectReference
 * @return float alpha
 */
float get_alpha(TextObjectReference);
#endif
static int get_alpha(lua_State *L) {
  luaL_checknumber(L, 1);
  ScriptTextType *object = static_cast<ScriptTextType*>(loadObject(L, 1));
  lua_pushnumber(L, object->color[3]);
  return 1;
}

#if DOXYGEN_ONLY
/**
 * Set the text's color.
 * @param TextObjectReference
 * @param float red component (0.0 - 1.0).
 * @param float green component (0.0 - 1.0).
 * @param float blue component (0.0 - 1.0).
 * @return n/a
 */
void set_color(TextObjectReference, float, float, float);
#endif
static int set_color(lua_State *L) {
  luaL_checknumber(L, 1);
  ScriptTextType *object = static_cast<ScriptTextType*>(loadObject(L, 1));
  object->color[0] = lua_tonumber(L, 2);
  object->color[1] = lua_tonumber(L, 3);
  object->color[2] = lua_tonumber(L, 4);
  return 0;
}

#if DOXYGEN_ONLY
/**
 * Set the text's color.
 * @param TextObjectReference
 * @param Vector array containing red, green, and blue components (0.0 - 1.0).
 * @return n/a
 */
void set_colorv(TextObjectReference, Vector);
#endif
static int set_colorv(lua_State *L) {
  luaL_checknumber(L, 1);
  ScriptTextType *object = static_cast<ScriptTextType*>(loadObject(L, 1));
  Vector t = loadVector(L);
  object->color[0] = t.x;
  object->color[1] = t.y;
  object->color[2] = t.z;
  return 0;
}

#if DOXYGEN_ONLY
/**
 * Get the text's color.
 * @param TextObjectReference
 * @return Vector array containing red, green, and blue components (0.0 - 1.0).
 */
Vector get_color(TextObjectReference);
#endif
static int get_color(lua_State *L) {
  luaL_checknumber(L, 1);
  ScriptTextType *object = static_cast<ScriptTextType*>(loadObject(L, 1));
  Vector t; t.setVector(object->color[0], object->color[1], object->color[2]);
  returnVector(L, t);
  return 1;
}

#if DOXYGEN_ONLY
/**
 * Set text's width.
 * @param TextObjectReference
 * @param float width of text.
 * @return n/a
 */
void set_width(TextObjectReference, float);
#endif
static int set_width(lua_State *L) {
  luaL_checknumber(L, 1);
  ScriptTextType *object = static_cast<ScriptTextType*>(loadObject(L, 1));
  object->width = lua_tonumber(L, 2);
  return 0;
}

#if DOXYGEN_ONLY
/**
 * Get text's width.
 * @param TextObjectReference
 * @return float width of text.
 */
float get_width(TextObjectReference);
#endif
static int get_width(lua_State *L) {
  luaL_checknumber(L, 1);
  ScriptTextType *object = static_cast<ScriptTextType*>(loadObject(L, 1));
  lua_pushnumber(L, object->width);
  return 1;
}

static const luaL_reg krigTextLib[] = {
  {"get_fade_rate", get_fade_rate},
  {"set_fade_rate", set_fade_rate},
  {"get_alpha", get_alpha},
  {"set_alpha", set_alpha},
  {"get_color", get_color},
  {"set_color", set_color},
  {"get_width", get_width},
  {"set_width", set_width},
  {NULL, NULL}
};

int luaopen_krigText (lua_State *L) {
  luaL_openlib(L, "krig.text", krigTextLib, 0);
  return 1;
}
