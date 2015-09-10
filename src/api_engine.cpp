/**
 * @file api_engine.cpp
 * @brief Krig Game Engine Lua Scripting API - Game (Engine)
 *
 * Game (engine) focused API functionality.
 */
#include "api.h"
#include "api_engine.h"
#include "api_object.h"

#if DOXYGEN_ONLY
/**
 * Initiate the Game/Engine shutdown process. This command will kick off clean-up
 * operations and will end with terminating the game.
 * @return n/a
 */
void shutdown();
#endif
static int shutdown(lua_State *L) {
  g_KRIG_ENGINE->shutdown();
  return 0;
}

#if DOXYGEN_ONLY
/**
 * Query the engine for the last calculated value of frames per second (FPS).
 * Frames per second is calculated each iteration of the game loop.
 * @return float frames per second
 */
float get_fps();
#endif
static int get_fps(lua_State *L) { 
  lua_pushnumber(L, g_KRIG_ENGINE->getFps());
  return 1;
}

#if DOXYGEN_ONLY
/**
 * Display 2D text to the screen.
 * @param string text.
 * @param float x coordinate.
 * @param float y coordinate.
 * @return n/a
 */
void render_text(string, float, float);
#endif
static int render_text(lua_State *L) {
  const char *s = lua_tostring(L, 1);
  float x = lua_tonumber(L, 2);
  float y = lua_tonumber(L, 3);

  g_KRIG_ENGINE->renderText(s, x, y);

  return 0;
}

#if DOXYGEN_ONLY
/**
 * Display 3D text to the screen.
 * @param string text.
 * @param float x coordinate.
 * @param float y coordinate.
 * @param float z coordinate.
 * @param float scale x.
 * @param float scale y.
 * @return n/a
 */
void display_text(string, float, float, float, float, float);
#endif
static int display_text(lua_State *L) {
  const char *s = lua_tostring(L, 1);
  char * t = (char*)s;
  float x = lua_tonumber(L, 2);
  float y = lua_tonumber(L, 3);
  float z = lua_tonumber(L, 4);
  float sx = lua_tonumber(L, 5);
  float sy = lua_tonumber(L, 6);

  displayText(t, x, y, z, sx, sy);

  return 0;
}

#if DOXYGEN_ONLY
/**
 * Query the engine to determine if the specified key has been pressed this game cycle.
 * @param int - integer representation of the key
 * @return bool
 */
bool test_key_pressed(int);
#endif
static int test_key_pressed(lua_State *L) {
  int key = (int)lua_tonumber(L, 1);
  int result = (int)(g_KRIG_ENGINE->getKeyState()->testKeyPressed(key));
  lua_pushnumber(L, result);

  return 1;
}

#if DOXYGEN_ONLY
/**
 * Query the engine to determine if the specified key has been released this game cycle.
 * @param int - integer representation of the key
 * @return bool
 */
bool test_key_released(int);
#endif
static int test_key_released(lua_State *L) {
  int key = (int)lua_tonumber(L, 1);
  int result = (int)(g_KRIG_ENGINE->getKeyState()->testKeyReleased(key));
  lua_pushnumber(L, result);
  return 1;
}

#if DOXYGEN_ONLY
/**
 * Query the engine to determine if the specified key has been pressed this game cycle.
 * @param int - integer representation of the special key
 * @return bool
 */
bool test_special_key_pressed(int);
#endif
static int test_special_key_pressed(lua_State *L) {
  int key = (int)lua_tonumber(L, 1);
  int result = (int)(g_KRIG_ENGINE->getSpecialKeyState()->testKeyPressed(key));
  lua_pushnumber(L, result);
  return 1;
}

#if DOXYGEN_ONLY
/**
 * Query the engine to determine if the specified key has been released this game cycle.
 * @param int - integer representation of the special key
 * @return bool
 */
bool test_special_key_release(int);
#endif
static int test_special_key_released(lua_State *L) {
  int key = (int)lua_tonumber(L, 1);
  int result = (int)(g_KRIG_ENGINE->getSpecialKeyState()->testKeyReleased(key));
  lua_pushnumber(L, result);
  return 1;
}

#if DOXYGEN_ONLY
/**
 * Query the engine for the state of the compiler's DEBUG flag.
 * @return bool
 */
bool test_debug_enabled();
#endif
static int test_debug_enabled(lua_State *L) {
  lua_pushnumber(L, DEBUG);
  return 1;
}

#if DOXYGEN_ONLY
/**
 * Query the engine to determine the mouse's last known coordinates.
 * @return Vertex2 - x, y coordinate of the mouse.
 */
Vertex2 get_mouse_coordinates();
#endif
static int get_mouse_coordinates(lua_State *L) {
  float x = g_KRIG_ENGINE->getMouseX();
  float y = g_KRIG_ENGINE->getMouseY();

  lua_newtable(L);

  lua_pushnumber(L, 1);
  lua_pushnumber(L, x);
  lua_rawset(L, -3);

  lua_pushnumber(L, 2);
  lua_pushnumber(L, y);
  lua_rawset(L, -3);
  return 1;
}

#if DOXYGEN_ONLY
/**
 * Play the sound in the specified wav file.
 * @param GameObjectReference
 * @param string - path to wav file to play.
 * @return n/a
 */
void play_sound(GameObjectReference, string);
#endif
static int play_sound(lua_State *L) {
  luaL_checknumber(L, 1);
  Object *object = static_cast<Object*>(loadObject(L, 1));

  const char *s = lua_tostring(L, 2);
  string sound = string(s);

  SoundFX *soundFx = g_KRIG_ENGINE->getSoundFxClass();

  if (soundFx != NULL)
    soundFx->PlaySFX(sound);

  return 0;
}

#if DOXYGEN_ONLY
/**
 * Get a game object reference to the active camera object.
 * @return GameObjectReference
 */
GameObjectReference get_camera();
#endif
static int get_camera(lua_State *L) {
  returnObject(L, g_KRIG_ENGINE->getCurrentLevel()->getCamera());
  return 1;
}

#if DOXYGEN_ONLY
/**
 * Get a game object reference to the player object.
 * @return GameObjectReference
 */
GameObjectReference get_player();
#endif
static int get_player(lua_State *L) {
  returnObject(L, g_KRIG_ENGINE->getCurrentLevel()->getPlayer());
  return 1;
}

#if DOXYGEN_ONLY
/**
 * Fetch variable value from specified game object's script.
 * @param GameObjectReference
 * @return string - variable name.
 */
float get_script_value(GameObjectReference, string);
#endif
static int get_script_value(lua_State *L) {
  luaL_checknumber(L, 1);
  Object *object = static_cast<Object*>(loadObject(L, 1));
  const char *s = lua_tostring(L, 2);

  lua_pushnumber(L, object->getScriptValue(s));
  return 1;
}

#if DOXYGEN_ONLY
/**
 * Set variable value in specified game object's script.
 * @param GameObjectReference
 * @param float value.
 * @return string - variable name.
 */
void set_script_value(GameObjectReference, string, float);
#endif
static int set_script_value(lua_State *L) {
  luaL_checknumber(L, 1);
  Object *object = static_cast<Object*>(loadObject(L, 1));
  const char *s = lua_tostring(L, 2);
  float value = (int)lua_tonumber(L, 3);

  object->setScriptValue(s, value);
  return 0;
}

static const luaL_reg krigEngineLib[] = {
  {"shutdown", shutdown},
  {"get_fps", get_fps},
  {"render_text", render_text},
  {"display_text", display_text},
  {"test_key_pressed", test_key_pressed},
  {"test_key_released", test_key_released},
  {"test_special_key_pressed", test_special_key_pressed},
  {"test_special_key_released", test_special_key_released},
  {"test_debug_enabled", test_debug_enabled},
  {"get_mouse_coordinates", get_mouse_coordinates},
  {"play_sound", play_sound},
  {"get_camera", get_camera},
  {"get_player", get_player},
  {"get_script_value", get_script_value},
  {"set_script_value", set_script_value},
  {NULL, NULL}
};

int luaopen_krigEngine (lua_State *L) {
  luaL_openlib(L, "krig", krigEngineLib, 0);
  return 1;
}
