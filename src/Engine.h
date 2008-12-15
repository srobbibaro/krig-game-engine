#include "constants.h"
#include "GameLevel.h"

extern "C" {
    #include "lua/lua.h"
    #include "lua/lualib.h"
    #include "lua/lauxlib.h"
}

// Remove from final build
#include <vector>

using namespace std;

#ifndef _ENGINE_H_
#define _ENGINE_H_

class Engine
{
    private:
        GameLevel* currentLevel;    // current level
        GameTimer* timer;           // game's timer
        Camera* mainCamera;         // main camera used in level
        
        Camera* c1, *c2, *c3, *c4;
        
        int cam;
        
        EventQueue control;         // queue for handeling input
        Credits credits;
        Menu menu;
        int gameMode;               // current gameMode
        float timeElapsed;          // time elapsed in game
        float totalTime;
        unsigned int lists;         // lists used for rendering
       
        KeyState *keyState;

        char *MenuStr;
        
        // cell shading global variables ////////
        Matrix tempMatrix;
        GLuint shaderTexture[1];
        
        // Sound class ///////
        Sound *sounds;
        
        bool menuCursor;
            
        #if DEMO
        ofstream demo;
        #endif
        
        #if PLAY_DEMO
        ifstream demo;
        
        struct DemoCommand {
            int routine;
            int key;
            float time;
        } demoCommands[512];
        
        int currentDemoCommand;    
        #endif
        
        // Temporary values used for game tools (remove from final build)...
        // mouse control values
        float lastX;
        float lastY;
        
        // Terrain editing values
        int last_x, last_z, last_type;
        float last_height, last_red, last_green, last_blue;
        bool paint;
        bool paintColor;
        
        string levelScript;
        
        Vector* light;
        
        lua_State* L;  
    
    public:
        Engine();                   // initialize game
        void gameCycle(void);           // physics, animation, collision, draw
        void prepare(void);
        void initGL(void);
        void processKeyUp( int );
        void processKeyDown( int );
        void processCommands();
        void displayHUD( float, int, float, long );
        void processNormalKey( unsigned char );
        void loadLevel(const char*);
        void loadModels(void);
        void displayDebug(void);
        void processMouseMove( int x, int y);
        
        
        // Temporary functions used for tools (remove from final build)...
        void getTerrainInfo(int &x, int &z, float &height, int &type, float &red, float &green, float &blue);
        
        
        
        
        void updateTerrain(int &x, int &z, float &height, int &type, float &red, float &green, float &blue);
        void updateColor(float &red, float &green, float &blue);
        
        void setLevelScript(string level) 
        {
            levelScript = level;    
        }
        
        bool loadGame(string);
        void updateGame(float);
        void unloadGame();
        void shutdown();
        
        KeyState* getKeyState() { return keyState; }
};


#endif
