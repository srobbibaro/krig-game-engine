/**
 * @file Scripting.h
 * @brief Krig Game Engine Lua Scripting API
 *
 * API calls are split into four classes of functions. The classes are defined
 * by the type of script where they are available for use. For example, the
 * playBgMusic function is supported for a game level script and would not be
 * available to use for an object. The four classes of functions are: Game Object,
 * Text, Game Level, and Game (Engine).
 */
#ifndef _SCRIPTING_H_
#define _SCRIPTING_H_

extern "C" {
  #include "lua5.1/lua.h"
  #include "lua5.1/lualib.h"
  #include "lua5.1/lauxlib.h"
}

#include "Object.h"
#include "text.h"
#include "ScriptedObject.h"
#include "GameLevel.h"
#include "matrix.h"
#include "Engine.h"

// Macros /////////////////////////////////////////////////////////////////////
#define SCRIPT_CALLBACK_ON_LOAD        "on_load"
#define SCRIPT_CALLBACK_ON_UPDATE      "on_update"
#define SCRIPT_CALLBACK_ON_DRAW        "on_draw"
#define SCRIPT_CALLBACK_ON_DRAW_SCREEN "on_draw_screen"
#define SCRIPT_CALLBACK_ON_COLLISION   "on_collision"
#define SCRIPT_CALLBACK_ON_UNLOAD      "on_unload"

// Global variables ///////////////////////////////////////////////////////////
extern Object* g_script_player;
extern Object* g_script_camera;
extern GameLevel* g_script_game_level;
extern Engine* g_script_engine;

// Lua API ////////////////////////////////////////////////////////////////////
/** @name Game
 *  Game functions are only available in game scripts.
 */
///@{
#if DOXYGEN_ONLY
/**
 * Load the game level Lua script from the file specified.
 * @param string name of the game level Lua script to load.
 * @return n/a
 */
void loadLevel(string);
#endif
static int loadLevelLua(lua_State *L) {
  const char *s = lua_tostring(L, 1);
  g_script_engine->loadLevel(s);

  return 0;
}

#if DOXYGEN_ONLY
/**
 * Load the game level Lua script from the supplied string buffer.
 * @param string buffer containing a game level Lua script to load.
 * @return n/a
 */
void loadLevelFromBuffer(string);
#endif
static int loadLevelFromBufferLua(lua_State *L) {
  const char *s = lua_tostring(L, 1);
  g_script_engine->loadLevelFromBuffer(s);

  return 0;
}

#if DOXYGEN_ONLY
/**
 * Pause execution of the game
 * @return n/a
 */
void pause();
#endif
static int pauseLua(lua_State *L) {
  g_script_engine->pause();

  return 0;
}

#if DOXYGEN_ONLY
/**
 * TODO: Undefined
 * @return n/a
 */
void swapLevel();
#endif
static int swapLevelLua(lua_State *L) {
  g_script_engine->swapLevel();
  return 0;
}

#if DOXYGEN_ONLY
/**
 * Query the engine to determine if the present level has been marked complete.
 * @return bool true if the level has been marked complete, false otherwise
 */
void testLevelComplete();
#endif
static int testLevelCompleteLua(lua_State *L) {
  lua_pushnumber(L, g_script_game_level->checkComplete());
  return 1;
}

#if DOXYGEN_ONLY
/**
 * Query the engine to determine the ID of the present level.
 * @return int the ID associated with the present game level.
 */
int getLevelId();
#endif
static int getLevelIdLua(lua_State *L) {
  lua_pushnumber(L, g_script_game_level->getId());
  return 1;
}

#if DOXYGEN_ONLY
/**
 * Set the ID associated with the present level.
 * @param level_id
 * @return n/a
 */
int getLevelId();
#endif
static int setLevelIdLua(lua_State *L) {
  g_script_game_level->setId((int)lua_tonumber(L, 1));
  return 0;
}
///@}

/** @name Game Level
 *  Game level functions are only available in game level scripts.
 */
///@{
#if DOXYGEN_ONLY
/**
 * Playing song specified.
 * @param string name of file containing the song to play.
 * @param int repeat control (0 - do not repeat, 1 - repeat).
 * @return n/a
 */
void playBgMusic(string, int);
#endif
static int playBgMusicLua(lua_State *L) {
  const char *s = lua_tostring(L, 1);
  int repeat = (int)lua_tonumber(L, 2);
  g_script_game_level->setMusicPath(string(s));
  g_script_game_level->getMusic()->StopSong();
  g_script_game_level->getMusic()->PlaySong(s, repeat);
  return 0;
}

#if DOXYGEN_ONLY
/**
 * Stop playing song (if playing). Song will restart when played.
 * @return n/a
 */
void stopBgMusic();
#endif
static int stopBgMusicLua(lua_State *L) {
  g_script_game_level->getMusic()->StopSong();
  return 0;
}

#if DOXYGEN_ONLY
/**
 * Pause song (if playing). Song will resume where it was paused when played.
 * @return n/a
 */
void pauseBgMusic();
#endif
static int pauseBgMusicLua(lua_State *L) {
  g_script_game_level->getMusic()->PauseSong();
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
void setSkyBox(float, float, float, float, float, float, float, float, float);
#endif
static int setSkyBoxLua(lua_State *L) {
  int num = 1;

  float skyColors[3][3];
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      skyColors[i][j] = lua_tonumber(L, num++);
    }
  }

  g_script_game_level->setSkyBox(skyColors, 3, 3);
  return 0;
}

#if DOXYGEN_ONLY
/**
 * Set the light direction in the game level's scene. This setting affects
 * shading on models and the terrain.
 * @param float x coordinate of the directed light vector
 * @param float y coordinate of the directed light vector
 * @param float z coordinate of the directed light vector
 * @return n/a
 */
void setLightDirection(float, float, float);
#endif
static int setLightDirectionLua(lua_State *L) {
  g_script_game_level->setLightDirection(lua_tonumber(L, 1), lua_tonumber(L, 2), lua_tonumber(L, 3));
  return 0;
}

#if DOXYGEN_ONLY
/**
 * Set the light direction in the game level's scene. This setting affects
 * shading on models and the terrain.
 * @param Vector array containing x, y, z coordinates of the directed light vector
 * @return n/a
 */
void setLightDirectionv(Vector);
#endif
static int setLightDirectionvLua(lua_State *L) {
  Vector t = loadVector(L);
  g_script_game_level->setLightDirection(t.x, t.y, t.z);
  return 0;
}

#if DOXYGEN_ONLY
/**
 * Fetch the configured light direction.
 * @return Vector
 */
Vector getLightDirection();
#endif
static int getLightDirectionLua(lua_State *L) {
  returnVector(L, *g_script_game_level->getLightDirection());
  return 1;
}

#if DOXYGEN_ONLY
/**
 * Set the specified level's terrain.
 * @param TerrainObjectReference
 * @param string terrain file to load.
 * @return n/a
 */
void setTerrain(TerrainObjectReference, string);
#endif
static int setTerrainLua(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  Terrain *terrain = static_cast<Terrain*>(lua_touserdata(L, 1));

  const char *s = lua_tostring(L, 2);

  Vector *lightDirection = g_script_game_level->getLightDirection();
  terrain->load(s, lightDirection);
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
void displayText(string, float, float, float, float, float);
#endif
static int displayTextLua(lua_State *L) {
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
 * Display 2D text to the screen.
 * @param string text.
 * @param float x coordinate.
 * @param float y coordinate.
 * @return n/a
 */
void renderText(string, float, float);
#endif
static int renderTextLua(lua_State *L) {
  const char *s = lua_tostring(L, 1);
  float x = lua_tonumber(L, 2);
  float y = lua_tonumber(L, 3);

  g_script_engine->renderText(s, x, y);

  return 0;
}

#if DOXYGEN_ONLY
/**
 * Query the engine for the last calculated value of frames per second (FPS).
 * Frames per second is calculated each iteration of the game loop.
 * @return float frames per second
 */
float getFps();
#endif
static int getFpsLua(lua_State *L) {
  lua_pushnumber(L, g_script_engine->getFps());
  return 1;
}

#if DOXYGEN_ONLY
/**
 * A game level can use up to 4 cameras, each with its own independent attached
 * script and settings. However, only one camera can be "active" at a time. This
 * method returns the ID of the active camera.
 * @return int ID of the active camera
 */
int getCameraId();
#endif
static int getCameraIdLua(lua_State *L) {
  lua_pushnumber(L, g_script_game_level->getCamera()->id_);
  return 1;
}

#if DOXYGEN_ONLY
/**
 * The currently running game level maintains a state about whether it has been
 * marked as "complete" or not. This method sets this state.
 * @param bool true - level is marked as complete, false - level is not complete
 * @return n/a
 */
void setComplete(bool);
#endif
static int setCompleteLua(lua_State *L) {
  g_script_game_level->setComplete(lua_tonumber(L, 1));
  return 0;
}

#if DOXYGEN_ONLY
/**
 * Initiate the Game/Engine shutdown process. This command will kick off clean-up
 * operations and will end with terminating the game.
 * @return n/a
 */
void shutdown(bool);
#endif
static int shutdownLua(lua_State *L) {
  g_script_engine->shutdown();
  return 0;
}
///@}

/** @name Text
 *  Text functions are only available for text scripts.
 */
///@{
#if DOXYGEN_ONLY
/**
 * Fade text between fully seen and unseen incrementally each game cycle by
 * giving it a fade rate. This method sets the specified text object's fade rate.
 * @param TextObjectReference
 * @param float fade rate - can be positive or negative (numbers approaching
 *        zero fade more slowly than numbes with a higher magnitude).
 * @return n/a
 */
void setFadeRate(TextObjectReference, float);
#endif
static int setFadeRateLua(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  ScriptTextType *object = static_cast<ScriptTextType*>(lua_touserdata(L, 1));
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
float getFadeRate(TextObjectReference);
#endif
static int getFadeRateLua(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  ScriptTextType *object = static_cast<ScriptTextType*>(lua_touserdata(L, 1));
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
void setAlpha(TextObjectReference, float);
#endif
static int setAlphaLua(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  ScriptTextType *object = static_cast<ScriptTextType*>(lua_touserdata(L, 1));
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
float getAlpha(TextObjectReference);
#endif
static int getAlphaLua(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  ScriptTextType *object = static_cast<ScriptTextType*>(lua_touserdata(L, 1));
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
void setColor(TextObjectReference, float, float, float);
#endif
static int setColorLua(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  ScriptTextType *object = static_cast<ScriptTextType*>(lua_touserdata(L, 1));
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
void setColorv(TextObjectReference, Vector);
#endif
static int setColorvLua(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  ScriptTextType *object = static_cast<ScriptTextType*>(lua_touserdata(L, 1));
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
Vector getColor(TextObjectReference);
#endif
static int getColorLua(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  ScriptTextType *object = static_cast<ScriptTextType*>(lua_touserdata(L, 1));
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
void setWidth(TextObjectReference, float);
#endif
static int setWidthLua(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  ScriptTextType *object = static_cast<ScriptTextType*>(lua_touserdata(L, 1));
  object->width = lua_tonumber(L, 2);
  return 0;
}

#if DOXYGEN_ONLY
/**
 * Get text's width.
 * @param TextObjectReference
 * @return float width of text.
 */
float getWidth(TextObjectReference);
#endif
static int getWidthLua(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  ScriptTextType *object = static_cast<ScriptTextType*>(lua_touserdata(L, 1));
  lua_pushnumber(L, object->width);
  return 1;
}
///@}

/** @name Game Object
 * Game object functions are only available in game object scripts.
 */
///@{

#if DOXYGEN_ONLY
/**
 * Set the specified object's position.
 * @param GameObjectReference
 * @param float x coordinate.
 * @param float y coordinate.
 * @param float z coordinate.
 * @return n/a
 */
void setPosition(GameObjectReference, float, float, float);
#endif
static int setPositionLua(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  Object *object = static_cast<Object*>(lua_touserdata(L, 1));

  object->setPosition(lua_tonumber(L,2),lua_tonumber(L,3),lua_tonumber(L,4));
  return 0;
}

#if DOXYGEN_ONLY
/**
 * Set the specified object's position.
 * @param GameObjectReference
 * @param Vector
 * @return n/a
 */
void setPositionv(GameObjectReference, Vector);
#endif
static int setPositionvLua(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  Object *object = static_cast<Object*>(lua_touserdata(L, 1));

  object->setPosition(loadVector(L));
  return 0;
}

#if DOXYGEN_ONLY
/**
 * Get the specified object's position.
 * @param GameObjectReference
 * @return Vector - position.
 */
Vector getPosition(GameObjectReference);
#endif
static int getPositionLua(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  Object *object = static_cast<Object*>(lua_touserdata(L, 1));

  Vector position = object->getPosition();

  returnVector(L, position);
  return 1;
}

#if DOXYGEN_ONLY
/**
 * Set the specified object's velocity.
 * @param GameObjectReference
 * @param float x component.
 * @param float y component.
 * @param float z component.
 * @return n/a
 */
void setVelocity(GameObjectReference, float, float, float);
#endif
static int setVelocityLua(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  Object *object = static_cast<Object*>(lua_touserdata(L, 1));

  object->setVelocity(lua_tonumber(L, 2),lua_tonumber(L, 3),lua_tonumber(L, 4));
  return 0;
}

#if DOXYGEN_ONLY
/**
 * Set the specified object's velocity.
 * @param GameObjectReference
 * @param Vector
 * @return n/a
 */
void setVelocityv(GameObjectReference, Vector);
#endif
static int setVelocityvLua(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  Object *object = static_cast<Object*>(lua_touserdata(L, 1));

  object->setVelocity(loadVector(L));
  return 0;
}

#if DOXYGEN_ONLY
/**
 * Get the specified object's velocity.
 * @param GameObjectReference
 * @return Vector - velocity.
 */
Vector getVelocity(GameObjectReference);
#endif
static int getVelocityLua(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  Object *object = static_cast<Object*>(lua_touserdata(L, 1));

  Vector velocity = object->getVelocity();

  returnVector(L, velocity);
  return 1;
}

#if DOXYGEN_ONLY
/**
 * Set the specified object's rotational velocity.
 * @param GameObjectReference
 * @param float rotation velocity x component.
 * @param float rotation velocity y component.
 * @param float rotation velocity z component.
 * @return n/a
 */
void setRotationVelocity(GameObjectReference, float, float, float);
#endif
static int setRotationVelocityLua(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  Object *object = static_cast<Object*>(lua_touserdata(L, 1));
  object->setRotationVelocity(lua_tonumber(L,2),lua_tonumber(L,3),lua_tonumber(L,4));
  return 0;
}

#if DOXYGEN_ONLY
/**
 * Set the specified object's rotational velocity.
 * @param GameObjectReference
 * @param Vector rotation velocity.
 * @return n/a
 */
void setRotationVelocityv(GameObjectReference, Vector);
#endif
static int setRotationVelocityvLua(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  Object *object = static_cast<Object*>(lua_touserdata(L, 1));
  object->setRotationVelocity(loadVector(L));
  return 0;
}

#if DOXYGEN_ONLY
/**
 * Fetch the specified object's rotational velocity.
 * @param GameObjectReference
 * @return Vector - rotational velocity.
 */
Vector getRotationVelocity(GameObjectReference);
#endif
static int getRotationVelocityLua(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  Object *object = static_cast<Object*>(lua_touserdata(L, 1));

  Vector rotationVelocity = object->getRotationVelocity();

  returnVector(L, rotationVelocity);
  return 1;
}

#if DOXYGEN_ONLY
/**
 * Set the specified object's speed.
 * @param GameObjectReference
 * @param float speed in direction of the object.
 * @param float speed in up direction of the object.
 * @param float speed in orthoginal direction of the object.
 * @return n/a
 */
void setSpeed(GameObjectReference, float, float, float);
#endif
static int setSpeedLua(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  Object *object = static_cast<Object*>(lua_touserdata(L, 1));

  object->setSpeed(lua_tonumber(L, 2),lua_tonumber(L, 3),lua_tonumber(L, 4));
  return 0;
}

#if DOXYGEN_ONLY
/**
 * Set the specified object's speed.
 * @param GameObjectReference
 * @param Vector speed.
 * @return n/a
 */
void setSpeedv(GameObjectReference, Vector);
#endif
static int setSpeedvLua(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  Object *object = static_cast<Object*>(lua_touserdata(L, 1));

  object->setSpeed(loadVector(L));
  return 0;
}

#if DOXYGEN_ONLY
/**
 * Fetch the specified object's speed.
 * @param GameObjectReference
 * @return Vector - speed.
 */
Vector getSpeed(GameObjectReference);
#endif
static int getSpeedLua(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  Object *object = static_cast<Object*>(lua_touserdata(L, 1));

  Vector speed = object->getSpeed();

  returnVector(L, speed);
  return 1;
}

#if DOXYGEN_ONLY
/**
 * Get a game object reference to the player object.
 * @return GameObjectReference
 */
GameObjectReference getPlayer();
#endif
static int getPlayerLua(lua_State *L) {
  lua_pushlightuserdata(L, (void*)g_script_player);
  return 1;
}

#if DOXYGEN_ONLY
/**
 * Get a game object reference to the active camera object.
 * @return GameObjectReference
 */
GameObjectReference getCamera();
#endif
static int getCameraLua(lua_State *L) {
  lua_pushlightuserdata(L, (void*)g_script_camera);
  return 1;
}

#if DOXYGEN_ONLY
/**
 * Set the specified object's rotation.
 * @param GameObjectReference
 * @param float x rotation component.
 * @param float y rotation component.
 * @param float z rotation component.
 * @return n/a
 */
void setRotation(GameObjectReference, float, float, float);
#endif
static int setRotationLua(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  Object *object = static_cast<Object*>(lua_touserdata(L, 1));
  object->setRotationEuler(lua_tonumber(L,2),lua_tonumber(L,3),lua_tonumber(L,4));
  object->setRotationChanged(true);
  return 0;
}

#if DOXYGEN_ONLY
/**
 * Set the specified object's rotation.
 * @param GameObjectReference
 * @param Vector rotation.
 * @return n/a
 */
void setRotationv(GameObjectReference, Vector);
#endif
static int setRotationvLua(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  Object *object = static_cast<Object*>(lua_touserdata(L, 1));
  object->setRotationEuler(loadVector(L));
  object->setRotationChanged(true);
  return 0;
}

#if DOXYGEN_ONLY
/**
 * Fetch the specified object's rotation.
 * @param GameObjectReference
 * @return Vector - rotation.
 */
Vector getRotation(GameObjectReference);
#endif
static int getRotationLua(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  Object *object = static_cast<Object*>(lua_touserdata(L, 1));

  Vector tv;

  object->getRotation().getEulerAngles(tv);

  returnVector(L, tv);
  return 1;
}

#if DOXYGEN_ONLY
/**
 * Get the specified object's direction.
 * @param GameObjectReference
 * @return Vector - direction.
 */
Vector getDirection(GameObjectReference);
#endif
static int getDirectionLua(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  Object *object = static_cast<Object*>(lua_touserdata(L, 1));

  Vector direction = object->getDirection();

  returnVector(L, direction);
  return 1;
}

#if DOXYGEN_ONLY
/**
 * Get the specified object's up vector.
 * @param GameObjectReference
 * @return Vector - up vector.
 */
Vector getUp(GameObjectReference);
#endif
static int getUpLua(lua_State *L)
{
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  Object *object = static_cast<Object*>(lua_touserdata(L, 1));

  Vector up = object->getUp();

  returnVector(L, up);
  return 1;
}

#if DOXYGEN_ONLY
/**
 * Get the specified object's orthogonal vector (orthogonal to up and direction vectors).
 * @param GameObjectReference
 * @return Vector - orthogonal vector.
 */
Vector getOrthogonal(GameObjectReference);
#endif
static int getOrthogonalLua(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  Object *object = static_cast<Object*>(lua_touserdata(L, 1));

  Vector orth = object->getOrth();

  returnVector(L, orth);
  return 1;
}

#if DOXYGEN_ONLY
/**
 * Add to the specified object's position.
 * @param GameObjectReference
 * @param float x coordinate.
 * @param float y coordinate.
 * @param float z coordinate.
 * @return n/a
 */
void addPosition(GameObjectReference, float, float, float);
#endif
static int addPositionLua(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  Object *object = static_cast<Object*>(lua_touserdata(L, 1));

  Vector position = object->getPosition();

  position += Vector(lua_tonumber(L,2),lua_tonumber(L,3),lua_tonumber(L,4));

  object->setPosition(position);

  return 0;
}

#if DOXYGEN_ONLY
/**
 * Add to the specified object's position.
 * @param GameObjectReference
 * @param Vector coordinates.
 * @return n/a
 */
void addPositionv(GameObjectReference, Vector);
#endif
static int addPositionvLua(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  Object *object = static_cast<Object*>(lua_touserdata(L, 1));

  Vector position = object->getPosition();

  position += Vector(loadVector(L));

  object->setPosition(position);
  return 0;
}

#if DOXYGEN_ONLY
/**
 * Add to the specified object's rotation.
 * @param GameObjectReference
 * @param float x rotation component.
 * @param float y rotation component.
 * @param float z rotation component.
 * @return n/a
 */
void addRotation(GameObjectReference, float, float, float);
#endif
static int addRotationLua(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  Object *object = static_cast<Object*>(lua_touserdata(L, 1));

  Quaternion qt;
  qt.buildFromEuler(lua_tonumber(L,2),lua_tonumber(L,3),lua_tonumber(L,4));
  object->setRotation(object->getRotation() * qt);

  return 0;
}

#if DOXYGEN_ONLY
/**
 * Add to the specified object's rotation.
 * @param GameObjectReference
 * @param Vector rotation.
 * @return n/a
 */
void addRotationv(GameObjectReference, Vector);
#endif
static int addRotationvLua(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  Object *object = static_cast<Object*>(lua_touserdata(L, 1));

  Quaternion qt;
  qt.buildFromEuler(loadVector(L));
  object->setRotation(object->getRotation() * qt);

  return 0;
}

#if DOXYGEN_ONLY
/**
 * Add to the specified object's velocity.
 * @param GameObjectReference
 * @param float x component.
 * @param float y component.
 * @param float z component.
 * @return n/a
 */
void addVelocity(GameObjectReference, float, float, float);
#endif
static int addVelocityLua(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  Object *object = static_cast<Object*>(lua_touserdata(L, 1));

  Vector velocity = object->getVelocity();

  velocity += Vector(lua_tonumber(L,2),lua_tonumber(L,3),lua_tonumber(L,4));

  object->setVelocity(velocity);

  return 0;
}

#if DOXYGEN_ONLY
/**
 * Add to the specified object's velocity.
 * @param GameObjectReference
 * @param Vector
 * @return n/a
 */
void addVelocityv(GameObjectReference, Vector);
#endif
static int addVelocityvLua(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  Object *object = static_cast<Object*>(lua_touserdata(L, 1));

  Vector velocity = object->getVelocity();

  velocity += Vector(loadVector(L));

  object->setVelocity(velocity);
  return 0;
}

#if DOXYGEN_ONLY
/**
 * Add to the specified object's speed.
 * @param GameObjectReference
 * @param float speed in direction of the object.
 * @param float speed in up direction of the object.
 * @param float speed in orthoginal direction of the object.
 * @return n/a
 */
void addSpeed(GameObjectReference, float, float, float);
#endif
static int addSpeedLua(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  Object *object = static_cast<Object*>(lua_touserdata(L, 1));

  Vector speed = object->getSpeed();

  speed += Vector(lua_tonumber(L,2),lua_tonumber(L,3),lua_tonumber(L,4));

  object->setSpeed(speed);
  return 0;
}

#if DOXYGEN_ONLY
/**
 * Add to the specified object's speed.
 * @param GameObjectReference
 * @param Vector speed.
 * @return n/a
 */
void addSpeedv(GameObjectReference, Vector);
#endif
static int addSpeedvLua(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  Object *object = static_cast<Object*>(lua_touserdata(L, 1));

  Vector speed = object->getSpeed();

  speed += Vector(loadVector(L));

  object->setSpeed(speed);

  return 0;
}

#if DOXYGEN_ONLY
/**
 * Add to the specified object's rotational velocity.
 * @param GameObjectReference
 * @param float rotation velocity x component.
 * @param float rotation velocity y component.
 * @param float rotation velocity z component.
 * @return n/a
 */
void addRotationVelocity(GameObjectReference, float, float, float);
#endif
static int addRotationVelocityLua(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  Object *object = static_cast<Object*>(lua_touserdata(L, 1));

  Vector rotationVelocity = object->getRotationVelocity();

  rotationVelocity += Vector(lua_tonumber(L,2),lua_tonumber(L,3),lua_tonumber(L,4));

  object->setRotationVelocity(rotationVelocity);

  return 0;
}

#if DOXYGEN_ONLY
/**
 * Add to the specified object's rotational velocity.
 * @param GameObjectReference
 * @param Vector rotation velocity.
 * @return n/a
 */
void addRotationVelocityv(GameObjectReference, Vector);
#endif
static int addRotationVelocityvLua(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  Object *object = static_cast<Object*>(lua_touserdata(L, 1));

  Vector rotationVelocity = object->getRotationVelocity();

  rotationVelocity += Vector(loadVector(L));

  object->setRotationVelocity(rotationVelocity);

  return 0;
}

#if DOXYGEN_ONLY
/**
 * Add to the scale for the specified object.
 * @param GameObjectReference
 * @param x scale factor.
 * @param y scale factor.
 * @param z scale factor.
 * @return n/a
 */
void addScale(GameObjectReference, float, float, float);
#endif
static int addScaleLua(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  Object *object = static_cast<Object*>(lua_touserdata(L, 1));

  Vector scale = object->getScale();

  scale += Vector(lua_tonumber(L,2),lua_tonumber(L,3),lua_tonumber(L,4));

  object->setScale(scale);

  return 0;
}

#if DOXYGEN_ONLY
/**
 * Add to the scale for the specified object.
 * @param GameObjectReference
 * @param Vector scale factor.
 * @return n/a
 */
void addScalev(GameObjectReference, Vector);
#endif
static int addScalevLua(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  Object *object = static_cast<Object*>(lua_touserdata(L, 1));

  Vector scale = object->getScale();

  scale += Vector(loadVector(L));

  object->setScale(scale);

  return 0;
}

#if DOXYGEN_ONLY
/**
 * Add to the rate that the specified game object should scale each game cycle.
 * @param GameObjectReference
 * @param x scale rate.
 * @param y scale rate.
 * @param z scale rate.
 * @return n/a
 */
void addScaleRate(GameObjectReference, float, float, float);
#endif
static int addScaleRateLua(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  Object *object = static_cast<Object*>(lua_touserdata(L, 1));

  Vector scaleRate = object->getScaleRate();

  scaleRate += Vector(lua_tonumber(L,2),lua_tonumber(L,3),lua_tonumber(L,4));

  object->setScaleRate(scaleRate);

  return 0;
}

#if DOXYGEN_ONLY
/**
 * Add to the rate that the specified game object should scale each game cycle.
 * @param GameObjectReference
 * @param Vector scale rate.
 * @return n/a
 */
void addScaleRatev(GameObjectReference, Vector);
#endif
static int addScaleRatevLua(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);

  Object *object = static_cast<Object*>(lua_touserdata(L, 1));

  Vector scaleRate = object->getScaleRate();

  scaleRate += Vector(loadVector(L));

  object->setScaleRate(scaleRate);

  return 0;
}

#if DOXYGEN_ONLY
/**
 * Set the end rotation (by axis) for the specified game object's interpolation.
 * @param GameObjectReference
 * @param float x component.
 * @param float y component.
 * @param float z component.
 * @param float degrees.
 * @return n/a
 */
void setInterpolationRotationEndAxis(GameObjectReference, float, float, float, float);
#endif
static int setInterpolationRotationEndAxisLua(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  Object *object = static_cast<Object*>(lua_touserdata(L, 1));

  Vector tv;
  tv.setVector(lua_tonumber(L,2),lua_tonumber(L,3),lua_tonumber(L,4));
  tv.normalize();

  Quaternion rInterpEnd = object->getRInterpEnd();

  rInterpEnd.buildFromAxis(tv, lua_tonumber(L,5));

  object->setRInterpEnd(rInterpEnd);

  return 0;
}

#if DOXYGEN_ONLY
/**
 * Set the end rotation (by axis) for the specified game object's interpolation.
 * @param GameObjectReference
 * @param Vector axis.
 * @param float degrees.
 * @return n/a
 */
void setInterpolationRotationEndAxisv(GameObjectReference, Vector, float);
#endif
static int setInterpolationRotationEndAxisvLua(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  Object *object = static_cast<Object*>(lua_touserdata(L, 1));

  Vector tv = loadVector(L);
  tv.normalize();

  Quaternion rInterpEnd = object->getRInterpEnd();

  rInterpEnd.buildFromAxis(tv, lua_tonumber(L,2));

  object->setRInterpEnd(rInterpEnd);

  return 0;
}

#if DOXYGEN_ONLY
/**
 * Set the beginning rotation (by axis) for the specified game object's interpolation.
 * @param GameObjectReference
 * @param float x component.
 * @param float y component.
 * @param float z component.
 * @param float degrees.
 * @return n/a
 */
void setInterpolationRotationStartAxis(GameObjectReference, float, float, float, float);
#endif
static int setInterpolationRotationStartAxisLua(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  Object *object = static_cast<Object*>(lua_touserdata(L, 1));

  Quaternion rInterpStart = object->getRInterpStart();

  Vector tv;
  tv.setVector(lua_tonumber(L,2),lua_tonumber(L,3),lua_tonumber(L,4));
  tv.normalize();

  rInterpStart.buildFromAxis(tv, lua_tonumber(L,5));

  object->setRInterpStart(rInterpStart);

  return 0;
}

#if DOXYGEN_ONLY
/**
 * Set the beginning rotation (by axis) for the specified game object's interpolation.
 * @param GameObjectReference
 * @param Vector axis.
 * @param float degrees.
 * @return n/a
 */
void setInterpolationRotationStartAxisv(GameObjectReference, Vector, float);
#endif
static int setInterpolationRotationStartAxisvLua(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  Object *object = static_cast<Object*>(lua_touserdata(L, 1));

  Quaternion rInterpStart = object->getRInterpStart();

  Vector tv = loadVector(L);
  tv.normalize();

  rInterpStart.buildFromAxis(tv, lua_tonumber(L,5));

  object->setRInterpStart(rInterpStart);

  return 0;
}

#if DOXYGEN_ONLY
/**
 * Set the beginning rotation for the specified game object's interpolation.
 * @param GameObjectReference
 * @param float x coordinate.
 * @param float y coordinate.
 * @param float z coordinate.
 * @return n/a
 */
void setInterpolationRotationStart(GameObjectReference, float, float, float);
#endif
static int setInterpolationRotationStartLua(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  Object *object = static_cast<Object*>(lua_touserdata(L, 1));

  Quaternion rInterpStart = object->getRInterpStart();

  rInterpStart.buildFromEuler(lua_tonumber(L,2),lua_tonumber(L,3),lua_tonumber(L,4));

  object->setRInterpStart(rInterpStart);

  return 0;
}

#if DOXYGEN_ONLY
/**
 * Set the beginning rotation for the specified game object's interpolation.
 * @param GameObjectReference
 * @param Vector coordinates.
 * @return n/a
 */
void setInterpolationRotationStartv(GameObjectReference, Vector);
#endif
static int setInterpolationRotationStartvLua(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  Object *object = static_cast<Object*>(lua_touserdata(L, 1));

  Quaternion rInterpStart = object->getRInterpStart();

  rInterpStart.buildFromEuler(loadVector(L));

  object->setRInterpStart(rInterpStart);

  return 0;
}

#if DOXYGEN_ONLY
/**
 * Set the end rotation for the specified game object's interpolation.
 * @param GameObjectReference
 * @param float x coordinate.
 * @param float y coordinate.
 * @param float z coordinate.
 * @return n/a
 */
void setInterpolationRotationEnd(GameObjectReference, float, float, float);
#endif
static int setInterpolationRotationEndLua(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  Object *object = static_cast<Object*>(lua_touserdata(L, 1));

  Quaternion rInterpEnd = object->getRInterpEnd();

  rInterpEnd.buildFromEuler(lua_tonumber(L,2),lua_tonumber(L,3),lua_tonumber(L,4));

  object->setRInterpEnd(rInterpEnd);

  return 0;
}

#if DOXYGEN_ONLY
/**
 * Set the end rotation for the specified game object's interpolation.
 * @param GameObjectReference
 * @param Vector coordinates.
 * @return n/a
 */
void setInterpolationRotationEndv(GameObjectReference, Vector);
#endif
static int setInterpolationRotationEndvLua(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  Object *object = static_cast<Object*>(lua_touserdata(L, 1));

  Quaternion rInterpEnd = object->getRInterpEnd();

  rInterpEnd.buildFromEuler(loadVector(L));

  object->setRInterpEnd(rInterpEnd);

  return 0;
}

#if DOXYGEN_ONLY
/**
 * Enable/Disable interpolation for the specified object.
 * @param GameObjectReference
 * @param bool enable/disable.
 * @return n/a
 */
void setInterpolationEnable(GameObjectReference, bool);
#endif
static int setInterpolationEnableLua(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  Object *object = static_cast<Object*>(lua_touserdata(L, 1));

  // TODO: there's probably a better way to do this...
  int op = (int)lua_tonumber(L,2);

  if (op == 0)
    object->setIsInterpolationEnabled(false);
  else
    object->setIsInterpolationEnabled(true);

  return (0);
}

#if DOXYGEN_ONLY
/**
 * Set beginning value of interpolation variable's value range.
 * @param GameObjectReference
 * @param float value.
 * @return n/a
 */
void setInterpolationVariableBeginValue(GameObjectReference, float);
#endif
static int setInterpolationVariableBeginValueLua(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  Object *object = static_cast<Object*>(lua_touserdata(L, 1));
  object->setValInterpBegin(lua_tonumber(L, 2));
  return 0;
}

#if DOXYGEN_ONLY
/**
 * Set end value of interpolation variable's value range.
 * @param GameObjectReference
 * @param float value.
 * @return n/a
 */
void setInterpolationVariableEndValue(GameObjectReference, float);
#endif
static int setInterpolationVariableEndValueLua(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  Object *object = static_cast<Object*>(lua_touserdata(L, 1));
  object->setValInterpEnd(lua_tonumber(L, 2));
  return 0;
}

#if DOXYGEN_ONLY
/**
 * Set current value of interpolation variable's value range.
 * @param GameObjectReference
 * @param float value.
 * @return n/a
 */
void setInterpolationVariableCurrentValue(GameObjectReference, float);
#endif
static int setInterpolationVariableCurrentValueLua(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  Object *object = static_cast<Object*>(lua_touserdata(L, 1));
  object->setValInterpCurrent(lua_tonumber(L, 2));
  return 0;
}

#if DOXYGEN_ONLY
/**
 * Set the specified object's rotational velocity from an axis and angle.
 * @param GameObjectReference
 * @param float x rotation velocity component.
 * @param float y rotation velocity component.
 * @param float z rotation velocity component.
 * @param float angle.
 * @return n/a
 */
void setRotationVelocityAxis(GameObjectReference, float, float, float, float);
#endif
static int setRotationVelocityAxisLua(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  Object *object = static_cast<Object*>(lua_touserdata(L, 1));

  Vector rotationVelocity = object->getRotationVelocity();

  rotationVelocity.setVector(lua_tonumber(L,2),lua_tonumber(L,3),lua_tonumber(L,4));
  rotationVelocity.normalize();
  rotationVelocity.scale(lua_tonumber(L,5));

  object->setRotationVelocity(rotationVelocity);

  return 0;
}

#if DOXYGEN_ONLY
/**
 * Set the specified object's rotational velocity.
 * @param GameObjectReference
 * @param Vector rotation velocity.
 * @return n/a
 */
void setRotationVelocityAxisv(GameObjectReference, Vector, float);
#endif
static int setRotationVelocityAxisvLua(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  Object *object = static_cast<Object*>(lua_touserdata(L, 1));

  Vector rotationVelocity = object->getRotationVelocity();

  rotationVelocity = loadVector(L);
  rotationVelocity.normalize();
  rotationVelocity.scale(lua_tonumber(L,2));

  object->setRotationVelocity(rotationVelocity);

  return 0;
}

#if DOXYGEN_ONLY
/**
 * Set the specified object's rotation.
 * @param GameObjectReference
 * @param float x rotation component.
 * @param float y rotation component.
 * @param float z rotation component.
 * @param float angle.
 * @return n/a
 */
void setRotationAxis(GameObjectReference, float, float, float, float);
#endif
static int setRotationAxisLua(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  Object *object = static_cast<Object*>(lua_touserdata(L, 1));

  Vector tv;
  tv.setVector(lua_tonumber(L,2),lua_tonumber(L,3),lua_tonumber(L,4));
  //tv.normalize();

  Quaternion rotation;

  rotation.buildFromAxis(tv, lua_tonumber(L,5));
  object->setRotationChanged(true);

  object->setRotation(rotation);

  return 0;
}

#if DOXYGEN_ONLY
/**
 * Set the specified object's rotation.
 * @param GameObjectReference
 * @param Vector rotation axis.
 * @param float angle.
 * @return n/a
 */
void setRotationAxisv(GameObjectReference, Vector, float);
#endif
static int setRotationAxisvLua(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  Object *object = static_cast<Object*>(lua_touserdata(L, 1));

  Vector tv = loadVector(L);
  //tv.normalize();

  Quaternion rotation;

  rotation.buildFromAxis(tv, lua_tonumber(L,2));

  Vector v;
  rotation.getEulerAngles(v);

  object->setRotationChanged(true);

  object->setRotation(rotation);

  return 0;
}

#if DOXYGEN_ONLY
/**
 * Add to the specified object's rotation.
 * @param GameObjectReference
 * @param float x rotation component.
 * @param float y rotation component.
 * @param float z rotation component.
 * @param float angle.
 * @return n/a
 */
void addRotationAxis(GameObjectReference, float, float, float, float);
#endif
static int addRotationAxisLua(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  Object *object = static_cast<Object*>(lua_touserdata(L, 1));

  Quaternion tq;
  Vector tv;
  tv.setVector(lua_tonumber(L,2),lua_tonumber(L,3),lua_tonumber(L,4));
  //tv.normalize();
  tq.buildFromAxis(tv, lua_tonumber(L,5));

  Quaternion rotation = object->getRotation();

  rotation = rotation * tq;

  object->setRotation(rotation);

  return 0;
}

#if DOXYGEN_ONLY
/**
 * Add to the specified object's rotation.
 * @param GameObjectReference
 * @param Vector rotation axis.
 * @param float angle.
 * @return n/a
 */
void addRotationAxisv(GameObjectReference, Vector, float);
#endif
static int addRotationAxisvLua(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  Object *object = static_cast<Object*>(lua_touserdata(L, 1));

  Quaternion tq;
  Vector tv = loadVector(L);
  tv.normalize();
  tq.buildFromAxis(tv, lua_tonumber(L,2));

  Quaternion rotation = object->getRotation();

  rotation = rotation * tq;

  object->setRotation(rotation);

  return 0;
}

#if DOXYGEN_ONLY
/**
 * Fetch the specified game object's timer.
 * @param GameObjectReference
 * @return float
 */
float getTimer(GameObjectReference, float);
#endif
static int getTimerLua(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  Object *object = static_cast<Object*>(lua_touserdata(L, 1));

  // TODO: Is this logic working properly?
  lua_pushnumber(L, 0);
  return 1;
}

#if DOXYGEN_ONLY
/**
 * Suspend running the specified game object's script until the suspend time has elapsed.
 * @param GameObjectReference
 * @param float - path to wav file to play.
 * @return n/a
 */
void suspend(GameObjectReference, float);
#endif
static int suspendLua(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  Object *object = static_cast<Object*>(lua_touserdata(L, 1));
  object->setSuspendTime(lua_tonumber(L, 2));
  return 0;
}

#if DOXYGEN_ONLY
/**
 * Play the sound in the specified wav file.
 * @param GameObjectReference
 * @param string - path to wav file to play.
 * @return n/a
 */
void playSound(GameObjectReference, string);
#endif
static int playSoundLua(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  Object *object = static_cast<Object*>(lua_touserdata(L, 1));

  const char *s = lua_tostring(L, 2);
  string sound = string(s);

  SoundFX *soundFx = g_script_engine->getSoundFxClass();

  if (soundFx != NULL)
    soundFx->PlaySFX(sound);

  return 0;
}

#if DOXYGEN_ONLY
/**
 * Add a game object to the current level.
 * @param TerrainReference
 * @param string - path to lua game object script for this object to use.
 * @param float - values to be passed into the "on_load" method for the game object.
 * @return GameObjectReference
 */
GameObjectReference addObject(TerrainReference, string, float, float, ...);
#endif
static int addObjectLua(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  Object *object = static_cast<Object*>(lua_touserdata(L, 1));

  const char *s = lua_tostring(L, 2);
  string script = string(s);

  int n = lua_gettop(L);
  n = n - 2;

  if (n > 8) n = 8;

  float args[8];

  for (int i = 0; i < n; i++) {
    args[i] = lua_tonumber(L, i+3);
  }

  ScriptedObject * temp = g_script_game_level->addObject(script, args, n);

  lua_pushlightuserdata(L, (void*)temp);
  return 1;
}

#if DOXYGEN_ONLY
/**
 * Add a game object to the current level.
 * @param TerrainReference
 * @param string - path to lua script for this text object to use.
 * @param float - values to be passed into the "on_load" method for the game object.
 * @return TextObjectReference
 */
TextObjectReference addText(TerrainReference, string, float, float, ...);
#endif
static int addTextLua(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  Object *object = static_cast<Object*>(lua_touserdata(L, 1));

  const char *s = lua_tostring(L, 2);
  string script = string(s);

  const char *t = lua_tostring(L, 3);
  string text = string(t);

  int n = lua_gettop(L);
  n = n - 3;

  if (n > 8) n = 8;

  float args[8];

  for (int i = 0; i < n; i++) {
    args[i] = lua_tonumber(L, i+4);
  }

  ScriptTextType *temp = g_script_game_level->addScriptTextType(script, args, n);
  temp->text = text;

  lua_pushlightuserdata(L, (void*)temp);
  return 1;
}

#if DOXYGEN_ONLY
/**
 * Remove the specified object from the level.
 * @param GameObjectReference
 * @return n/a
 */
void removeObject(GameObjectReference);
#endif
static int removeObjectLua(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  Object *object = static_cast<Object*>(lua_touserdata(L, 1));

  object->setState(DEAD);
  return 0;
}

#if DOXYGEN_ONLY
/**
 * Specify the model to render for this game object.
 * @param GameObjectReference
 * @string file containing the model to load.
 * @return n/a
 */
void setModel(GameObjectReference, string);
#endif
static int setModelLua(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  ScriptedObject *object = static_cast<ScriptedObject*>(lua_touserdata(L, 1));

  const char *s = lua_tostring(L, 2);
  string model = string(s);

  object->load(model);
  return 0;
}

#if DOXYGEN_ONLY
/**
 * Set the scale for the specified object.
 * @param GameObjectReference
 * @param x scale factor.
 * @param y scale factor.
 * @param z scale factor.
 * @return n/a
 */
void setScale(GameObjectReference, float, float, float);
#endif
static int setScaleLua(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  Object *object = static_cast<Object*>(lua_touserdata(L, 1));

  object->setScale(lua_tonumber(L,2),lua_tonumber(L,3),lua_tonumber(L,4));
  object->setScaleChanged(true);
  //object->update(lgameLevel->getLightDirection());
  return 0;
}

#if DOXYGEN_ONLY
/**
 * Set the scale for the specified object.
 * @param GameObjectReference
 * @param Vector scale factor.
 * @return n/a
 */
void setScalev(GameObjectReference, Vector);
#endif
static int setScalevLua(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  Object *object = static_cast<Object*>(lua_touserdata(L, 1));

  object->setScale(loadVector(L));
  object->setScaleChanged(true);
  return 0;
}

#if DOXYGEN_ONLY
/**
 * Set lua script for specified game object.
 * @param GameObjectReference
 * @param string lua script file name.
 * @return n/a
 */
void setScript(GameObjectReference, string);
#endif
static int setScriptLua(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  Object *object = static_cast<Object*>(lua_touserdata(L, 1));

  const char *s = lua_tostring(L, 2);
  string script = string(s);

  float args[8];

  object->unloadScript();
  object->setScript(script);
  object->loadScript(script, args, 0);

  return 0;
}

#if DOXYGEN_ONLY
/**
 * Enable the specified game object's "always lit" mode which will always render
 * the object's model in full direct light (regardless of the level's light direction).
 * @param GameObjectReference
 * @return n/a
 */
void enableAlwaysLit(GameObjectReference);
#endif
static int enableAlwaysLitLua(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  Object *object = static_cast<Object*>(lua_touserdata(L, 1));
  object->setIsAlwaysLit(true);
  return 0;
}

#if DOXYGEN_ONLY
/**
 * Disable the specified game object's "always lit" mode which will is the default setting
 * and renders object using the level's light direction
 * @param GameObjectReference
 * @return n/a
 */
void disableAlwaysLit(GameObjectReference);
#endif
static int disableAlwaysLitLua(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  Object *object = static_cast<Object*>(lua_touserdata(L, 1));
  object->setIsAlwaysLit(false);
  return 0;
}

#if DOXYGEN_ONLY
/**
 * Add a particle system to the specified object.
 * @param GameObjectReference
 * @param int - number representing the particle system to load.
 * @return n/a
 */
void addParticleSystem(GameObjectReference, int);
#endif
static int addParticleSystemLua(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  Object *object = static_cast<Object*>(lua_touserdata(L, 1));

  object->setParticleSystem((int)lua_tonumber(L, 2));

  return 0;
}

#if DOXYGEN_ONLY
/**
 * Set a type identification number for the specified game object.
 * @param GameObjectReference
 * @param int ID for this game object.
 * @return n/a
 */
void setTypeId(GameObjectReference, int);
#endif
static int setTypeIdLua(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  Object *object = static_cast<Object*>(lua_touserdata(L, 1));
  object->setTypeId((int)lua_tonumber(L, 2));
  return 0;
}

#if DOXYGEN_ONLY
/**
 * Fetch the type identification number for the specified game object.
 * @param GameObjectReference
 * @return int - identification number.
 */
void getTypeId(GameObjectReference);
#endif
static int getTypeIdLua(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  Object *object = static_cast<Object*>(lua_touserdata(L, 1));
  lua_pushnumber(L, object->getTypeId());
  return 1;
}

#if DOXYGEN_ONLY
/**
 * Set the rate that the specified game object should scale each game cycle.
 * @param GameObjectReference
 * @param x scale rate.
 * @param y scale rate.
 * @param z scale rate.
 * @return n/a
 */
void setScaleRate(GameObjectReference, float, float, float);
#endif
static int setScaleRateLua(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  Object *object = static_cast<Object*>(lua_touserdata(L, 1));

  Vector scaleRate = object->getScaleRate();

  scaleRate.setVector(lua_tonumber(L, 2), lua_tonumber(L, 3), lua_tonumber(L, 4));

  object->setScaleRate(scaleRate);
  return 0;
}

#if DOXYGEN_ONLY
/**
 * Set the rate that the specified game object should scale each game cycle.
 * @param GameObjectReference
 * @param Vector scale rate.
 * @return n/a
 */
void setScaleRatev(GameObjectReference, Vector);
#endif
static int setScaleRatevLua(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  Object *object = static_cast<Object*>(lua_touserdata(L, 1));

  Vector scaleRate = object->getScaleRate();

  scaleRate = loadVector(L);

  object->setScaleRate(scaleRate);

  return 0;
}

#if DOXYGEN_ONLY
/**
 * Fetch the scale for the specified game object.
 * @param GameObjectReference
 * @return Vector - game object's scale factor.
 */
Vector getScale(GameObjectReference);
#endif
static int getScaleLua(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  Object *object = static_cast<Object*>(lua_touserdata(L, 1));

  Vector scale = object->getScale();

  returnVector(L, scale);
  return 1;
}

#if DOXYGEN_ONLY
/**
 * Fetch the scale rate for the specified game object.
 * @param GameObjectReference
 * @return Vector - game object's scale rate.
 */
Vector getScaleRate(GameObjectReference);
#endif
static int getScaleRateLua(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  Object *object = static_cast<Object*>(lua_touserdata(L, 1));

  Vector scaleRate = object->getScaleRate();

  returnVector(L, scaleRate);
  return 1;
}

#if DOXYGEN_ONLY
/**
 * Fetch variable value from specified game object's script.
 * @param GameObjectReference
 * @return string - variable name.
 */
float getScriptValue(GameObjectReference, string);
#endif
static int getScriptValueLua(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  Object *object = static_cast<Object*>(lua_touserdata(L, 1));
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
void setScriptValue(GameObjectReference, string, float);
#endif
static int setScriptValueLua(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  Object *object = static_cast<Object*>(lua_touserdata(L, 1));
  const char *s = lua_tostring(L, 2);
  float value = (int)lua_tonumber(L, 3);

  object->setScriptValue(s, value);
  return 0;
}

#if DOXYGEN_ONLY
/**
 * Enable collision detection between the specified object and other game objects.
 * @param GameObjectReference
 * @return n/a
 */
void enableCollisionDetection(GameObjectReference);
#endif
static int enableCollisionDetectionLua(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  Object *object = static_cast<Object*>(lua_touserdata(L, 1));
  object->setCollisionDetectionEnabled(true);
  return 0;
}

#if DOXYGEN_ONLY
/**
 * Disable collision detection between the specified object and all other game objects.
 * @param GameObjectReference
 * @return n/a
 */
void disableCollisionDetection(GameObjectReference);
#endif
static int disableCollisionDetectionLua(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  Object *object = static_cast<Object*>(lua_touserdata(L, 1));
  object->setCollisionDetectionEnabled(false);
  return 0;
}

#if DOXYGEN_ONLY
/**
 * Query the game engine to determine if collision detection has been enabled
 * for the specified game object.
 * @param GameObjectReference
 * @return bool
 */
void getCollisionDetectionEnabled(GameObjectReference);
#endif
static int getCollisionDetectionEnabledLua(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  Object *object = static_cast<Object*>(lua_touserdata(L, 1));
  lua_pushnumber(L, object->getCollisionDetectionEnabled());
  return 1;
}

#if DOXYGEN_ONLY
/**
 * Set the active state of the specified object to true (active).
 * @param GameObjectReference game object reference obtained from the engine
 *        through another API call.
 * @return n/a
 */
void setActive(GameObjectReference);
#endif
static int setActiveLua(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  Object *object = static_cast<Object*>(lua_touserdata(L, 1));
  object->setActive(true);
  return 0;
}

#if DOXYGEN_ONLY
/**
 * Set the active state of the specified object to false (inactive).
 * @param GameObjectReference game object reference obtained from the engine
 *        through another API call.
 * @return n/a
 */
void setInactive(GameObjectReference);
#endif
static int setInactiveLua(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  Object *object = static_cast<Object*>(lua_touserdata(L, 1));
  object->setActive(false);
  return 0;
}

#if DOXYGEN_ONLY
/**
 * Obtain the active/inactive state of the specified object.
 * @param GameObjectReference game object reference obtained from the engine
 *        through another API call.
 * @return bool - true = active, false = inactive
 */
bool getActive(GameObjectReference);
#endif
static int getActiveLua(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  Object *object = static_cast<Object*>(lua_touserdata(L, 1));
  lua_pushnumber(L, object->getActive());
  return 1;
}

#if DOXYGEN_ONLY
/**
 * Designate the specified game object to be drawn by the engine each game cycle.
 * @param GameObjectReference
 * @return n/a
 */
void enableDraw(GameObjectReference);
#endif
static int enableDrawLua(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  Object *object = static_cast<Object*>(lua_touserdata(L, 1));
  object->setDrawEnabled(true);
  return 0;
}

#if DOXYGEN_ONLY
/**
 * Designate the specified game object not to be drawn by the engine each game cycle.
 * @param GameObjectReference
 * @return n/a
 */
void disableDraw(GameObjectReference);
#endif
static int disableDrawLua(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  Object *object = static_cast<Object*>(lua_touserdata(L, 1));
  object->setDrawEnabled(false);
  return 0;
}

#if DOXYGEN_ONLY
/**
 * Query the engine to determine if the specified game object will be drawn each game cycle.
 * @param GameObjectReference
 * @return bool - whether or not the object will be drawn
 */
void getDrawEnabled(GameObjectReference);
#endif
static int getDrawEnabledLua(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  Object *object = static_cast<Object*>(lua_touserdata(L, 1));
  lua_pushnumber(L, object->getDrawEnabled());
  return 1;
}

#if DOXYGEN_ONLY
/**
 * Fetch the radius of the specified game object's bounding sphere.
 * @param GameObjectReference
 * @return float - radius
 */
float getBoundingSphereRadius(GameObjectReference);
#endif
static int getBoundingSphereRadiusLua(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  Object *object = static_cast<Object*>(lua_touserdata(L, 1));
  lua_pushnumber(L, object->getBoundingSphere().getRadius());
  return 1;
}

#if DOXYGEN_ONLY
/**
 * Determine whether the specified object is in the current camera's view.
 * @param GameObjectReference
 * @return bool - whether or not the object is in the camera's view
 */
float getInView(GameObjectReference);
#endif
static int getInViewLua(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  Object *object = static_cast<Object*>(lua_touserdata(L, 1));
  lua_pushnumber(L, object->getInView());
  return 1;
}

#if DOXYGEN_ONLY
/**
 * Rotate the specified object such that it appears to be resting on the terrain.
 * Additionally, the specified rotation will be used as the base rotation of the game object.
 * @param GameObjectReference
 * @param x rotation around x axis
 * @param y rotation around y axis
 * @param z rotation around z axis
 * @return n/a
 */
void orientOnTerrain(GameObjectReference, float, float, float);
#endif
static int orientOnTerrainLua(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  Model *object = static_cast<Model*>(lua_touserdata(L, 1));

  Quaternion rotation;
  rotation.buildFromEuler(lua_tonumber(L,2),lua_tonumber(L,3),lua_tonumber(L,4));

  object->orientOnTerrain(g_script_game_level->getTerrain(), rotation);

  return 0;
}

#if DOXYGEN_ONLY
/**
 * Determine the height of the terrain beneath the specified game object and
 * set its height to match the terrain's height at this location. The offset
 * is used to adjust the object's height along the y axis.
 * @param GameObjectReference
 * @param float - height offset
 * @return n/a
 */
void setHeightFromTerrain(GameObjectReference, float);
#endif
static int setHeightFromTerrainLua(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  Model *object = static_cast<Model*>(lua_touserdata(L, 1));

  float offset = lua_tonumber(L, 2);

  object->setHeightFromTerrain(g_script_game_level->getTerrain(), offset);

  return 0;
}

#if DOXYGEN_ONLY
/**
 * Query the engine to determine if the specified key has been pressed this game cycle.
 * @param int - integer representation of the key
 * @return bool
 */
bool engine_testKeyPressed(int);
#endif
static int engine_testKeyPressedLua(lua_State *L) {
  int key = (int)lua_tonumber(L, 1);
  int result = (int)(g_script_engine->getKeyState()->testKeyPressed(key));
  lua_pushnumber(L, result);

  return 1;
}

#if DOXYGEN_ONLY
/**
 * Query the engine to determine if the specified key has been released this game cycle.
 * @param int - integer representation of the key
 * @return bool
 */
bool engine_testKeyReleased(int);
#endif
static int engine_testKeyReleasedLua(lua_State *L) {
  int key = (int)lua_tonumber(L, 1);
  int result = (int)(g_script_engine->getKeyState()->testKeyReleased(key));
  lua_pushnumber(L, result);
  return 1;
}

#if DOXYGEN_ONLY
/**
 * Query the engine to determine if the specified key has been pressed this game cycle.
 * @param int - integer representation of the special key
 * @return bool
 */
bool engine_testKeyPressed(int);
#endif
static int engine_testSpecialKeyPressedLua(lua_State *L) {
  int key = (int)lua_tonumber(L, 1);
  int result = (int)(g_script_engine->getSpecialKeyState()->testKeyPressed(key));
  lua_pushnumber(L, result);

  return 1;
}

#if DOXYGEN_ONLY
/**
 * Query the engine to determine if the specified key has been released this game cycle.
 * @param int - integer representation of the special key
 * @return bool
 */
bool engine_testSpecialKeyRelease(int);
#endif
static int engine_testSpecialKeyReleasedLua(lua_State *L) {
  int key = (int)lua_tonumber(L, 1);
  int result = (int)(g_script_engine->getSpecialKeyState()->testKeyReleased(key));
  lua_pushnumber(L, result);
  return 1;
}

#if DOXYGEN_ONLY
/**
 * Query the engine for the state of the compiler's DEBUG flag.
 * @return bool
 */
bool engine_testDebugEnabled();
#endif
static int engine_testDebugEnabledLua(lua_State *L) {
  lua_pushnumber(L, DEBUG);
  return 1;
}

#if DOXYGEN_ONLY
/**
 * Query the engine to determine the mouse's last known coordinates.
 * @return Vertex2 - x, y coordinate of the mouse.
 */
Vertex2 engine_getMouseCoordinates();
#endif
static int engine_getMouseCoordinatesLua(lua_State *L) {
  float x = g_script_engine->getMouseX();
  float y = g_script_engine->getMouseY();

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
 * Query the engine for a game object of the specified type.
 * @param int - ID of object type to find.
 * @return Vertex2 - x, y coordinate of the mouse.
 */
GameObjectReference level_findObjectOfType(int);
#endif
static int level_findObjectOfTypeLua(lua_State *L) {
  int type = (int)lua_tonumber(L, 1);
  Object *temp = g_script_game_level->findEnemyOfType(type);

  if (temp != NULL)
    lua_pushlightuserdata(L, (void*)temp);
  else
    lua_pushnil(L);

  return 1;
}

#if DOXYGEN_ONLY
/**
 * Calculate scalar value between two vectors.
 * @param Vector
 * @param Vector
 * @return float
 */
float vector_getScalar(Vector, Vector);
#endif
static int vector_getScalarLua(lua_State *L) {
  Vector t = loadVector(L);
  Vector u = loadVector(L);
  lua_pushnumber(L, t.getScaler(u));
  return 1;
}

#if DOXYGEN_ONLY
/**
 * Normalize vector.
 * @param Vector
 * @return Vector
 */
Vector vector_normalize(Vector, Vector);
#endif
static int vector_normalizeLua(lua_State *L) {
  Vector t = loadVector(L);
  t.normalize();
  returnVector(L, t);
  return 1;
}

#if DOXYGEN_ONLY
/**
 * Calculate dot product between two vectors.
 * @param Vector
 * @param Vector
 * @return float - dot product.
 */
float vector_dotProduct(Vector, Vector);
#endif
static int vector_dotProductLua(lua_State *L) {
  Vector t = loadVector(L);
  Vector u = loadVector(L);
  float angle = t.dotProduct(u);
  lua_pushnumber(L, angle);
  return 1;
}

#if DOXYGEN_ONLY
/**
 * Calculate cross product between two vectors.
 * @param Vector
 * @param Vector
 * @return Vector
 */
Vector vector_crossProduct(Vector, Vector);
#endif
static int vector_crossProductLua(lua_State *L) {
  Vector t = loadVector(L);
  Vector u = loadVector(L);
  Vector result;
  result.crossProduct(t, u);
  returnVector(L, result);
  return 1;
}

#if DOXYGEN_ONLY
/**
 * Get height of terrain at specified x,z coordinates.
 * @param int x coordinate.
 * @param int z coordinate.
 * @return Vector
 */
Vector terrain_getVertexHeight(int, int);
#endif
static int terrain_getVertexHeightLua(lua_State *L) {
  int x = lua_tonumber(L,1);
  int z = lua_tonumber(L,2);

  float height = (float)g_script_game_level->getTerrain()->getVertexHeight(x, z);

  lua_pushnumber(L, height);

  return 1;
}

#if DOXYGEN_ONLY
/**
 * Get color of terrain at specified x,z coordinates.
 * @param int x coordinate.
 * @param int z coordinate.
 * @return Vector
 */
Vector terrain_getVertexType(int, int);
#endif
static int terrain_getVertexColorLua(lua_State *L) {
  int x = lua_tonumber(L,1);
  int z = lua_tonumber(L,2);

  Vector color = g_script_game_level->getTerrain()->getVertexColor(x, z);

  returnVector(L, color);

  return 1;
}

#if DOXYGEN_ONLY
/**
 * Get type of terrain at specified x,z coordinates.
 * @param int x coordinate.
 * @param int z coordinate.
 * @return float - type.
 */
float terrain_getVertexType(int, int);
#endif
static int terrain_getVertexTypeLua(lua_State *L) {
  int x = lua_tonumber(L,1);
  int z = lua_tonumber(L,2);

  float type = g_script_game_level->getTerrain()->getVertexType(x, z);

  lua_pushnumber(L, type);

  return 1;
}

#if DOXYGEN_ONLY
/**
 * Set height of terrain at specified x,z coordinates.
 * @param int x coordinate.
 * @param int z coordinate.
 * @param float height
 * @return n/a.
 */
void terrain_setVertexHeight(int, int, Vector);
#endif
static int terrain_setVertexHeightLua(lua_State *L) {
  int x = lua_tonumber(L,1);
  int z = lua_tonumber(L,2);
  float height = lua_tonumber(L,3);

  g_script_game_level->getTerrain()->setVertexHeight(x, z, height);

  return 0;
}

#if DOXYGEN_ONLY
/**
 * Set type of terrain at specified x,z coordinates.
 * @param int x coordinate.
 * @param int z coordinate.
 * @param Vector containing r,g,b values.
 * @return n/a.
 */
void terrain_setVertexType(int, int, Vector);
#endif
static int terrain_setVertexColorLua(lua_State *L) {
  int x = lua_tonumber(L,1);
  int z = lua_tonumber(L,2);
  Vector color = loadVector(L);

  g_script_game_level->getTerrain()->setVertexColor(x, z, color);

  return 0;
}

#if DOXYGEN_ONLY
/**
 * Set type of terrain at specified x,z coordinates.
 * @param int x coordinate.
 * @param int z coordinate.
 * @param int type
 * @return n/a.
 */
void terrain_setVertexType(int, int, int);
#endif
static int terrain_setVertexTypeLua(lua_State *L) {
  int x = lua_tonumber(L,1);
  int z = lua_tonumber(L,2);
  int type = lua_tonumber(L,3);

  g_script_game_level->getTerrain()->setVertexType(x, z, type);

  return 0;
}

#if DOXYGEN_ONLY
/**
 * Get height of terrain at specified x,z coordinates.
 * @param int x coordinate.
 * @param int z coordinate.
 * @return float - height.
 */
float terrain_getHeight(int, int);
#endif
static int terrain_getHeightLua(lua_State *L) {
  float x = lua_tonumber(L,1);
  float z = lua_tonumber(L,2);

  float height = (float)g_script_game_level->getTerrain()->getHeight(x, z);

  lua_pushnumber(L, height);

  return 1;
}

#if DOXYGEN_ONLY
/**
 * Add to the camera's rotation.
 * @param GameObjectReference
 * @param float x rotation component.
 * @param float y rotation component.
 * @param float z rotation component.
 * @return n/a
 */
void camera_addRotation(CameraObjectReference, float, float, float);
#endif
static int camera_addRotationLua(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  Camera *object = static_cast<Camera*>(lua_touserdata(L, 1));

  Quaternion qt;
  qt.buildFromEuler(lua_tonumber(L,2),lua_tonumber(L,3),lua_tonumber(L,4));
  object->worldRotation = object->worldRotation * qt;

  return 0;
}

#if DOXYGEN_ONLY
/**
 * Return the definition of he specified plane, a list containing A, B, C and D, where: Ax + By + Cz + D = 0.
 * @param GameObjectReference
 * @param int plane number (0-6), where: 0 - left, 1 - right, 2 - down, 3 - up, 4 - far, and 5 - near.
 * @return array - definition of plane.
 */
array camera_getFrustumPlane(int);
#endif
static int camera_getFrustumPlaneLua(lua_State *L) {
  int plane_num = lua_tonumber(L,1);
  float a, b, c, d;

  static_cast<Camera*>(g_script_camera)->getFrustum()->getPlaneDefinition(plane_num, a, b, c, d);

  lua_newtable(L);

  lua_pushnumber(L, 1);
  lua_pushnumber(L, a);
  lua_rawset(L, -3);

  lua_pushnumber(L, 2);
  lua_pushnumber(L, b);
  lua_rawset(L, -3);

  lua_pushnumber(L, 3);
  lua_pushnumber(L, c);
  lua_rawset(L, -3);

  lua_pushnumber(L, 4);
  lua_pushnumber(L, d);
  lua_rawset(L, -3);

  return 1;
}
///@}

// helpers ////////////////////////////////////////////////////////////////////
static Vector loadVector(lua_State *L) {
  Vector t;

  // x
  lua_pushnumber(L, 1);
  lua_gettable(L, -2);
  t.x = (float)lua_tonumber(L, -1);
  lua_pop(L, 1);

  // y
  lua_pushnumber(L, 2);
  lua_gettable(L, -2);
  t.y = (float)lua_tonumber(L, -1);
  lua_pop(L, 1);

  // z
  lua_pushnumber(L, 3);
  lua_gettable(L, -2);
  t.z = (float)lua_tonumber(L, -1);
  lua_pop(L, 1);

  lua_pop(L, 1);

  return t;
}

static void returnVector(lua_State *L, Vector &t) {
  lua_newtable(L);

  lua_pushnumber(L, 1);
  lua_pushnumber(L, t.x);
  lua_rawset(L, -3);

  lua_pushnumber(L, 2);
  lua_pushnumber(L, t.y);
  lua_rawset(L, -3);

  lua_pushnumber(L, 3);
  lua_pushnumber(L, t.z);
  lua_rawset(L, -3);
}

static int registerFunctions(lua_State *L, int level) {
  // game objects
  lua_register(L, "setPosition", setPositionLua);
  lua_register(L, "setPositionv", setPositionvLua);
  lua_register(L, "getPosition", getPositionLua);
  lua_register(L, "setVelocity", setVelocityLua);
  lua_register(L, "setVelocityv", setVelocityvLua);
  lua_register(L, "getVelocity", getVelocityLua);
  lua_register(L, "setRotationVelocity", setRotationVelocityLua);
  lua_register(L, "setRotationVelocityv", setRotationVelocityvLua);
  lua_register(L, "getRotationVelocity", getRotationVelocityLua);
  lua_register(L, "setSpeed", setSpeedLua);
  lua_register(L, "setSpeedv", setSpeedvLua);
  lua_register(L, "getSpeed", getSpeedLua);
  lua_register(L, "setRotation", setRotationLua);
  lua_register(L, "setRotationv", setRotationvLua);
  lua_register(L, "getRotation", getRotationLua);
  lua_register(L, "getCamera", getCameraLua);
  lua_register(L, "getPlayer", getPlayerLua);
  lua_register(L, "getDirection", getDirectionLua);
  lua_register(L, "getUp", getUpLua);
  lua_register(L, "getOrthogonal", getOrthogonalLua);
  lua_register(L, "addPosition", addPositionLua);
  lua_register(L, "addPositionv", addPositionvLua);
  lua_register(L, "addRotation", addRotationLua);
  lua_register(L, "addRotationv", addRotationvLua);
  lua_register(L, "addVelocity", addVelocityLua);
  lua_register(L, "addVelocityv", addVelocityvLua);
  lua_register(L, "addSpeed", addSpeedLua);
  lua_register(L, "addSpeedv", addSpeedvLua);
  lua_register(L, "addRotationVelocity", addRotationVelocityLua);
  lua_register(L, "addRotationVelocityv", addRotationVelocityvLua);
  lua_register(L, "addScale", addScaleLua);
  lua_register(L, "addScalev", addScalevLua);
  lua_register(L, "addScaleRate", addScaleRateLua);
  lua_register(L, "addScaleRatev", addScaleRatevLua);
  lua_register(L, "setInterpolationRotationStartAxis", setInterpolationRotationStartAxisLua);
  lua_register(L, "setInterpolationRotationStartAxisv", setInterpolationRotationStartAxisvLua);
  lua_register(L, "setInterpolationRotationEndAxis", setInterpolationRotationEndAxisLua);
  lua_register(L, "setInterpolationRotationEndAxisv", setInterpolationRotationEndAxisvLua);
  lua_register(L, "setInterpolationRotationStart", setInterpolationRotationStartLua);
  lua_register(L, "setInterpolationRotationStartv", setInterpolationRotationStartvLua);
  lua_register(L, "setInterpolationRotationEnd", setInterpolationRotationEndLua);
  lua_register(L, "setInterpolationRotationEndv", setInterpolationRotationEndvLua);
  lua_register(L, "setInterpolationEnable", setInterpolationEnableLua);
  lua_register(L, "setInterpolationVariableBeginValue", setInterpolationVariableBeginValueLua);
  lua_register(L, "setInterpolationVariableEndValue", setInterpolationVariableEndValueLua);
  lua_register(L, "setInterpolationVariableCurrentValue", setInterpolationVariableCurrentValueLua);
  lua_register(L, "setRotationVelocityAxis", setRotationVelocityAxisLua);
  lua_register(L, "setRotationAxis", setRotationAxisLua);
  lua_register(L, "setRotationAxisv", setRotationAxisvLua);
  lua_register(L, "addRotationAxis", addRotationAxisLua);
  lua_register(L, "addRotationAxisv", addRotationAxisvLua);
  lua_register(L, "getTimer", getTimerLua);
  lua_register(L, "suspend", suspendLua);
  lua_register(L, "playSound", playSoundLua);
  lua_register(L, "addObject", addObjectLua);
  lua_register(L, "removeObject", removeObjectLua);
  lua_register(L, "setModel", setModelLua);
  lua_register(L, "setScale", setScaleLua);
  lua_register(L, "setScalev", setScalevLua);
  lua_register(L, "setScript", setScriptLua);
  lua_register(L, "setScaleRate", setScaleRateLua);
  lua_register(L, "setScaleRatev", setScaleRatevLua);
  lua_register(L, "getScale", getScaleLua);
  lua_register(L, "getScaleRate", getScaleRateLua);
  lua_register(L, "addParticleSystem", addParticleSystemLua);
  lua_register(L, "getTypeId", getTypeIdLua);
  lua_register(L, "setTypeId", setTypeIdLua);
  lua_register(L, "getScriptValue", getScriptValueLua);
  lua_register(L, "setScriptValue", setScriptValueLua);
  lua_register(L, "getCollisionDetectionEnabled", getCollisionDetectionEnabledLua);
  lua_register(L, "enableCollisionDetection", enableCollisionDetectionLua);
  lua_register(L, "disableCollisionDetection", disableCollisionDetectionLua);
  lua_register(L, "getActive", getActiveLua);
  lua_register(L, "setActive", setActiveLua);
  lua_register(L, "setInactive", setInactiveLua);
  lua_register(L, "getDrawEnabled", getDrawEnabledLua);
  lua_register(L, "enableDraw", enableDrawLua);
  lua_register(L, "disableDraw", disableDrawLua);
  lua_register(L, "getBoundingSphereRadius", getBoundingSphereRadiusLua);
  lua_register(L, "getInView", getInViewLua);
  lua_register(L, "addText", addTextLua);
  lua_register(L, "enableAlwaysLit", enableAlwaysLitLua);
  lua_register(L, "disableAlwaysLit", disableAlwaysLitLua);
  lua_register(L, "orientOnTerrain", orientOnTerrainLua);
  lua_register(L, "setHeightFromTerrain", setHeightFromTerrainLua);
  lua_register(L, "engine_testKeyPressed", engine_testKeyPressedLua);
  lua_register(L, "engine_testKeyReleased", engine_testKeyReleasedLua);
  lua_register(L, "engine_testSpecialKeyPressed", engine_testSpecialKeyPressedLua);
  lua_register(L, "engine_testSpecialKeyReleased", engine_testSpecialKeyReleasedLua);
  lua_register(L, "engine_testDebugEnabled", engine_testDebugEnabledLua);
  lua_register(L, "engine_getMouseCoordinates", engine_getMouseCoordinatesLua);
  lua_register(L, "level_findObjectOfType", level_findObjectOfTypeLua);
  lua_register(L, "vector_getScalar", vector_getScalarLua);
  lua_register(L, "vector_normalize", vector_normalizeLua);
  lua_register(L, "vector_dotProduct", vector_dotProductLua);
  lua_register(L, "vector_crossProduct", vector_crossProductLua);
  lua_register(L, "terrain_getVertexHeight", terrain_getVertexHeightLua);
  lua_register(L, "terrain_getVertexColor", terrain_getVertexColorLua);
  lua_register(L, "terrain_getVertexType", terrain_getVertexTypeLua);
  lua_register(L, "terrain_setVertexHeight", terrain_setVertexHeightLua);
  lua_register(L, "terrain_setVertexColor", terrain_setVertexColorLua);
  lua_register(L, "terrain_setVertexType", terrain_setVertexTypeLua);
  lua_register(L, "terrain_getHeight", terrain_getHeightLua);
  lua_register(L, "camera_addRotation", camera_addRotationLua);
  lua_register(L, "camera_getFrustumPlane", camera_getFrustumPlaneLua);

  // text based functions
  lua_register(L, "setFadeRate", setFadeRateLua);
  lua_register(L, "getFadeRate", getFadeRateLua);
  lua_register(L, "setAlpha", setAlphaLua);
  lua_register(L, "getAlpha", getAlphaLua);
  lua_register(L, "setColor", setColorLua);
  lua_register(L, "setColorv", setColorvLua);
  lua_register(L, "getColor", getColorLua);
  lua_register(L, "setWidth", setWidthLua);
  lua_register(L, "getWidth", getWidthLua);

  // game level only
  if (level < 2) {
    lua_register(L, "playBgMusic", playBgMusicLua);
    lua_register(L, "stopBgMusic", stopBgMusicLua);
    lua_register(L, "pauseBgMusic", pauseBgMusicLua);
    lua_register(L, "setSkyBox", setSkyBoxLua);
    lua_register(L, "setLightDirection", setLightDirectionLua);
    lua_register(L, "setLightDirectionv", setLightDirectionvLua);
    lua_register(L, "setTerrain", setTerrainLua);
    lua_register(L, "displayText", displayTextLua);
    lua_register(L, "renderText", renderTextLua);
    lua_register(L, "getFps", getFpsLua);
    lua_register(L, "getCameraId", getCameraIdLua);
    lua_register(L, "getLightDirection", getLightDirectionLua);
    lua_register(L, "setComplete", setCompleteLua);
    lua_register(L, "shutdown", shutdownLua);
  }

  // game only
  if (level == 0) {
    lua_register(L, "loadLevel", loadLevelLua);
    lua_register(L, "loadLevelFromBuffer", loadLevelFromBufferLua);
    lua_register(L, "pause", pauseLua);
    lua_register(L, "testLevelComplete", testLevelCompleteLua);
    lua_register(L, "getLevelId", getLevelIdLua);
    lua_register(L, "setLevelId", setLevelIdLua);
    lua_register(L, "swapLevel", swapLevelLua);
  }
}
#endif
