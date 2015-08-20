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

class GameLevel {
  public:
    GameLevel(unsigned int);
    ~GameLevel();

    void updateLevel();
    void animateLevel();
    void prepareLevel(void);
    void drawLevel(void);

    bool loadLevelLua(string file);
    bool loadLevelFromBufferLua(const char* buffer);
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

    ScriptedObject* addObject(string script, float args[], int n) {
      ScriptedObject *temp = static_cast<ScriptedObject*> (freeObjects_[script].head);

      if (temp != NULL) {
        temp->initSettings();
        freeObjects_[script].remove(temp);
        PRINT_DEBUG_LVL(
          2,
          "Object removed: script='%s'. Added to free objects map (size=%d).\n",
          script.c_str(),
          freeObjects_[script].size
        );
      }
      else {
        temp = new ScriptedObject();
        temp->setScript(script);

        PRINT_DEBUG_LVL(2, "Allocated a new object of type '%s'.\n", script.c_str());
      }

      objects_.insertFront(temp);
      //temp->unloadScript();

      temp->loadScript(script, args, n);

      return temp;
    }

    ScriptTextType* addScriptTextType(string script, float args[], int n) {
      ScriptTextType *temp = static_cast<ScriptTextType*> (freeObjects_[script].head);

      if (temp != NULL) {
        temp->initSettings();
        freeObjects_[script].remove(temp);

        PRINT_DEBUG_LVL(
          2,
          "Pulled object of type '%s' from FOM. (FOM size=%d).\n",
          script.c_str(),
          freeObjects_[script].size
        );
      }
      else {
        temp = new ScriptTextType();
        temp->setScript(script);

        PRINT_DEBUG_LVL(2, "Allocated a new object of type '%s'.\n", script.c_str());
      }

      objects_.insertFront(temp);
      //temp->unloadScript();
      temp->loadScript(script, args, n);

      return temp;
    }

    ObjectList* getObjects() { return &objects_; }
    void setCamera( Camera* tCamera );
    Terrain* getTerrain(void);
    Camera* getCamera(void);
    Player* getPlayer(void);
    Music* getMusic() { return &music_; }

    void setId(int id) { id_ = id; }
    int getId() { return id_; }

    float getElapsedTime() { return elapsedTime_; }
    void setElapsedTime(float elapsedTime) { elapsedTime_ = elapsedTime; }

    void setComplete(bool isComplete) { isComplete_ = isComplete; }
    string getScriptName() { return scriptName_; }
    string getMusicPath() { return musicPath_; }
    void setMusicPath(string musicPath) { musicPath_ = musicPath; }
    Vector* getLightDirection() { return &lightDirection_; }

    void setSkyBox(float[][3], int, int);
    void setLightDirection(float x, float y, float z) {
      lightDirection_.setVector(x, y, z);
      lightDirection_.normalize();
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

    string scriptName_;

    int id_;

    lua_State* luaState_;
};

#endif
