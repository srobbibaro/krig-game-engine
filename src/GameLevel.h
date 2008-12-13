extern "C" {
    #include "lua/lua.h"
    #include "lua/lualib.h"
    #include "lua/lauxlib.h"
}

#include "constants.h"
#include "Object.h"
#include "Player.h"
#include "Terrain.h"
#include "Camera.h"
#include "Text.h"
#include "QuadTree.h"
#include "sound.h"

#ifndef _GAME_LEVEL_H_
#define _GAME_LEVEL_H_

class GameLevel
{
    private:
        Terrain* terrain;    // list of objects starting with terrain
        unsigned int lists;  // display lists used for rendering
        Object* obj;         // temp object for loading
        Vector light;
        Player* player;     // player object
        //Boss* boss;         // pointer to boss object
        Object* camera;     // camera
        Sound* snd;         // pointer to sound class
        float bgcolor[3][3];
        bool complete;
        
        float time;             // current time elapsed since level began running
        float eventBeginTime;   // beginning time of event
        
        // script information //
        float scriptDuration;   // duration of script
        //ScriptTextType *scriptText;
        //Script* textScript;
        
        int numTextStrings;
        int numTextScripts;
        /////////////////////////    
        
        QuadTree* q;
        DisplayList *l; 
        
        bool grid;
        bool bboxes;
        bool controlTriangles;     
        
        string musicPath;  
        
        KeyState* keyState;
        
        lua_State* L;
    
    
    public:
        GameLevel( unsigned int);
        ~GameLevel();
        void updateLevel(float);
        void animateLevel( float );
        void prepareLevel( void);
        void drawLevel( void );
        bool loadLevelLua( string file );
        void loadScript( string file );
        void setPlayer( Player* tPlayer );
        void setTerrain( Terrain* tTerrain );
        void setCamera( Camera* tCamera );
        Terrain* getTerrain( void );
        Camera* getCamera( void );
        Player* getPlayer( void );
        void setSoundClass( Sound * );
        Object* findEnemy( void );
        float findDistance( Object*, Object* );
        //Boss* returnBoss ( void );
        void drawSky( void );
        bool checkComplete( void );
        void unloadLevel(void);
        void removePlayer( void );
        void processScripts( void );
        
        void animateText( float );
        void drawText( void );
        
        void loadObject(lua_State* L, int number);
        
        // remove this from the final build //
        void updateTerrain(int &x, int &z, float &height, int &type, float &red, float &green, float &blue);
        void updateColor(int &x, int &z, float &red, float &green, float &blue);
        
        void getTerrainInfo(int &x, int &z, float &height, int &type, float &red, float &green, float &blue);
        void saveTerrain(char*);
        void toggleGrid(void);
        void toggleBoundingBoxes(void);
        void toggleControlTriangles(void);
        
        string getMusicPath() { return (musicPath); }
        void setMusicPath(string music) { musicPath = music; }
        Vector* getLight() { return &light; }
        
        void setSkyBox(float**, int, int);
        void setLightDirection(float x, float y, float z)
        {
            light.setVector(x, y, z);
            light.normalize();
        }
                
        Sound* getSoundClass() { return snd; }
        
        void setKeyState(KeyState* ks) { keyState = ks; }
        KeyState* getKeyState() { return keyState; }
};

#endif
