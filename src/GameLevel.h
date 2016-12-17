#ifndef _GAME_LEVEL_H_
#define _GAME_LEVEL_H_

#include "ObjectList.h"
#include "Object.h"
#include "ModelGameObject.h"
#include "Terrain.h"
#include "Camera.h"
#include "QuadTree.h"
#include "Music.h"

extern "C" {
  #include "lua5.1/lua.h"
  #include "lua5.1/lualib.h"
  #include "lua5.1/lauxlib.h"
}

#define MAX_LEVEL_OBJECTS 1024

#define TYPE_GAME_OBJECT 1
#define TYPE_GAME_TEXT   2
#define TYPE_GAME_SPRITE 3

class GameLevel {
  public:
    GameLevel(const unsigned int&);
    ~GameLevel();

    void updateLevel();
    void animateLevel();
    void prepareLevel();
    void drawLevel();

    bool loadLevel(const char*);
    bool loadLevelFromBuffer(const char*);
    void loadScript(const std::string &);

    Object* findEnemyOfType(const int&);
    float findDistance(Object*, Object*);

    void drawSky();
    bool checkComplete();
    void unloadLevel();
    void removeObjects();

    void postDraw();

    void updateTerrain(int &x, int &z, float &height, int &type, float &red, float &green, float &blue);
    void updateColor(int &x, int &z, float &red, float &green, float &blue);

    void getTerrainInfo(int &x, int &z, float &height, int &type, float &red, float &green, float &blue);
    void saveTerrain(const char*);
    void toggleGrid();
    void toggleBoundingBoxes();
    void toggleControlTriangles();

    void drawObjects();
    void drawObjectOutlines();
    void drawShadows(Vector*);

    void updateObjects(Vector*);
    void prepareObjects();
    void animateObjects(const float&);

    Object* addObject(const std::string&, lua_State*, const unsigned int&);

    ObjectList* getObjects()         { return &objects_; }
    Terrain* getTerrain()            { return((Terrain*)terrain_); }
    Camera* getCamera()              { return((Camera*)camera_); }
    ModelGameObject* getPlayer()     { return((ModelGameObject*)player_); }
    Music* getMusic()                { return &music_; }
    void setCamera(Camera* camera)   {
      camera_           = camera;
      idToObjectMap_[0] = (Object*)camera_;
      camera_->setGameLevelId(0);
    }

    void setId(const int &id) { id_ = id; }
    int getId()               { return id_; }

    float getElapsedTime()                        { return elapsedTime_; }
    void setElapsedTime(const float &elapsedTime) { elapsedTime_ = elapsedTime; }

    void setComplete(const bool &isComplete)        { isComplete_ = isComplete; }
    std::string getMusicPath()                      { return musicPath_; }
    void setMusicPath(const std::string &musicPath) { musicPath_ = musicPath; }
    Vector* getLightDirection()                     { return &lightDirection_; }

    void setSkyBox(float[][3], const int&, const int&);
    void setLightDirection(const float &x, const float &y, const float &z) {
      lightDirection_.setVector(x, y, z);
      lightDirection_.normalize();
    }

    Object* getObjectFromId(const int &id) {
      return (id >= 0 && id < MAX_LEVEL_OBJECTS ? idToObjectMap_[id] : NULL);
    }

  private:
    unsigned int lists_;      // display lists used for rendering

    Vector lightDirection_;

    ModelGameObject* player_; // player object
    Object* camera_;          // camera
    Terrain *terrain_;        // Terrain for this level (if one is loaded)

    Music music_;

    ObjectList objects_;
    std::map <std::string, ObjectList> freeObjects_;
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

    std::string musicPath_;

    int id_;

    lua_State* luaState_;

    bool prepLevelLoad(const char* luaCode);
    bool finishLevelLoad();
};

#endif
