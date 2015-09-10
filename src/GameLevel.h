#include "constants.h"
#include "Object.h"
#include "Player.h"
#include "terrain.h"
#include "Camera.h"
#include "text.h"
#include "QuadTree.h"
#include "Music.h"
#include "ObjectList.h"

extern "C" {
  #include "lua5.1/lua.h"
  #include "lua5.1/lualib.h"
  #include "lua5.1/lauxlib.h"
}

#ifndef _GAME_LEVEL_H_
#define _GAME_LEVEL_H_

#define MAX_LEVEL_OBJECTS 1024

#define TYPE_GAME_OBJECT 1
#define TYPE_GAME_TEXT   2

class GameLevel {
  public:
    GameLevel(unsigned int);
    ~GameLevel();

    void updateLevel();
    void animateLevel();
    void prepareLevel(void);
    void drawLevel(void);

    bool loadLevel(const char* file);
    bool loadLevelFromBuffer(const char* buffer);
    void loadScript(string file);

    Object* findEnemyOfType(int type);
    float findDistance(Object*, Object*);

    void drawSky(void);
    bool checkComplete(void);
    void unloadLevel(void);
    void removeObjects(void);

    void postDraw();

    void updateTerrain(int &x, int &z, float &height, int &type, float &red, float &green, float &blue);
    void updateColor(int &x, int &z, float &red, float &green, float &blue);

    void getTerrainInfo(int &x, int &z, float &height, int &type, float &red, float &green, float &blue);
    void saveTerrain(const char*);
    void toggleGrid(void);
    void toggleBoundingBoxes(void);
    void toggleControlTriangles(void);

    void drawObjects();
    void drawObjectOutlines();
    void drawShadows(Vector*);

    void updateObjects(Vector*);
    void prepareObjects();
    void animateObjects(float);

    Object* addObject(string script, lua_State* luaState, unsigned int type);

    ObjectList* getObjects()       { return &objects_; }
    Terrain* getTerrain(void)      { return((Terrain*)terrain_); }
    Camera* getCamera(void)        { return((Camera*)camera_); }
    Player* getPlayer(void)        { return((Player*)player_); }
    Music* getMusic()              { return &music_; }
    void setCamera(Camera* camera) {
      camera_           = camera;
      idToObjectMap_[0] = (Object*)camera_;
      camera_->setGameLevelId(0);
    }

    void setId(int id) { id_ = id; }
    int getId() { return id_; }

    float getElapsedTime() { return elapsedTime_; }
    void setElapsedTime(float elapsedTime) { elapsedTime_ = elapsedTime; }

    void setComplete(bool isComplete) { isComplete_ = isComplete; }
    string getMusicPath() { return musicPath_; }
    void setMusicPath(string musicPath) { musicPath_ = musicPath; }
    Vector* getLightDirection() { return &lightDirection_; }

    void setSkyBox(float[][3], int, int);
    void setLightDirection(float x, float y, float z) {
      lightDirection_.setVector(x, y, z);
      lightDirection_.normalize();
    }

    Object* getObjectFromId(int id) {
      return (id >= 0 && id < MAX_LEVEL_OBJECTS ? idToObjectMap_[id] : NULL);
    }

  private:
    unsigned int lists_;  // display lists used for rendering

    Vector lightDirection_;

    Player* player_;     // player object
    Object* camera_;     // camera
    Terrain *terrain_;   // Terrain for this level (if one is loaded)

    Music music_;

    ObjectList objects_;
    map <string, ObjectList> freeObjects_;
    Object* idToObjectMap_[MAX_LEVEL_OBJECTS];
    unsigned int numObjects_;

    // colors for sky box
    float bgcolor_[3][3];

    bool isComplete_;

    float elapsedTime_;

    QuadTree quadTree_;
    DisplayList displayList_;

    bool grid_;
    bool bboxes_;
    bool controlTriangles_;

    string musicPath_;

    int id_;

    lua_State* luaState_;

    bool prepLevelLoad(const char* luaCode);
    bool finishLevelLoad();
};

#endif
