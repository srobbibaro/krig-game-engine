#include <cstdlib>
#include <string.h>
#include <dirent.h>

#include "Engine.h"
#include "Scripting.h"
#include "buffer.h"

extern const char* intro_script_buffer;

GameLevel *g_script_game_level;

const char* buffer1 = "function on_load()\nreturn\nend\nfunction on_update(elapsedTime)\nreturn\nend\nfunction on_unload()\nreturn\nend\n";

//------------------------------------------------------------------------------
Engine::Engine()
{
    // initialize resources used by the engine //
    keyState_.initKeyState();
    specialKeyState_.initKeyState();

    loadModels();

    currentLevel_ = NULL;
    storedLevel_ = NULL;

    // setup game timer /////////////////////////
    if( !timer_.init() ) {
        PRINT_ERROR("Timer initialization failed.");

        exit (1);
    }
    ///////////////////////////////////////////////

    fps_ = 0.0f;

    #if DEMO || PLAY_DEMO
        demo.open( "demo.txt" );
    #endif

    #if PLAY_DEMO
    int i = 0;
    currentDemoCommand = 0;

    while ( !demo.eof() && i < 512 ) {
        demo >> demoCommands[i].routine >> demoCommands[i].key >> demoCommands[i].time;
        i++;
    }
    #endif

#if EDIT
    // terrain editing values //
    paint = false;
    paintColor = false;

    lastX = 0.0f;
    lastY = 0.0f;

    last_x = 0;
    last_z = 0;
    last_red = 0.0f;
    last_green = 0.0f;
    last_blue = 0.0f;
    last_height = 0.0f;
    last_type = 0;
    ////////////////////////////
#endif

    luaState_ = NULL;
    mainCamera_ = c1_ = c2_ = c3_ = c4_ = NULL;

    isPaused_ = false;

    mouseX_ = 0.0f;
    mouseY_ = 0.0f;

    isRunning_ = true;
    isIntroRunning_ = false;
}

//------------------------------------------------------------------------------
bool Engine::loadGame( string file )
{
    g_script_engine = this;

    // If the lua state has not been initialized for this object, attempt to
    // initialize it.
    if (file == "" || luaState_ != NULL)
        return false;

    luaState_ = lua_open();

    // If the lua state still could not be initialized, then exit the game.
    // ... we can do something smarter with this in the finished product.
    if (!luaState_) {
        PRINT_ERROR("Could not create Lua state.\n");
	    exit(-1);
    }

	// load Lua base libraries
	luaL_openlibs(luaState_);

    // Register our functions for use in lua (currently defined in
    // Object.h)
    registerFunctions(luaState_, 0);

    // load the script
    PRINT_DEBUG("Loading Lua game script '%s'...\n", file.c_str());
	luaL_dofile(luaState_, file.c_str());

	// Find the update function and call it
    lua_getglobal(luaState_, "on_load");

    // Call the function with 1 argument and no return values
    lua_call(luaState_, 0, 0);

    return (true);
}

//------------------------------------------------------------------------------
bool Engine::loadGameFromBuffer( char* buffer )
{
    g_script_engine = this;

    // If the lua state has not been initialized for this object, attempt to
    // initialize it.
    if (buffer == "" || luaState_ != NULL)
        return false;

    luaState_ = lua_open();

    // If the lua state still could not be initialized, then exit the game.
    // ... we can do something smarter with this in the finished product.
    if (!luaState_) {
        PRINT_ERROR("Could not create Lua state.\n");
	    exit(-1);
    }

	// load Lua base libraries
	luaL_openlibs(luaState_);

    // Register our functions for use in lua (currently defined in
    // Object.h)
    registerFunctions(luaState_, 0);

    // load the script
    PRINT_DEBUG("Loading Lua game script from buffer...\n");

	//luaL_dofile(luaState_, file.c_str());
	luaL_loadbuffer(luaState_, buffer, strlen(buffer), "line") || lua_pcall(luaState_, 0, 0, 0);

	// Find the update function and call it
    lua_getglobal(luaState_, "on_load");

    // Call the function with 1 argument and no return values
    lua_call(luaState_, 0, 0);

    return (true);
}

bool Engine::loadIntroCredits()
{
    g_script_engine = this;

    // If the lua state has not been initialized for this object, attempt to
    // initialize it.
    if (luaState_ != NULL)
        return false;

    luaState_ = lua_open();

    // If the lua state still could not be initialized, then exit the game.
    // ... we can do something smarter with this in the finished product.
    if (!luaState_) {
        PRINT_ERROR("Could not create Lua state.\n");
	    exit(-1);
    }

	// load Lua base libraries
	luaL_openlibs(luaState_);

    // Register our functions for use in lua (currently defined in
    // Object.h)
    registerFunctions(luaState_, 0);

    // load the script
    PRINT_DEBUG("Loading Lua game script 'intro credits'...\n");

	//luaL_dofile(luaState_, file.c_str());

	luaL_loadbuffer(luaState_, buffer1, strlen(buffer1), "line") || lua_pcall(luaState_, 0, 0, 0);

	// Find the update function and call it
    //lua_getglobal(luaState_, "on_load");

    // Call the function with 1 argument and no return values
    //lua_call(luaState_, 0, 0);

    loadLevelFromBuffer(intro_script_buffer);
        //loadLevel("./levels/intro.lua");

    isIntroRunning_ = true;

    return (true);
}


//------------------------------------------------------------------------------
void Engine::updateGame(float elapsedTime)
{
    // Attempt to execute the script only if the lua state has already been
    // initialized with a script
    if (!luaState_)
        return;

    // Find the update function and call it
    lua_getglobal(luaState_, "on_update");

    // Push the time passed since the last iteration of the game loop
    lua_pushnumber(luaState_, elapsedTime);

    // Call the function with 2 argument and no return values
    lua_call(luaState_, 1, 0);

    // get the result //
    //position.z = (float)lua_tonumber(luaState_, -1);
    //position.y = (float)lua_tonumber(luaState_, -2);
    //position.x = (float)lua_tonumber(luaState_, -3);
    //lua_pop(luaState_, 1);
}

//------------------------------------------------------------------------------
void Engine::unloadGame()
{
    // Attempt to execute the on_unload function only if the lua state has
    // already been initialized with a script
    if (!luaState_)
        return;

    // Find the update function and call it
    lua_getglobal(luaState_, "on_unload");

    // Call the function with 1 argument and no return values
    lua_call(luaState_, 0, 0);

    // get the result //
    //position.z = (float)lua_tonumber(luaState_, -1);
    //position.y = (float)lua_tonumber(luaState_, -2);
    //position.x = (float)lua_tonumber(luaState_, -3);
    //lua_pop(luaState_, 1);

    lua_close(luaState_);
    luaState_ = NULL;
}

//------------------------------------------------------------------------------
void Engine::gameCycle()
{
    timeElapsed_ = timer_.getElapsedSeconds(1);
    updateGame(timeElapsed_);
    fps_ = timer_.getFPS();

    if (currentLevel_ != NULL && !isPaused_)
        currentLevel_->setElapsedTime(timeElapsed_);

    #if PLAY_DEMO
        totalTime += timeElapsed;

        while ( demoCommands[currentDemoCommand].time <= totalTime && currentDemoCommand < 512 ) {
            switch ( demoCommands[currentDemoCommand].routine )
            {
                case 1:
                    processKeyDown( demoCommands[currentDemoCommand].key );
                    break;
                case 2:
                    processNormalKey( (unsigned char)(demoCommands[currentDemoCommand].key) );
                    break;
                case 3:
                    processKeyUp( demoCommands[currentDemoCommand].key );
                    break;
            }
            currentDemoCommand++;
        }
    #endif

    if (currentLevel_ != NULL) {
        if ( currentLevel_->checkComplete() ) {
            if (isIntroRunning_) {
                unloadGame();
                loadGame("./scripts/main.lua");
                isIntroRunning_ = false;
            }
        }
        else {
            if (!isPaused_) {
                mainCamera_->update(currentLevel_->getElapsedTime());

                currentLevel_->animateLevel();

                //processCommands();



                currentLevel_->updateLevel();

                currentLevel_->prepareLevel();   // collision detection

                //currentLevel_->updateLevel();

                prepare();

                //Matrix m;
                //mainCamera_->worldRotation.buildRotationMatrix(m);

                //glMultMatrixf(m.data);
                currentLevel_->drawLevel();


                currentLevel_->getMusic()->Update();

                glClear( GL_DEPTH_BUFFER_BIT );
                glLoadIdentity();




                glDepthFunc(GL_ALWAYS);
                currentLevel_->postDraw();
                glDepthFunc(GL_LESS);

                glutSwapBuffers();
            }
        }
    }

    processCommands();
}

//------------------------------------------------------------------------------
void Engine::prepare()
{
    glClear( GL_DEPTH_BUFFER_BIT );
    glLoadIdentity();

    mainCamera_->prepareGLView();

    glDepthMask(GL_FALSE);
        currentLevel_->drawSky();
    glDepthMask(GL_TRUE);

    //glDepthFunc(GL_ALWAYS);
    //    currentLevel_->postDraw();
   // glDepthFunc(GL_LESS);

    Matrix translationMatrix;

    Vector position = mainCamera_->getPosition();

    translationMatrix.setTranslation(-position.x, -position.y, -position.z);

    glMultMatrixf(translationMatrix.data);
}

//------------------------------------------------------------------------------
void Engine::initGL()
{
    GLfloat shaderData[16] = {
        0.4f, 0.4f, 0.4f, 0.7f, 0.7f, 0.7f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
    };

    //glClearColor( 0.0f, 0.0f, 0.3f, 1.0f );
    glClearColor( 1.0f, 0.0f, 1.0f, 1.0f );

    glShadeModel( GL_SMOOTH );                  // use smooth shading
    glEnable( GL_DEPTH_TEST );                  // remove hidden surfaces

    // create 1D bitmap for lighting ////////////
    glGenTextures( 1, &shaderTexture_[0] );

    glBindTexture( GL_TEXTURE_1D, shaderTexture_[0] );
    glTexParameteri( GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTexParameteri( GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );

    glTexImage1D( GL_TEXTURE_1D, 0, 1, 16, 0, GL_LUMINANCE, GL_FLOAT, shaderData );
    /////////////////////////////////////////////

    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    glFrontFace( GL_CCW );
    glEnable( GL_CULL_FACE );
    glPolygonMode( GL_FRONT, GL_FILL );
    glEnable( GL_BLEND );

    // setup projection matrix //////////////////
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluPerspective ( 45.0 , (800.0f/600.0f) , 0.001f, 300.0f );
    glMatrixMode( GL_MODELVIEW );

    glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );
    glDisable( GL_LIGHTING );
    glBindTexture( GL_TEXTURE_1D, shaderTexture_[0] );
    glLineWidth( 1.0f );

    glDepthFunc( GL_LESS );

    // setup display lists //////////////////////
    lists_ = glGenLists( 3 );

    // list to setup cel shading ////////////////
    glNewList( lists_, GL_COMPILE );
        glDisable( GL_BLEND );
        //glEnable (GL_DEPTH_TEST);
        glDepthFunc( GL_LEQUAL );
        glEnable( GL_TEXTURE_1D );
        //glBindTexture( GL_TEXTURE_1D, shaderTexture[0] );
    	glLineWidth( 1.0f );
    glEndList();

    // list to setup to draw outline ////////////
    glNewList( lists_+1, GL_COMPILE );
        glDisable( GL_TEXTURE_1D );
        glEnable( GL_BLEND );
        glDepthFunc( GL_LESS );
        glLineWidth( 3.0f );
        glCullFace( GL_FRONT );
        glPolygonMode( GL_BACK, GL_LINE );
   glEndList();

   // list to reset display settings
   glNewList( lists_+2, GL_COMPILE );
        glPolygonMode( GL_BACK, GL_FILL );
        glDepthFunc( GL_LESS );
        glCullFace( GL_BACK );
    glEndList();

    glHint(GL_POLYGON_SMOOTH, GL_NICEST);
    glEnable(GL_POLYGON_SMOOTH);
    glEnable(GL_BLEND);
}

//------------------------------------------------------------------------------
void Engine::processKeyUp(int key)
{
    PRINT_DEBUG_LVL(1, "special key up (%d)\n", key);
    specialKeyState_.keys[key] = 2;

    #if DEMO
        if ( gameMode == 1 )
            demo << 3 << " " << key << " " << totalTime << endl;
    #endif
}
//------------------------------------------------------------------------------
void Engine::processKeyDown( int key )
{
    PRINT_DEBUG_LVL(1, "special key down (%d)\n", key);
    specialKeyState_.keys[key] = 1;


    #if DEMO
        if ( gameMode == 1 )
            demo << 1 << " " << key << " " << totalTime << endl;
    #endif
}

//------------------------------------------------------------------------------
void Engine::processCommands()
{
    keyState_.initKeyState();
    specialKeyState_.initKeyState();
    /*
    int cmd = control.deQueue();    // get command from queue

    while( cmd != NO_COMMAND )      // while while commands are defined,
    {                               // ie, the queue is not empty
        switch ( cmd )
        {
            case VEL_UP_KEY_DOWN:
            {
                 vMove += 1;
                 player->velocity_.y = mainCamera_->velocity_.y+10.0f;

                  Vector rotationAxis;

                    rotationAxis.crossProduct(player->up, player->direction);
                    rotationAxis.normalize();
                 Quaternion t;
                t.buildFromAxis(rotationAxis, -.045f);
                player->rotation = player->rotation * t;


                break;
            }
            case VEL_UP_KEY_UP:
            {
                vMove -= 1;
                if ( vMove < 0 )
                    vMove = 0;

                if ( vMove == 2 )
                    player->velocity_.y = mainCamera_->velocity_.y-10.0f;
                else {


                    player->velocity_.y = mainCamera_->velocity_.y;

                   player->velocity_.y = mainCamera_->velocity_.y;


                    Quaternion t;
                    t.buildFromEuler(0.0f, 1.57f, 0.0f);
                    player->rotation = t;
                }
                break;
            }
            case VEL_DOWN_KEY_DOWN:
            {
                vMove += 2;
                player->velocity_.y = mainCamera_->velocity_.y-10.0f;


                 Vector rotationAxis;

                    rotationAxis.crossProduct(player->up, player->direction);
                    rotationAxis.normalize();
                Quaternion t;
                t.buildFromAxis(rotationAxis, .045f);
                player->rotation = player->rotation * t;

                break;
            }
            case VEL_DOWN_KEY_UP:
            {
                vMove -= 2;
                if ( vMove < 0 )
                    vMove = 0;

                if ( vMove == 1 )
                    player->velocity_.y = mainCamera_->velocity_.y+10.0f;
                else {
                    player->velocity_.y = mainCamera_->velocity_.y;

                    Quaternion t;


                    t.buildFromEuler(0.0f, 1.57f, 0.0f);
                    player->rotation = t;
                }
                break;
            }

           case SHOOT_DOWN:
            {
                //(player)->fireShot(mainCamera_->velocity_.x);        // gameSpeed
                (player)->temp = new ScriptedObject("./scripts/player_shot.lua");
                (player)->temp->player = player;
                (player)->temp->camera = mainCamera_;
                (player)->temp->s = sounds;
                (player)->temp->loadScript("./scripts/player_shot.lua");
                (player)->add((player)->temp);

                 Vector tv;
                 bool hit = (player)->direction.intersectBox( (player)->position, (player)->collisionBox_, 1.0f, tv );

                if ( hit ) {
                    (player)->temp->setPosition( tv );
                }
                else {
                    (player)->temp->position = (player)->position;
                }

                break;
            }
            case MISSILE_DOWN:
                obj = currentLevel_->findEnemy();
                (player)->fireMissle( obj );

                break;

            case TOGGLE_MENU:
                control.emptyQueue();
                if ( gameMode == 1 )
                {
                    sounds->PauseSong();
                    sounds->PlaySFX("pause.wav");
                    MenuStr = "Resume";
                    gameMode = 0;

                    if ( numLives <= 0 )
                    {
                        numLives = 5;
                        MenuStr = "Restart";
                    }
                }
                else if ( gameMode == 2 )
                {
                    sounds->StopSong();
                    gameMode = 0;
                    lvlNum = 0;
                    newLevel();
                    //strcpy( MenuStr, "Restart" );
                }

                hMove = vMove = 0;
                player->setVelocity( mainCamera_->velocity_.x, mainCamera_->velocity_.y, mainCamera_->velocity_.z );
                break;

            case SELECT_OPTION:
                sounds->StopSong();
                vMove = hMove = 0;
                control.emptyQueue();

                if ( menuCursor ) {
                    gameMode = 1;
                    if (currentLevel_->getMusicPath() != "") {
                        sounds->PlaySong(currentLevel_->getMusicPath().c_str(), true);
                    }
                }
                else if ( !menuCursor )
                    control.enQueue( QUIT_GAME );

                break;

            case QUIT_GAME:
            #if DEMO
                demo.close();
            #endif
                //delete currentLevel_;
                exit(0);
                break;
        }
    }
    */
}
//------------------------------------------------------------------------------
void Engine::processNormalKey(unsigned char key)
{
    //key = toupper(key);

    keyState_.keys[key] = 1;

    PRINT_DEBUG_LVL(1, "key (%d)\n", key);
#if EDIT
    switch (key)
    {
        // Game controls...
        case 32:
            //if ((player)->userControl && gameMode == 1 && (player)->state != DEAD)
            //    control.enQueue(SHOOT_DOWN);

            #if DEMO
            if ( gameMode == 1 && player->state_ != DEAD )
                demo << 2 << " " << int(key) << " " << totalTime << endl;
            #endif
            break;
        case 'M':
        case 'm':
            //if ((player)->userControl && gameMode == 1 && (player)->state_ != DEAD)
            //    control.enQueue(MISSILE_DOWN);
            break;

        // Select current camera
        case '1':
            mainCamera_ = c1_;
            currentLevel_->setCamera(mainCamera_);
            break;
        case '2':
            mainCamera_ = c2_;
            currentLevel_->setCamera(mainCamera_);
            break;
        case '3':
            mainCamera_ = c3_;
            currentLevel_->setCamera(mainCamera_);
            break;
        case '4':
            mainCamera_ = c4_;
            currentLevel_->setCamera(mainCamera_);
            break;

        // Save terrain
        case 'O':
        case 'o':
            currentLevel_->saveTerrain("terrains/new_terrain.txt");
            break;

        // Toggle settings
        case 'G':
        case 'g':
            currentLevel_->toggleGrid();
            break;
        case 'B':
        case 'b':
            currentLevel_->toggleBoundingBoxes();
            break;
        case 'C':
        case 'c':
            currentLevel_->toggleControlTriangles();
            break;

        case '.':
            // used for profiling
            exit(0);
            break;
    }
#endif
}

//------------------------------------------------------------------------------
void Engine::loadLevel(const char* levelFile)
{
    levelScript_ = levelFile;

    if (currentLevel_)
        delete currentLevel_;

    if (!mainCamera_) {
        // setup the current level... this really ought to be moved.
        //mainCamera_ = new Camera();
        c1_ = new Camera(1);
        c2_ = new Camera(2);
        c3_ = new Camera(3);
        c4_ = new Camera(4);

        mainCamera_ = c1_;
        c2_->setPosition(0.0f, 100.0f, 0.0f);
        c2_->setRotationEuler(1.57, 0.0f, 0.0f);
        c3_->setPosition(0.0f, 0.0f, 0.0f);
        c4_->setPosition(0.0f, 0.0f, 0.0f);
    }

    mainCamera_->init();
    mainCamera_->unloadScript();

    currentLevel_ = new GameLevel(lists_);
    currentLevel_->setCamera( mainCamera_ );

    g_script_game_level = currentLevel_;

    currentLevel_->loadLevelLua(levelScript_);
    timeElapsed_ = timer_.getElapsedSeconds(1);
}

void Engine::loadLevelFromBuffer(const char* buffer)
{
    //levelScript_ = levelFile;

    if (currentLevel_)
        delete currentLevel_;

    if (!mainCamera_) {
        // setup the current level... this really ought to be moved.
        //mainCamera_ = new Camera();
        c1_ = new Camera(1);
        c2_ = new Camera(2);
        c3_ = new Camera(3);
        c4_ = new Camera(4);

        mainCamera_ = c1_;
        c2_->setPosition(0.0f, 100.0f, 0.0f);
        c2_->setRotationEuler(1.57, 0.0f, 0.0f);
        c3_->setPosition(0.0f, 0.0f, 0.0f);
        c4_->setPosition(0.0f, 0.0f, 0.0f);
    }

    mainCamera_->init();
    mainCamera_->unloadScript();

    currentLevel_ = new GameLevel(lists_);
    currentLevel_->setCamera( mainCamera_ );

    g_script_game_level = currentLevel_;

    currentLevel_->loadLevelFromBufferLua(buffer);
    timeElapsed_ = timer_.getElapsedSeconds(1);

}

//------------------------------------------------------------------------------
void Engine::loadModels()
{
    DIR *dir;
    struct dirent *de;
    ModelStorage *model;
    char filePath[MAX_PATH_LEN];
    string hashKey;

    dir = opendir("./models/");

    while((de = readdir(dir)) != NULL ) {
        // only consider model files with the .mdl extension //
        if( strstr(de->d_name, ".mdl") != NULL ) {
            // build full path to load //
            strcpy(filePath, "./models/");
            strcat(filePath, de->d_name);

            // load model file into model storage //
            PRINT_DEBUG("Loading model file '%s'...\n", filePath);

            model = new ModelStorage();
            model->load(filePath);

            // TODO: Investigate progress on "shadows" feature
            //model->buildEdges();

            // insert model file into model hash //
            hashKey = string(de->d_name);
            modelHash[hashKey] = model;

            PRINT_DEBUG("done.\n");
        }
    }

    closedir(dir);
}

void Engine::processMouseMove(int x, int y)
{
    mouseX_ = x;
    mouseY_ = y;
}

#if EDIT
void Engine::getTerrainInfo(int &x, int &z, float &height, int &type, float &red, float &green, float &blue)
{
    if (mainCamera_ == NULL)
        return;

    Vector position = c2_->getPosition();

    x = (int)(position.x /5.0f);
    z = -(int)(position.z /5.0f);
    currentLevel_->getTerrainInfo(x,z,height,type,red,green,blue);
}

void Engine::updateTerrain(int &x, int &z, float &height, int &type, float &red, float &green, float &blue)
{
    if (!mainCamera_)
        return;

    if (mainCamera_->id_ == 2 || mainCamera_->id_ == 3) {
        Vector position = c2_->getPosition();
        x = (int)(position.x /5.0f);
        z = -(int)(position.z /5.0f);
        currentLevel_->updateTerrain(x,z,height,type,red,green,blue);

        last_x = x;
        last_z = z;
        last_type = type;
        last_red = red;
        last_green = green;
        last_blue = blue;
        last_height = height;
    }
}

void Engine::updateColor(float &red, float &green, float &blue)
{
    if (!mainCamera_)
        return;

    if (mainCamera_->id_ == 2 || mainCamera_->id_ == 3) {
        Vector position = c2_->getPosition();

        int x = (int)(position.x /5.0f);
        int z = -(int)(position.z /5.0f);
        currentLevel_->updateColor(x, z, red,green,blue);

        last_red = red;
        last_green = green;
        last_blue = blue;
    }
}
#endif

void Engine::shutdown()
{
    isRunning_ = false;
}

Engine::~Engine()
{
    unloadGame();

    if (currentLevel_)
        delete currentLevel_;

    if (storedLevel_)
        delete storedLevel_;

    if (mainCamera_) {
        delete c1_;
        delete c2_;
        delete c3_;
        delete c4_;
    }

    currentLevel_ = storedLevel_ = NULL;
    c1_ = c2_ = c3_ = c4_ = mainCamera_ = NULL;
}

void Engine::pause()
{
    isPaused_ = !isPaused_;
}
