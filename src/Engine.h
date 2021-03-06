/**
 * @file Engine.h
 * @brief Containing class for all game engine management.
 *
 * Game engine management.
 */
#ifndef _ENGINE_H_
#define _ENGINE_H_

#include <GL/glut.h>
#include <string>

#include "SoundFX.h"
#include "gametimer.h"
#include "KeyState.h"
#include "GameLevel.h"
#include "TextGameObject.h"

extern "C" {
  #include "lua5.1/lua.h"
  #include "lua5.1/lualib.h"
  #include "lua5.1/lauxlib.h"
}

class Engine {
  public:
    Engine();
    ~Engine();

    void gameCycle();
    void prepare();
    void initGL();

    void processKeyUp(const int&);
    void processKeyDown(const int&);
    void processCommands();
    void processNormalKeyUp(const unsigned char&);
    void processNormalKeyDown(const unsigned char&);

    KeyState* getKeyState()        { return &keyState_; }
    KeyState* getSpecialKeyState() { return &specialKeyState_; }

    void processMouseMove(const int&, const int&);
    float getMouseX() { return mouseX_; }
    float getMouseY() { return mouseY_; }

    void loadLevel(const char*);
    void loadLevelFromBuffer(const char*);

    bool loadGame(const std::string&);
    bool loadIntroCredits();

    void updateGame(const float&);
    void unloadGame();
    void unload();

    void shutdown();
    void pause();

    float getFps() { return fps_; }

    void loadModels();
    void loadTextures();

    void renderText(const char* s, const float &x, const float &y) {
      glRasterPos2f (x, y);
      TextGameObject::render_string(GLUT_BITMAP_HELVETICA_18, s);
    }

    SoundFX* getSoundFxClass() { return &soundFx_; }
    bool getIsRunning()        { return isRunning_; }

    GameLevel* getCurrentLevel() { return currentLevel_; }

    void swapLevel() {
      GameLevel *temp = currentLevel_;
      currentLevel_   = storedLevel_;
      storedLevel_    = temp;
    }

    bool loadPng(const char*, unsigned char**, unsigned int*, unsigned int*, unsigned int*);

#if EDIT
    void getTerrainInfo(int &x, int &z, float &height, int &type, float &red, float &green, float &blue);
    void updateTerrain(int &x, int &z, float &height, int &type, float &red, float &green, float &blue);

    void updateColor(float &red, float &green, float &blue);

    void setMouseX(float mouseX) { mouseX_ = mouseX; }
    void setMouseY(float mouseY) { mouseY_ = mouseY; }
#endif

  private:
    GameLevel *currentLevel_, *storedLevel_;
    Camera *mainCamera_, *c1_, *c2_, *c3_, *c4_;
    KeyState keyState_, specialKeyState_;
    SoundFX soundFx_;

    GameTimer timer_;   // game's timer
    float timeElapsed_; // time elapsed in game
    float fps_;

    lua_State* luaState_;

    bool isRunning_, isIntroRunning_, isPaused_;

    float mouseX_, mouseY_;

    // Used through game for cel shading
    GLuint shaderTexture_[1];
    unsigned int lists_; // lists used for rendering

    void prepLevelLoad();

#if EDIT
    // Mouse control values
    float lastX, lastY;

    // Terrain editing values
    int last_x, last_z, last_type;
    float last_height;
    float last_red, last_green, last_blue;
    bool paint, paintColor;
#endif
};

#endif
