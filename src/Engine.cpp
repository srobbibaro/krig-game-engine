#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#include "Engine.h"
#include "Scripting.h"

GameLevel *lgameLevel;

//------------------------------------------------------------------------------
Engine::Engine()
{
    // initialize resources used by the engine //
    keyState = new KeyState();
    keyState->initKeyState();

    soundFx_ = new SoundFX();
    loadModels();

    currentLevel = NULL;
    storedLevel = NULL;

    // setup game timer /////////////////////////
    timer = new GameTimer;
    if( !timer->init() ) {
        printf ( "Timer initialization failed." );
        exit (1);
    }
    ///////////////////////////////////////////////

    fps = 0.0f;

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

    L = NULL;
    mainCamera = c1 = c2 = c3 = c4 = NULL;

    isPaused = false;
}

//------------------------------------------------------------------------------
bool Engine::loadGame( string file )
{
    lengine = this;

    // If the lua state has not been initialized for this object, attempt to
    // initialize it.
    if (file == "" || L != NULL)
        return false;

    L = lua_open();

    // If the lua state still could not be initialized, then exit the game.
    // ... we can do something smarter with this in the finished product.
    if (L == NULL) {
        printf("Error creating Lua state.\n");
	    exit(-1);
    }

	// load Lua base libraries
	luaL_openlibs(L);

    // Register our functions for use in lua (currently defined in
    // Object.h)
    registerFunctions(L, 0);

    // load the script
    cout << "Loading Lua script (game file): --" << file << "--\n";
	luaL_dofile(L, file.c_str());

	// Find the update function and call it
    lua_getglobal(L, "on_load");

    // Call the function with 1 argument and no return values
    lua_call(L, 0, 0);

    return (true);
}

//------------------------------------------------------------------------------
void Engine::updateGame(float elapsedTime)
{
    // Attempt to execute the script only if the lua state has already been
    // initialized with a script
    if (L == NULL)
        return;

    // Find the update function and call it
    lua_getglobal(L, "on_update");

    // Push the time passed since the last iteration of the game loop
    lua_pushnumber(L, elapsedTime);

    // Call the function with 2 argument and no return values
    lua_call(L, 1, 0);

    // get the result //
    //position.z = (float)lua_tonumber(L, -1);
    //position.y = (float)lua_tonumber(L, -2);
    //position.x = (float)lua_tonumber(L, -3);
    //lua_pop(L, 1);
}

//------------------------------------------------------------------------------
void Engine::unloadGame()
{
    // Attempt to execute the on_unload function only if the lua state has
    // already been initialized with a script
    if (L == NULL)
        return;

    // Find the update function and call it
    lua_getglobal(L, "on_unload");

    // Call the function with 1 argument and no return values
    lua_call(L, 0, 0);

    // get the result //
    //position.z = (float)lua_tonumber(L, -1);
    //position.y = (float)lua_tonumber(L, -2);
    //position.x = (float)lua_tonumber(L, -3);
    //lua_pop(L, 1);

    lua_close(L);
    L = NULL;
}

//------------------------------------------------------------------------------
void Engine::gameCycle()
{
    timeElapsed = timer->getElapsedSeconds(1);
    updateGame(timeElapsed);
    fps = timer->getFPS();

    if (currentLevel != NULL && !isPaused)
        currentLevel->setElapsedTime(timeElapsed);

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

    if (currentLevel != NULL) {
        if ( currentLevel->checkComplete() ) {}
        else {
            if (!isPaused) {
                mainCamera->update(currentLevel->getElapsedTime());

                currentLevel->animateLevel();

                //processCommands();

                currentLevel->updateLevel();

                currentLevel->prepareLevel();   // collision detection

                //currentLevel->updateLevel();

                prepare();

                currentLevel->drawLevel();

                currentLevel->getMusic()->Update();
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

    mainCamera->prepareGLView();

    glDepthMask(GL_FALSE);
        currentLevel->drawSky();
    glDepthMask(GL_TRUE);

    glDepthFunc(GL_ALWAYS);
        currentLevel->postDraw();
    glDepthFunc(GL_LESS);

    Matrix translationMatrix;
    translationMatrix.setTranslation(-mainCamera->position.x, -mainCamera->position.y, -mainCamera->position.z);

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
    glGenTextures( 1, &shaderTexture[0] );

    glBindTexture( GL_TEXTURE_1D, shaderTexture[0] );
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
    glBindTexture( GL_TEXTURE_1D, shaderTexture[0] );
    glLineWidth( 1.0f );

    glDepthFunc( GL_LESS );

    // setup display lists //////////////////////
    lists = glGenLists( 3 );

    // list to setup cel shading ////////////////
    glNewList( lists, GL_COMPILE );
        glDisable( GL_BLEND );
        //glEnable (GL_DEPTH_TEST);
        glDepthFunc( GL_LEQUAL );
        glEnable( GL_TEXTURE_1D );
        //glBindTexture( GL_TEXTURE_1D, shaderTexture[0] );
    	glLineWidth( 1.0f );
    glEndList();

    // list to setup to draw outline ////////////
    glNewList( lists+1, GL_COMPILE );
        glDisable( GL_TEXTURE_1D );
        glEnable( GL_BLEND );
        glDepthFunc( GL_LESS );
        glLineWidth( 3.0f );
        glCullFace( GL_FRONT );
        glPolygonMode( GL_BACK, GL_LINE );
   glEndList();

   // list to reset display settings
   glNewList( lists+2, GL_COMPILE );
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
    cout << "key up=" << key << endl;
    keyState->keys[key] = 2;

    #if DEMO
        if ( gameMode == 1 )
            demo << 3 << " " << key << " " << totalTime << endl;
    #endif
}
//------------------------------------------------------------------------------
void Engine::processKeyDown( int key )
{
    cout << "key down=" << key << endl;
    keyState->keys[key] = 1;

    #if DEMO
        if ( gameMode == 1 )
            demo << 1 << " " << key << " " << totalTime << endl;
    #endif
}

//------------------------------------------------------------------------------
void Engine::processCommands()
{
    keyState->initKeyState();
    /*
    int cmd = control.deQueue();    // get command from queue

    while( cmd != NO_COMMAND )      // while while commands are defined,
    {                               // ie, the queue is not empty
        switch ( cmd )
        {
            case VEL_UP_KEY_DOWN:
            {
                 vMove += 1;
                 player->velocity.y = mainCamera->velocity.y+10.0f;

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
                    player->velocity.y = mainCamera->velocity.y-10.0f;
                else {


                    player->velocity.y = mainCamera->velocity.y;

                   player->velocity.y = mainCamera->velocity.y;


                    Quaternion t;
                    t.buildFromEuler(0.0f, 1.57f, 0.0f);
                    player->rotation = t;
                }
                break;
            }
            case VEL_DOWN_KEY_DOWN:
            {
                vMove += 2;
                player->velocity.y = mainCamera->velocity.y-10.0f;


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
                    player->velocity.y = mainCamera->velocity.y+10.0f;
                else {
                    player->velocity.y = mainCamera->velocity.y;

                    Quaternion t;


                    t.buildFromEuler(0.0f, 1.57f, 0.0f);
                    player->rotation = t;
                }
                break;
            }

           case SHOOT_DOWN:
            {
                //(player)->fireShot(mainCamera->velocity.x);        // gameSpeed
                (player)->temp = new ScriptedObject("./scripts/player_shot.lua");
                (player)->temp->player = player;
                (player)->temp->camera = mainCamera;
                (player)->temp->s = sounds;
                (player)->temp->loadScript("./scripts/player_shot.lua");
                (player)->add((player)->temp);

                 Vector tv;
                 bool hit = (player)->direction.intersectBox( (player)->position, (player)->collisionBox, 1.0f, tv );

                if ( hit ) {
                    (player)->temp->setPosition( tv );
                }
                else {
                    (player)->temp->position = (player)->position;
                }

                break;
            }
            case MISSILE_DOWN:
                obj = currentLevel->findEnemy();
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
                player->setVelocity( mainCamera->velocity.x, mainCamera->velocity.y, mainCamera->velocity.z );
                break;

            case SELECT_OPTION:
                sounds->StopSong();
                vMove = hMove = 0;
                control.emptyQueue();

                if ( menuCursor ) {
                    gameMode = 1;
                    if (currentLevel->getMusicPath() != "") {
                        sounds->PlaySong(currentLevel->getMusicPath().c_str(), true);
                    }
                }
                else if ( !menuCursor )
                    control.enQueue( QUIT_GAME );

                break;

            case QUIT_GAME:
            #if DEMO
                demo.close();
            #endif
                //delete currentLevel;
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

    keyState->keys[key] = 1;

    cout << "key=" << key << endl;

    switch (key)
    {
        // Game controls...
        case 32:
            //if ((player)->userControl && gameMode == 1 && (player)->state != DEAD)
            //    control.enQueue(SHOOT_DOWN);

            #if DEMO
            if ( gameMode == 1 && player->state != DEAD )
                demo << 2 << " " << int(key) << " " << totalTime << endl;
            #endif
            break;
        case 'M':
        case 'm':
            //if ((player)->userControl && gameMode == 1 && (player)->state != DEAD)
            //    control.enQueue(MISSILE_DOWN);
            break;
        case 13:
            //if (gameMode == 0)
            //    control.enQueue( SELECT_OPTION );
            break;
        case 27:
            //control.enQueue(TOGGLE_MENU);
            break;

#if EDIT
        // Move camera up/down/left/right
        case 'W':
        case 'w':
            if (mainCamera->id == 4) {
                mainCamera->position.x-=mainCamera->direction.x*2.0f;
                mainCamera->position.y-=mainCamera->direction.y*2.0f;
                mainCamera->position.z+=mainCamera->direction.z*2.0f;
            }
            if (mainCamera->id == 2 or mainCamera->id == 3) {
                mainCamera->position.z-=5.0f; //default scale factor for now
                if( mainCamera->id == 2 ) {
                    c3->position.x = c2->position.x;
                    c3->position.z = c2->position.z;

                    if (paint) {
                        int x = last_x;
                        int z = last_z;
                        int type = last_type;
                        float red = last_red;
                        float green = last_green;
                        float blue = last_blue;
                        float height = last_height;

                        updateTerrain(x,z,height,type,red,green,blue);
                    }
                    else if (paintColor) {
                        float red = last_red;
                        float green = last_green;
                        float blue = last_blue;
                        float height = last_height;

                        updateColor(red,green,blue);
                    }
                }
                else {
                    c2->position.x = c3->position.x;
                    c2->position.z = c3->position.z;
                }
            }
            break;
        case 'A':
        case 'a':
            if (mainCamera->id == 4) {
                Vector rotationAxis;

                rotationAxis.crossProduct(dynamic_cast<Camera*>(mainCamera)->up, mainCamera->direction);
                rotationAxis.normalize();

                mainCamera->position.x+=rotationAxis.x*2.0f;
                mainCamera->position.y+=rotationAxis.y*2.0f;
                mainCamera->position.z-=rotationAxis.z*2.0f;
            }
            if (mainCamera->id == 2 or mainCamera->id == 3) {
                mainCamera->position.x-=5.0f; //default scale factor for now

                if( mainCamera->id == 2 ) {
                    c3->position.x = c2->position.x;
                    c3->position.z = c2->position.z;

                    if (paint) {
                        int x = last_x;
                        int z = last_z;
                        int type = last_type;
                        float red = last_red;
                        float green = last_green;
                        float blue = last_blue;
                        float height = last_height;

                        updateTerrain(x,z,height,type,red,green,blue);
                    }
                    else if (paintColor) {
                        float red = last_red;
                        float green = last_green;
                        float blue = last_blue;
                        float height = last_height;

                        updateColor(red,green,blue);
                    }
                }
                else {
                    c2->position.x = c3->position.x;
                    c2->position.z = c3->position.z;
                }
            }
            break;
        case 'S':
        case 's':
            if (mainCamera->id == 4) {
                mainCamera->position.x+=mainCamera->direction.x*2.0f;
                mainCamera->position.y+=mainCamera->direction.y*2.0f;
                mainCamera->position.z-=mainCamera->direction.z*2.0f;
            }
            if (mainCamera->id == 2 or mainCamera->id == 3) {
                mainCamera->position.z+=5.0f; //default scale factor for now

                if( mainCamera->id == 2 ) {
                    c3->position.x = c2->position.x;
                    c3->position.z = c2->position.z;

                    if (paint) {
                        int x = last_x;
                        int z = last_z;
                        int type = last_type;
                        float red = last_red;
                        float green = last_green;
                        float blue = last_blue;
                        float height = last_height;

                        updateTerrain(x,z,height,type,red,green,blue);
                    }
                    else if (paintColor) {
                        float red = last_red;
                        float green = last_green;
                        float blue = last_blue;
                        float height = last_height;

                        updateColor(red,green,blue);
                    }
                }
                else {
                    c2->position.x = c3->position.x;
                    c2->position.z = c3->position.z;
                }
            }
            break;
        case 'D':
        case 'd':
            if (mainCamera->id == 4) {
                Vector rotationAxis;

                rotationAxis.crossProduct(dynamic_cast<Camera*>(mainCamera)->up, mainCamera->direction);
                rotationAxis.normalize();

                mainCamera->position.x-=rotationAxis.x*2.0f;
                mainCamera->position.y-=rotationAxis.y*2.0f;
                mainCamera->position.z+=rotationAxis.z*2.0f;
            }
            if (mainCamera->id == 2 or mainCamera->id == 3) {
                mainCamera->position.x+=5.0f; //default scale factor for now

                if( mainCamera->id == 2 ) {
                    c3->position.x = c2->position.x;
                    c3->position.z = c2->position.z;

                    if (paint) {
                        int x = last_x;
                        int z = last_z;
                        int type = last_type;
                        float red = last_red;
                        float green = last_green;
                        float blue = last_blue;
                        float height = last_height;

                        updateTerrain(x,z,height,type,red,green,blue);
                    }
                    else if(paintColor) {
                        float red = last_red;
                        float green = last_green;
                        float blue = last_blue;
                        float height = last_height;

                        updateColor(red,green,blue);
                    }
                }
                else {
                    c2->position.x = c3->position.x;
                    c2->position.z = c3->position.z;
                }
            }
            break;

        // Move camera up/down
        case 'R':
        case 'r':
            if (mainCamera->id == 4) {
                mainCamera->position.x+=dynamic_cast<Camera*>(mainCamera)->up.x*2.0f;
                mainCamera->position.y+=dynamic_cast<Camera*>(mainCamera)->up.y*2.0f;
                mainCamera->position.z-=dynamic_cast<Camera*>(mainCamera)->up.z*2.0f;
            }
            if (mainCamera->id == 2 or mainCamera->id == 3) {
                mainCamera->position.y+=1.0f; //default scale factor for now
            }
            break;
        case 'F':
        case 'f':
            if ( mainCamera->id == 4) {
                mainCamera->position.x-=dynamic_cast<Camera*>(mainCamera)->up.x*2.0f;
                mainCamera->position.y-=dynamic_cast<Camera*>(mainCamera)->up.y*2.0f;
                mainCamera->position.z+=dynamic_cast<Camera*>(mainCamera)->up.z*2.0f;
            }
            if (mainCamera->id == 2 or mainCamera->id == 3) {
                mainCamera->position.y-=1.0f; //default scale factor for now
            }
            break;

        // Point camera to look down z axis
        case 'Z':
        case 'z':
            if (mainCamera->id == 4) {
                mainCamera->direction.setVector(0.0f, 0.0f, -1.0f);
                dynamic_cast<Camera*>(mainCamera)->up.setVector(0.0f, 1.0f, 0.0f);
                mainCamera->rotation.buildFromEuler(0.0f, 0.0f, 0.0f);
            }
            break;

        // Select current camera
        case '1':
            mainCamera = c1;
            currentLevel->setCamera(mainCamera);
            break;
        case '2':
            mainCamera = c2;
            currentLevel->setCamera(mainCamera);
            break;
        case '3':
            mainCamera = c3;
            currentLevel->setCamera(mainCamera);
            break;
        case '4':
            mainCamera = c4;
            currentLevel->setCamera(mainCamera);
            break;

        // Save terrain
        case 'O':
        case 'o':
            currentLevel->saveTerrain("terrains/new_terrain.txt" );
            break;

        // Undo last terrain change
        case 'U':
        {
            int x = last_x;
            int z = last_z;
            int type = last_type;
            float red = last_red;
            float green = last_green;
            float blue = last_blue;
            float height = last_height;

            updateTerrain(x,z,height,type,red,green,blue);
            break;
        }

        // Undo last color change
        case 'u':
        {
            float red = last_red;
            float green = last_green;
            float blue = last_blue;
            float height = last_height;

            updateColor(red,green,blue);
            break;
        }

        // Toggle settings
        case 'G':
        case 'g':
            currentLevel->toggleGrid();
            break;
        case 'B':
        case 'b':
            currentLevel->toggleBoundingBoxes();
            break;
        case 'C':
        case 'c':
            currentLevel->toggleControlTriangles();
            break;

        // Paint terrain and color
        case 'P':
            paint = !paint;
            break;

        // Paint color only
        case 'p':
            paintColor = !paintColor;
            break;

        // Edit terrain height
        case 'h':
            incTerrainHeight(-.05f);
            break;
        case 'H':
            incTerrainHeight(.05f);
            break;

        // Set terrain height to 0
        case '0':
            incTerrainHeight(1000.0);
            break;

        // Edit terrain type
        case 'T':
        case 't':
        {
            int skip = 0;
            int xpos, zpos, type;
            float height, red, green, blue;

            getTerrainInfo(xpos, zpos, height, type, red, green, blue);

            if (++type > 1)
                type = 0;

            updateTerrain(xpos,zpos,height, type, red, green, blue);
            break;
        }

        // Edit light direction
        case '[':
            light->x += .1;
            light->normalize();
            break;
        case ']':
            light->y += .1;
            light->normalize();
            break;
        case '\\':
            light->z += 0.1;
            light->normalize();
            break;
        case '{':
            light->x -= .1;
            light->normalize();
            break;
        case '}':
            light->y -= .1;
            light->normalize();
            break;
        case '|':
            light->z -= 0.1;
            light->normalize();
            break;
        case '.':
            // used for profiling
            exit(0);
            break;
#endif
    }
}

//------------------------------------------------------------------------------
void Engine::loadLevel(const char* levelFile)
{
    levelScript = levelFile;

    if (currentLevel != NULL)
        delete currentLevel;

    if (mainCamera == NULL) {
        // setup the current level... this really ought to be moved.
        //mainCamera = new Camera();
        c1 = new Camera(1);
        c2 = new Camera(2);
        c3 = new Camera(3);
        c4 = new Camera(4);

        mainCamera = c1;
        c2->setPosition(0.0f, 100.0f, 0.0f);
        c2->setRotationEuler(1.57, 0.0f, 0.0f);
        c3->setPosition(0.0f, 0.0f, 0.0f);
        c4->setPosition(0.0f, 0.0f, 0.0f);
    }

    mainCamera->init();
    mainCamera->unloadScript();

    currentLevel = new GameLevel(lists);
    currentLevel->setCamera( mainCamera );

    lgameLevel = currentLevel;

    currentLevel->loadLevelLua(levelScript);
    timeElapsed = timer->getElapsedSeconds(1);
    light = currentLevel->getLightDirection();
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
            model = new ModelStorage();
            model->load(filePath);
            //#model->.buildEdges(); - used for shadows "converted from old code"

            // insert model file into model hash //
            hashKey = string(de->d_name);
            modelHash[hashKey] = model;

            cout << "Loaded model file '" << filePath << "'" << endl;
        }
    }

    closedir(dir);
}

void Engine::processMouseMove(int x, int y)
{
    if (mainCamera == NULL)
        return;

    if (mainCamera->id == 3 or mainCamera->id == 4) {
    y -= 300;
    x -= 400;

    if ( y > lastY ) {
        Quaternion t;
        t.buildFromEuler((y-lastY)*.0075, 0.0f, 0.0f);
        mainCamera->rotation = mainCamera->rotation * t;
    }
    else if ( y < lastY ) {
        Quaternion t;
        t.buildFromEuler((y-lastY)*.0075, 0.0f, 0.0f);
        mainCamera->rotation = mainCamera->rotation * t;
    }

    lastY = y;

    if ( x > lastX ) {
        Quaternion t;
        t.buildFromEuler(0.0f, (x-lastX)*.0075, 0.0f);
        mainCamera->rotation = mainCamera->rotation * t;
    }
    else if ( x < lastX ) {
        Quaternion t;
        t.buildFromEuler(0.0f, (x-lastX)*.0075, 0.0f);
        mainCamera->rotation = mainCamera->rotation * t;
    }

    lastX = x;
    }
}

void Engine::getTerrainInfo(int &x, int &z, float &height, int &type, float &red, float &green, float &blue)
{
    if (mainCamera == NULL)
        return;

    x = (int)(c2->position.x /5.0f);
    z = -(int)(c2->position.z /5.0f);
    currentLevel->getTerrainInfo(x,z,height,type,red,green,blue);
}

void Engine::updateTerrain(int &x, int &z, float &height, int &type, float &red, float &green, float &blue)
{
    if (mainCamera == NULL)
        return;

    if (mainCamera->id == 2 || mainCamera->id == 3) {
        x = (int)(c2->position.x /5.0f);
        z = -(int)(c2->position.z /5.0f);
        currentLevel->updateTerrain(x,z,height,type,red,green,blue);

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
    if (mainCamera == NULL)
        return;

    if (mainCamera->id == 2 || mainCamera->id == 3) {
        int x = (int)(c2->position.x /5.0f);
        int z = -(int)(c2->position.z /5.0f);
        currentLevel->updateColor(x, z, red,green,blue);

        last_red = red;
        last_green = green;
        last_blue = blue;
    }
}

void Engine::shutdown()
{
    //unloadGame();

    if (currentLevel != NULL)
        delete currentLevel;

    if (storedLevel != NULL)
        delete storedLevel;

    if (mainCamera != NULL) {
        delete c1;
        delete c2;
        delete c3;
        delete c4;
    }

    if (soundFx_ != NULL)
        delete soundFx_;

    if (timer != NULL)
        delete timer;

    if (keyState != NULL)
        delete keyState;

    currentLevel = storedLevel = NULL;
    c1 = c2 = c3 = c4 = mainCamera = NULL;

    exit(0);
}

void Engine::pause()
{
    isPaused = !isPaused;
}

