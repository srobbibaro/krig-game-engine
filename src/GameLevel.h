#include "constants.h"
#include "Object.h"
#include "Player.h"
#include "terrain.h"
#include "Camera.h"
#include "text.h"
#include "QuadTree.h"
#include "Music.h"

extern "C" {
    #include "lua.h"
    #include "lualib.h"
    #include "lauxlib.h"
}

#ifndef _GAME_LEVEL_H_
#define _GAME_LEVEL_H_

class GameLevel
{
    private:
        Terrain* terrain;    // list of objects starting with terrain
        unsigned int lists;  // display lists used for rendering

        Vector lightDirection_;

        Player* player;     // player object
        Object* camera;     // camera

        Music *music_;

        // colors for sky box
        float bgcolor[3][3];

        bool isComplete_;

        float elapsedTime;

        // script information //
        //ScriptTextType *scriptText;
        //Script* textScript;

        int numTextStrings;
        int numTextScripts;
        /////////////////////////

        QuadTree* quadTree_;
        DisplayList *displayList_;

        bool grid;
        bool bboxes;
        bool controlTriangles;

        string musicPath;

        string scriptName;

        int id;

        lua_State* L;

    public:
        GameLevel( unsigned int);
        ~GameLevel();
        void updateLevel();
        void animateLevel();
        void prepareLevel( void);
        void drawLevel( void );
        bool loadLevelLua( string file );
        void loadScript( string file );
        void setCamera( Camera* tCamera );
        Terrain* getTerrain( void );
        Camera* getCamera( void );
        Player* getPlayer( void );
        Object* findEnemy( void );
        float findDistance( Object*, Object* );

        void drawSky( void );
        bool checkComplete( void );
        void unloadLevel(void);
        void removeObjects( void );

        string getMusicPath() { return (musicPath); }
        void setMusicPath(string music) { musicPath = music; }
        Vector* getLightDirection() { return &lightDirection_; }

        void setSkyBox(float**, int, int);
        void setLightDirection(float x, float y, float z)
        {
            lightDirection_.setVector(x, y, z);
            lightDirection_.normalize();
        }

        Music* getMusic() { return music_; }

        void setId(int tid) { id = tid; }
        int getId() { return id; }

        float getElapsedTime() { return elapsedTime; }
        void setElapsedTime(float tElapsedTime) { elapsedTime = tElapsedTime; }

        void setComplete(bool isComplete) { isComplete_ = isComplete; }
        string getScriptName() { return scriptName; }

        void postDraw();

        // remove this from the final build //
        void updateTerrain(int &x, int &z, float &height, int &type, float &red, float &green, float &blue);
        void updateColor(int &x, int &z, float &red, float &green, float &blue);

        void getTerrainInfo(int &x, int &z, float &height, int &type, float &red, float &green, float &blue);
        void saveTerrain(char*);
        void toggleGrid(void);
        void toggleBoundingBoxes(void);
        void toggleControlTriangles(void);
};

#endif
