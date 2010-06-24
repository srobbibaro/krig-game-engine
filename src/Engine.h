#include "constants.h"
#include "GameLevel.h"

extern "C" {
    #include "lua.h"
    #include "lualib.h"
    #include "lauxlib.h"
}

using namespace std;

#ifndef _ENGINE_H_
#define _ENGINE_H_

class Engine
{
    private:
        GameLevel *currentLevel_, *storedLevel_;
        GameTimer timer_;           // game's timer
        Camera *mainCamera_, *c1_, *c2_, *c3_, *c4_;

        float timeElapsed_;          // time elapsed in game
        unsigned int lists_;         // lists used for rendering

        KeyState keyState_;
        KeyState specialKeyState_;

        // cell shading global variables ////////
        GLuint shaderTexture_[1];

        SoundFX soundFx_;

        string levelScript_;

        lua_State* luaState_;

        float mouseX_, mouseY_;

        bool isPaused_;
        float fps_;

        bool isRunning_;

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

    public:
        Engine();                   // initialize game
        ~Engine();
        void gameCycle(void);           // physics, animation, collision, draw
        void prepare(void);
        void initGL(void);
        void processKeyUp( int );
        void processKeyDown( int );
        void processCommands();
        void processNormalKey( unsigned char );
        void loadLevel(const char*);

        void setLevelScript(string levelScript)
        {
            levelScript_ = levelScript;
        }

        bool loadGame(string);
        void updateGame(float);
        void unloadGame();

        void shutdown();
        void pause();

        float getFps() { return fps_; }

        KeyState* getKeyState() { return &keyState_; }
        KeyState* getSpecialKeyState() { return &specialKeyState_; }

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

        void swapLevel()
        {
            GameLevel *temp = currentLevel_;
            currentLevel_ = storedLevel_;
            storedLevel_ = temp;
        }

        SoundFX* getSoundFxClass() { return &soundFx_; }

        void setMouseX( float mouseX ) { mouseX_ = mouseX;  }
        void setMouseY( float mouseY ) { mouseY_ = mouseY;  }
        float getMouseX() { return mouseX_; }
        float getMouseY() { return mouseY_; }

        bool getIsRunning() {return isRunning_;}
};

#endif
