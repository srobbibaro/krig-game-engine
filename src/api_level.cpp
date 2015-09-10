/**
 * @file api_level.cpp
 * @brief Krig Game Engine Lua Scripting API - Game Level
 *
 * Game level focused API functionality.
 */
#include "api.h"
#include "api_level.h"

#if DOXYGEN_ONLY
/**
 * Load the game level Lua script from the file specified.
 * @param string name of the game level Lua script to load.
 * @return n/a
 */
void load(string);
#endif
static int load(lua_State *L) {
  const char *s = lua_tostring(L, 1);
  g_KRIG_ENGINE->loadLevel(s);

  return 0;
}

#if DOXYGEN_ONLY
/**
 * Load the game level Lua script from the supplied string buffer.
 * @param string buffer containing a game level Lua script to load.
 * @return n/a
 */
void load_from_buffer(string);
#endif
static int load_from_buffer(lua_State *L) {
  const char *s = lua_tostring(L, 1);
  g_KRIG_ENGINE->loadLevelFromBuffer(s);

  return 0;
}

#if DOXYGEN_ONLY
/**
 * Pause execution of the game
 * @return n/a
 */
void pause();
#endif
static int pause(lua_State *L) {
  g_KRIG_ENGINE->pause();

  return 0;
}

#if DOXYGEN_ONLY
/**
 * Query the engine to determine the ID of the present level.
 * @return int the ID associated with the present game level.
 */
int get_id();
#endif
static int get_id(lua_State *L) {
  lua_pushnumber(L, g_KRIG_ENGINE->getCurrentLevel()->getId());
  return 1;
}

#if DOXYGEN_ONLY
/**
 * Set the ID associated with the present level.
 * @param level_id
 * @return n/a
 */
int set_id(int);
#endif
static int set_id(lua_State *L) {
  g_KRIG_ENGINE->getCurrentLevel()->setId((int)lua_tonumber(L, 1));
  return 0;
}

#if DOXYGEN_ONLY
/**
 * TODO: Undefined
 * @return n/a
 */
void swap();
#endif
static int swap(lua_State *L) {
  g_KRIG_ENGINE->swapLevel();
  return 0;
}

#if DOXYGEN_ONLY
/**
 * Query the engine to determine if the present level has been marked complete.
 * @return bool true if the level has been marked complete, false otherwise
 */
bool get_complete();
#endif
static int get_complete(lua_State *L) {
  lua_pushnumber(L, g_KRIG_ENGINE->getCurrentLevel()->checkComplete());
  return 1;
}

#if DOXYGEN_ONLY
/**
 * The currently running game level maintains a state about whether it has been
 * marked as "complete" or not. This method sets this state.
 * @param bool true - level is marked as complete, false - level is not complete
 * @return n/a
 */
void set_complete(bool);
#endif
static int set_complete(lua_State *L) {
  g_KRIG_ENGINE->getCurrentLevel()->setComplete(lua_tonumber(L, 1));
  return 0;
}

#if DOXYGEN_ONLY
/**
 * Playing song specified.
 * @param string name of file containing the song to play.
 * @param int repeat control (0 - do not repeat, 1 - repeat).
 * @return n/a
 */
void play_music(string, int);
#endif
static int play_music(lua_State *L) {
  const char *s = lua_tostring(L, 1);
  int repeat = (int)lua_tonumber(L, 2);
  g_KRIG_ENGINE->getCurrentLevel()->setMusicPath(string(s));
  g_KRIG_ENGINE->getCurrentLevel()->getMusic()->StopSong();
  g_KRIG_ENGINE->getCurrentLevel()->getMusic()->PlaySong(s, repeat);
  return 0;
}

#if DOXYGEN_ONLY
/**
 * Stop playing song (if playing). Song will restart when played.
 * @return n/a
 */
void stop_music();
#endif
static int stop_music(lua_State *L) {
  g_KRIG_ENGINE->getCurrentLevel()->getMusic()->StopSong();
  return 0;
}

#if DOXYGEN_ONLY
/**
 * Pause song (if playing). Song will resume where it was paused when played.
 * @return n/a
 */
void pause_music();
#endif
static int pause_music(lua_State *L) {
  g_KRIG_ENGINE->getCurrentLevel()->getMusic()->PauseSong();
  return 0;
}

#if DOXYGEN_ONLY
/**
 * Render a sky box around the level using the three colors supplied.
 * Color 1 is the top color; Color two is in the middle; Color 3 is the bottom color.
 * @param float first color, red attribute (0.0 - 1.0)
 * @param float first color, green attribute (0.0 - 1.0)
 * @param float first color, blue attribute (0.0 - 1.0)
 * @param float second color, red attribute (0.0 - 1.0)
 * @param float second color, green attribute (0.0 - 1.0)
 * @param float second color, blue attribute (0.0 - 1.0)
 * @param float third color, red attribute (0.0 - 1.0)
 * @param float third color, green attribute (0.0 - 1.0)
 * @param float third color, blue attribute (0.0 - 1.0)
 * @return n/a
 */
void set_sky_box(float, float, float, float, float, float, float, float, float);
#endif
static int set_sky_box(lua_State *L) {
  int num = 1;

  float skyColors[3][3];
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      skyColors[i][j] = lua_tonumber(L, num++);
    }
  }

  g_KRIG_ENGINE->getCurrentLevel()->setSkyBox(skyColors, 3, 3);
  return 0;
}

#if DOXYGEN_ONLY
/**
 * Fetch the configured light direction.
 * @return Vector
 */
Vector get_light_direction();
#endif
static int get_light_direction(lua_State *L) {
  returnVector(L, *g_KRIG_ENGINE->getCurrentLevel()->getLightDirection());
  return 1;
}

#if DOXYGEN_ONLY
/**
 * Set the light direction in the game level's scene. This setting affects
 * shading on models and the terrain.
 * @param Vector array containing x, y, z coordinates of the directed light vector
 * @return n/a
 */
void set_light_direction(Vector);
#endif
#if DOXYGEN_ONLY
/**
 * Set the light direction in the game level's scene. This setting affects
 * shading on models and the terrain.
 * @param float x coordinate of the directed light vector
 * @param float y coordinate of the directed light vector
 * @param float z coordinate of the directed light vector
 * @return n/a
 */
void set_light_direction(float, float, float);
#endif
static int set_light_direction(lua_State *L) {
  int index = 1;
  Vector t = loadVector(L, index);
  g_KRIG_ENGINE->getCurrentLevel()->setLightDirection(t.x, t.y, t.z);
  return 0;
}

#if DOXYGEN_ONLY
/**
 * Set the specified level's terrain.
 * @param TerrainObjectReference
 * @param string terrain file to load.
 * @return n/a
 */
void set_terrain(TerrainObjectReference, string);
#endif
static int set_terrain(lua_State *L) {
  luaL_checktype(L, 1, LUA_TTABLE);
  Terrain *terrain = static_cast<Terrain*>(loadObject(L, 1));

  const char *s = lua_tostring(L, 2);

  Vector *lightDirection = g_KRIG_ENGINE->getCurrentLevel()->getLightDirection();
  terrain->load(s, lightDirection);
  return 0;
}

#if DOXYGEN_ONLY
/**
 * A game level can use up to 4 cameras, each with its own independent attached
 * script and settings. However, only one camera can be "active" at a time. This
 * method returns the ID of the active camera.
 * @return int ID of the active camera
 */
int get_camera_id();
#endif
static int get_camera_id(lua_State *L) {
  lua_pushnumber(L, g_KRIG_ENGINE->getCurrentLevel()->getCamera()->id_);
  return 1;
}

#if DOXYGEN_ONLY
/**
 * Query the engine for a game object of the specified type.
 * @param int - ID of object type to find.
 * @return Vertex2 - x, y coordinate of the mouse.
 */
GameObjectReference find_object_of_type(int);
#endif
static int find_object_of_type(lua_State *L) {
  int type = (int)lua_tonumber(L, 1);
  ScriptedObject *temp = static_cast<ScriptedObject*>(
    g_KRIG_ENGINE->getCurrentLevel()->findEnemyOfType(type)
  );

  returnObject(L, temp);
  return 1;
}

#if DOXYGEN_ONLY
/**
 * Add a game object to the current level.
 * @param string - path to lua script for new game object to load.
 * @param table - options table to be passed into the "on_load" method for the game object.
 * @return GameObjectReference
 */
GameObjectReference add_object(string, options);
#endif
static int add_object(lua_State *L) {
  const char *s = lua_tostring(L, 1);
  string script = string(s);

  ScriptedObject *temp = static_cast<ScriptedObject*>(
    g_KRIG_ENGINE->getCurrentLevel()->addObject(script, L, TYPE_GAME_OBJECT)
  );

  returnObject(L, temp);
  return 1;
}

#if DOXYGEN_ONLY
/**
 * Remove the specified object from the level.
 * @param GameObjectReference
 * @return n/a
 */
void remove_object(GameObjectReference);
#endif
static int remove_object(lua_State *L) {
  luaL_checktype(L, 1, LUA_TTABLE);
  Object *object = static_cast<Object*>(loadObject(L, 1));

  object->setState(DEAD);
  return 0;
}

#if DOXYGEN_ONLY
/**
 * Add a game object to the current level.
 * @param TerrainReference
 * @param string - path to lua script for new text object to load.
 * @param string - text.
 * @param table - options table to be passed into the "on_load" method for the text object.
 * @return TextObjectReference
 */
TextObjectReference add_text(string, string, options);
#endif
static int add_text(lua_State *L) {
  const char *s = lua_tostring(L, 1);
  string script = string(s);

  const char *t = lua_tostring(L, 2);
  string text   = string(t);

  ScriptTextType *temp = static_cast<ScriptTextType*>(
    g_KRIG_ENGINE->getCurrentLevel()->addObject(script, L, TYPE_GAME_TEXT)
  );
  if (temp != NULL) {
    temp->text = text;
  }

  returnObject(L, temp);
  return 1;
}

static const luaL_reg krigLevelLib[] = {
  {"load", load},
  {"load_from_buffer", load_from_buffer},
  {"pause", pause},
  {"get_id", get_id},
  {"set_id", set_id},
  {"swap", swap},
  {"get_complete", get_complete},
  {"set_complete", set_complete},
  {"play_music", play_music},
  {"stop_music", stop_music},
  {"pause_music", pause_music},
  {"set_sky_box", set_sky_box},
  {"get_light_direction", get_light_direction},
  {"set_light_direction", set_light_direction},
  {"set_terrain", set_terrain},
  {"get_camera_id", get_camera_id},
  {"find_object_of_type", find_object_of_type},
  {"add_object", add_object},
  {"remove_object", remove_object},
  {"add_text", add_text},
  {NULL, NULL}
};

int luaopen_krigLevel (lua_State *L) {
  luaL_openlib(L, "krig.level", krigLevelLib, 0);
  return 1;
}
