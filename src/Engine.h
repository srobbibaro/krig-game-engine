#include "constants.h"
#include "GameLevel.h"

extern "C" {
    #include "lua/lua.h"
    #include "lua/lualib.h"
    #include "lua/lauxlib.h"
}

using namespace std;

#ifndef _ENGINE_H_
#define _ENGINE_H_

class Engine
{
    private:
        GameLevel *currentLevel, *storedLevel;
        GameTimer* timer;           // game's timer
        Camera *mainCamera, *c1, *c2, *c3, *c4;
        
        float timeElapsed;          // time elapsed in game
        unsigned int lists;         // lists used for rendering
       
        KeyState *keyState;
        
        // cell shading global variables ////////
        GLuint shaderTexture[1];
        
        SoundFX *soundFx_;
        
        string levelScript;
        
        lua_State* L;  
        
        bool isPaused;
        float fps;
            
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
        Vector* light;
    
    public:
        Engine();                   // initialize game
        void gameCycle(void);           // physics, animation, collision, draw
        void prepare(void);
        void initGL(void);
        void processKeyUp( int );
        void processKeyDown( int );
        void processCommands();
        void processNormalKey( unsigned char );
        void loadLevel(const char*);
        
        void setLevelScript(string level) 
        {
            levelScript = level;    
        }
        
        bool loadGame(string);
        void updateGame(float);
        void unloadGame();
        
        void shutdown();
        void pause();
        
        float getFps() { return fps; }
        
        KeyState* getKeyState() { return keyState; }
        
        void loadModels(void);
        
        void renderText(const char* s, float x, float y) 
        {
            glRasterPos2f (x, y);
            render_string(GLUT_BITMAP_HELVETICA_18, s);
        }
        
        // Temporary functions used for tools (remove from final build)...
        void processMouseMove( int x, int y);
        
        void getTerrainInfo(int &x, int &z, float &height, int &type, float &red, float &green, float &blue);
        void updateTerrain(int &x, int &z, float &height, int &type, float &red, float &green, float &blue);
    
        void updateColor(float &red, float &green, float &blue);      
        
        void incTerrainHeight(float inc)
        {
            int skip = 0;
            int xpos, zpos, type;
            float height, red, green, blue;
    
            getTerrainInfo(xpos, zpos, height, type, red, green, blue);
            
            //# hack...
            if (inc > 999)
                height = 0.0f;
            else
                height += inc;
            
            updateTerrain(xpos,zpos,height, type, red, green, blue);
            return;
        }
        
        void swapLevel()
        {
            GameLevel *temp = currentLevel;
            currentLevel = storedLevel;
            storedLevel = temp;
        }
        
        SoundFX* getSoundFxClass() { return soundFx_; }
};

#endif
