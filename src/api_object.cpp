/**
 * @file api_object.cpp
 * @brief Krig Game Engine Lua Scripting API - Game Object
 *
 * Game object API functionality.
 */
#include "api.h"
#include "api_object.h"

#if DOXYGEN_ONLY
/**
 * Set the end rotation (by axis) for the specified game object's interpolation.
 * @param Vector4 - start rotation.
 * @param float - start interpolation value.
 * @param Vector4 - end rotation.
 * @param float - end interpolation value.
 * @return n/a
 */
void setup_interpolation(Vector4, float, Vector4, float);
#endif
static int setup_interpolation(lua_State *L) {
  luaL_checktype(L, 1, LUA_TTABLE);
  Object *object = static_cast<Object*>(loadObject(L, 1));

  Quaternion tq1 = loadQuaternion(L, 2);
  float tv1 = lua_tonumber(L, 3);
  Quaternion tq2 = loadQuaternion(L, 4);
  float tv2 = lua_tonumber(L, 5);

  object->setRInterpStart(tq1);
  object->setRInterpEnd(tq2);
  object->setValInterpBegin(tv1);
  object->setValInterpEnd(tv2);

  return 0;
}

#if DOXYGEN_ONLY
/**
 * Set current value of the interpolation variable.
 * @param float value.
 * @return n/a
 */
void set_interpolation_value(float);
#endif
static int update_interpolation_value(lua_State *L) {
  luaL_checktype(L, 1, LUA_TTABLE);
  Object *object = static_cast<Object*>(loadObject(L, 1));
  object->setValInterpCurrent(lua_tonumber(L, 2));
  return 0;
}

#if DOXYGEN_ONLY
/**
 * Suspend running the game object's script until the suspend time has elapsed.
 * @param float
 * @return n/a
 */
void suspend(float);
#endif
static int suspend(lua_State *L) {
  luaL_checktype(L, 1, LUA_TTABLE);
  Object *object = static_cast<Object*>(loadObject(L, 1));
  object->setSuspendTime(lua_tonumber(L, 2));
  return 0;
}

#if DOXYGEN_ONLY
/**
 * Specify the model to render for the game object.
 * @string file containing the model to load.
 * @return n/a
 */
void set_model(string);
#endif
static int set_model(lua_State *L) {
  luaL_checktype(L, 1, LUA_TTABLE);
  ScriptedObject *object = static_cast<ScriptedObject*>(loadObject(L, 1));

  const char *s = lua_tostring(L, 2);
  string model = string(s);

  object->load(model);
  return 0;
}

#if DOXYGEN_ONLY
/**
 * Specify the texture to render for the sprite.
 * @string file containing the texture to load (in png format).
 * @return n/a
 */
void set_texture(string);
#endif
static int set_texture(lua_State *L) {
  luaL_checktype(L, 1, LUA_TTABLE);
  Sprite *object = static_cast<Sprite*>(loadObject(L, 1));

  const char *s = lua_tostring(L, 2);
  string texture = string(s);

  object->setTexture(texture);
  return 0;
}

#if DOXYGEN_ONLY
/**
 * Set lua script for game object.
 * @param string lua script file name.
 * @return n/a
 */
void set_script(string);
#endif
static int set_script(lua_State *L) {
  luaL_checktype(L, 1, LUA_TTABLE);
  Object *object = static_cast<Object*>(loadObject(L, 1));

  if (object != NULL) {
    const char *s = lua_tostring(L, 2);
    string script = string(s);

    object->unloadScript();
    object->setScript(script);
    object->loadScript(script, L);
  }

  return 0;
}

#if DOXYGEN_ONLY
/**
 * Add a particle system to the game object.
 * @param int - number representing the particle system to load.
 * @return n/a
 */
void add_particle_system(GameObjectReference, int);
#endif
static int add_particle_system(lua_State *L) {
  luaL_checktype(L, 1, LUA_TTABLE);
  Object *object = static_cast<Object*>(loadObject(L, 1));

  object->setParticleSystem((int)lua_tonumber(L, 2));

  return 0;
}

#if DOXYGEN_ONLY
/**
 * Rotate the game object such that it appears to be resting on the terrain.
 * Additionally, the specified rotation will be used as the base rotation of 
 * the game object.
 * @param x rotation around x axis
 * @param y rotation around y axis
 * @param z rotation around z axis
 * @return n/a
 */
void orient_on_terrain(float, float, float);
#endif
static int orient_on_terrain(lua_State *L) {
  luaL_checktype(L, 1, LUA_TTABLE);
  Object *object = loadObject(L, 1);

  Quaternion rotation;
  rotation.buildFromEuler(lua_tonumber(L,2),lua_tonumber(L,3),lua_tonumber(L,4));

  object->orientOnTerrain(g_KRIG_ENGINE->getCurrentLevel()->getTerrain(), rotation);

  return 0;
}

#if DOXYGEN_ONLY
/**
 * Determine the height of the terrain beneath the game object and
 * set its height to match the terrain's height at this location. The offset
 * is used to adjust the object's height along the y axis.
 * @param float - height offset
 * @return n/a
 */
void set_height_from_terrain(GameObjectReference, float);
#endif
static int set_height_from_terrain(lua_State *L) {
  luaL_checktype(L, 1, LUA_TTABLE);
  Model *object = static_cast<Model*>(loadObject(L, 1));

  float offset = lua_tonumber(L, 2);

  object->setHeightFromTerrain(g_KRIG_ENGINE->getCurrentLevel()->getTerrain(), offset);

  return 0;
}

#if DOXYGEN_ONLY
/**
 * Load properties for game object.
 * @return GameObject
 */
GameObject load();
#endif
static int load(lua_State *L) {
  luaL_checktype(L, 1, LUA_TTABLE);
  Object *object = loadObject(L, 1);

  returnObject(L, object);
  object->buildLuaObjectTable(L);

  return 1;
}

#if DOXYGEN_ONLY
/**
 * Save game object properties.
 * @return n/a
 */
void save();
#endif
static int save(lua_State *L) {
  luaL_checktype(L, 1, LUA_TTABLE);
  Object *object = loadObject(L, 1);

  object->transferLuaObjectTable(L);

  return 0;
}

const luaL_reg krigObjectLib[] = {
  {"add_particle_system", add_particle_system},
  {"load", load},
  {"orient_on_terrain", orient_on_terrain},
  {"save", save},
  {"set_height_from_terrain", set_height_from_terrain},
  {"set_model", set_model},
  {"set_texture", set_texture},
  {"set_script", set_script},
  {"setup_interpolation", setup_interpolation},
  {"suspend", suspend},
  {"update_interpolation_value", update_interpolation_value},
  {NULL, NULL}
};
