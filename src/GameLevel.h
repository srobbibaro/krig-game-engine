extern "C" {
    #include "lua/lua.h"
    #include "lua/lualib.h"
    #include "lua/lauxlib.h"
}

#include "constants.h"
#include "Objects.h"
#include "Player.h"
#include "Terrain.h"
#include "Boss.h"
#include "Camera.h"
#include "ParticleSystem.h"
#include "Text.h"
#include "EnemyShip.h"
#include "Powerup.h"
#include "BoatCannon.h"
#include "SailBoat.h"
#include "Asteroid.h"
#include "Beam.h"
#include "QuadTree.h"

#ifndef _GAME_LEVEL_H_
#define _GAME_LEVEL_H_

class GameLevel
{
    private:
        Terrain* terrain;    // list of objects starting with terrain
        unsigned int lists; // display lists used for rendering
        Object* obj;        // temp object for loading
        Vector *light;
        Player* player;     // player object
        Boss* boss;         // pointer to boss object
        ScriptedObject* camera;     // camera
        Sound* snd;         // pointer to sound class
        float bgcolor[3][3];
        int weatherEffect;
        ParticleSystem *weather;
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
    
    
    public:
        GameLevel( unsigned int, Vector* );
        ~GameLevel();
        void updateLevel( Vector* );
        void animateLevel( float );
        void prepareLevel( void);
        void drawLevel( void );
        bool loadLevel( string file );
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
        Boss* returnBoss ( void );
        void drawSky( void );
        bool checkComplete( void );
        void unloadLevel(void);
        void removePlayer( void );
        void processScripts( void );
        
        void animateText( float );
        void drawText( void );
        
        void loadVector(lua_State* L, Vector* v);
        void loadObject(lua_State* L, int number);
        
        // remove this from the final build //
        void updateTerrain(int &x, int &z, float &height, int &type, float &red, float &green, float &blue);
        void updateColor(int &x, int &z, float &red, float &green, float &blue);
        
        void getTerrainInfo(int &x, int &z, float &height, int &type, float &red, float &green, float &blue);
        void saveTerrain(char*);
        void toggleGrid(void);
        void toggleBoundingBoxes(void);
        
        string musicPath;
};

#endif
