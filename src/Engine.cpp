#include <dirent.h>

#include "Engine.h"

//------------------------------------------------------------------------------
Engine::Engine()
{
    sounds = new Sound ("music/songlist.txt", "sounds/sfxlist.txt");
    loadModels();

    // setup light source ////////////////////////-
    light = new Vector();
    light->setVector( 0.0f, 0.15, .85 );
    light->normalize();
    currentLevel = NULL;
    
    // setup game timer /////////////////////////
    timer = new GameTimer;
    if( !timer->init() ) {
        printf ( "Timer initialization failed." );
        exit (1);
    }
     
    vMove = hMove = 0;

    gameMode = 1;  // 0
    menuCursor = false;
    MenuStr = "Start Game";
    lvlNum = 0;
    maxLevels = 0;
    numLives = 5;
    score = 0;
    shooting = false;
    
    player = new Player( MODEL_PLAYER );
    
    totalTime = 0;
    
    strcpy(levelFile[0],  "./levels/level1.txt" );
    strcpy(levelFile[1],  "./levels/level2.txt" );
    strcpy(levelFile[2],  "./levels/level3.txt" );
    strcpy(levelFile[3],  "./levels/level4.txt" );    
    
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
    
    paint = false;
    paintColor = false;
    
    
    lastX = 0.0f;
    lastY = 0.0f;
    
    cam = 0;
    
    last_x = 0;
    last_z = 0;
    last_red = 0.0f;
    last_green = 0.0f;
    last_blue = 0.0f;
    last_height = 0.0f;
    last_type = 0;
    
}

//------------------------------------------------------------------------------
void Engine::gameCycle()
{
    timeElapsed = timer->getElapsedSeconds(1);  
    processCommands();
    
    if ( gameMode == 3 ) {
        totalTime += timeElapsed;
    }
    
    
    #if PLAY_DEMO
    if ( gameMode == 1 )
    {
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
    }
    #endif
    
    switch (gameMode)
    {
        case 1:    
            light->normalize();
            if ( currentLevel->checkComplete() )
            {
                cout << "deleting level";
                currentLevel->removePlayer();
                newLevel();
                cout << "..done\n";
            }
            else
            {
               currentLevel->processScripts();
                
                (mainCamera)->update(timeElapsed);
            
                currentLevel->animateLevel( timeElapsed );
                currentLevel->animateText( timeElapsed );

                currentLevel->updateLevel( light );
                

                //currentLevel->prepareLevel();   // collision detection

                currentLevel->updateLevel( light );    
                

                prepare();
             
                currentLevel->drawLevel();
             
                
             }
            break;
            
        case 0:
            glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
            glLoadIdentity();  
            menu.draw( MenuStr, menuCursor );
            break;
        case 2:
            glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
            glLoadIdentity();  
            credits.draw( 500, timeElapsed );
            break;
            
        default:
            break;
    }
    
    sounds->Update(); 
    glutSwapBuffers();
}

//------------------------------------------------------------------------------
void Engine::prepare()
{
    glClear( GL_DEPTH_BUFFER_BIT );
    glLoadIdentity();  
       
    (mainCamera)->prepareGLView();  
    
    glDepthMask(GL_FALSE);
    currentLevel->drawSky();
    glDepthMask(GL_TRUE);
    
    glDepthFunc(GL_ALWAYS);
    if ( gameMode == 1 ) {
        displayHUD( (player)->life, (player)->numLives, boss->life, (player)->score );
        displayDebug();
    }
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
    
    
    currentLevel = new GameLevel( lists, light );
    //mainCamera = new Camera();
    c1 = new Camera();
    c1->id = 1;
    c2 = new Camera();
    c2->id = 2;
    c3 = new Camera();
    c3->id = 3;
    c4 = new Camera();
    c4->id = 4;
    
    mainCamera = c1;
    c2->setPosition(0.0f, 100.0f, 0.0f);
    c2->setRotationEuler(1.57, 0.0f, 0.0f);
    c3->setPosition(0.0f, 0.0f, 0.0f);
    c4->setPosition(0.0f, 0.0f, 0.0f);
    
    terrain = new Terrain();
    
    currentLevel->setPlayer( player );
    currentLevel->setSoundClass( sounds ); 
    currentLevel->setCamera( mainCamera );
    currentLevel->setTerrain( terrain );
    
    loadLevel();

   // sounds->StopSong();
   // sounds->PlaySong(4, false );
}

//------------------------------------------------------------------------------
void Engine::processKeyUp(int key)
{
    if ((player)->userControl) {
        switch( key )
        {
            case GLUT_KEY_UP:
                if ( gameMode == 1 ) 
                    control.enQueue( VEL_UP_KEY_UP );    
                break;
            case GLUT_KEY_DOWN:
                if ( gameMode == 1 ) 
                    control.enQueue( VEL_DOWN_KEY_UP );
                break;
            case GLUT_KEY_LEFT:
                if ( gameMode == 1 ) 
                    control.enQueue( VEL_LEFT_KEY_UP );
                break;
            case GLUT_KEY_RIGHT:
                if ( gameMode == 1 ) 
                    control.enQueue( VEL_RIGHT_KEY_UP );
                break;
        }
    }
    
    #if DEMO
        if ( gameMode == 1 )
            demo << 3 << " " << key << " " << totalTime << endl;
    #endif
}
//------------------------------------------------------------------------------
void Engine::processKeyDown( int key )
{
    if ((player)->userControl) {
        switch( key )
        {
            case GLUT_KEY_UP:
                if( gameMode == 1 ) {
                    control.enQueue( VEL_UP_KEY_DOWN ); }
                else {
                    control.enQueue( MOVE_CURSOR ); }
                break;
            case GLUT_KEY_DOWN:
                if( gameMode == 1 ) {
                    control.enQueue( VEL_DOWN_KEY_DOWN ); }
                else {
                    control.enQueue( MOVE_CURSOR ); }
                break;
            case GLUT_KEY_LEFT:
                if ( gameMode == 1 ) 
                    control.enQueue( VEL_LEFT_KEY_DOWN );
                break;
            case GLUT_KEY_RIGHT:
                if ( gameMode == 1 ) 
                    control.enQueue( VEL_RIGHT_KEY_DOWN );
                break;
            case GLUT_KEY_F1:
                control.enQueue( TOGGLE_LOD );
                break;
        }
    }

    #if DEMO
        if ( gameMode == 1 )
            demo << 1 << " " << key << " " << totalTime << endl;
    #endif
}

//------------------------------------------------------------------------------
void Engine::processCommands()
{
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
            case VEL_LEFT_KEY_DOWN:
                hMove += 1;
                player->velocity.x = mainCamera->velocity.x-10.0f;
                
                break;
                
            case VEL_LEFT_KEY_UP:
                hMove -= 1;
                if ( hMove < 0 )
                    hMove = 0;
                    
                if ( hMove == 2 )
                    player->velocity.x = mainCamera->velocity.x+10.0f;
                       
                else
                   player->velocity.x = mainCamera->velocity.x;
                
                break;
        
            case VEL_RIGHT_KEY_DOWN:
                hMove += 2;
                player->velocity.x = mainCamera->velocity.x+10.0f;

                break;
                
            case VEL_RIGHT_KEY_UP:
                hMove -= 2;
                if ( hMove < 0 )
                    hMove = 0;
                    
                if ( hMove == 1 )
                    player->velocity.x = mainCamera->velocity.x-10.0f;
                else
                    player->velocity.x = mainCamera->velocity.x; 

                break;
            
            case SHOOT_DOWN:
                (player)->fireShot(mainCamera->velocity.x);        // gameSpeed
                
                break;
                
            case MISSILE_DOWN:
                obj = currentLevel->findEnemy();
                (player)->fireMissle( obj );
            
                break;
                
            case TOGGLE_MENU:
                control.emptyQueue();
                if ( gameMode == 1 )
                {
                    sounds->PauseSong();
                    sounds->PlaySFX(2);
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
                    loadLevel();
                    //strcpy( MenuStr, "Restart" );     
                }   
               
                hMove = vMove = 0;
                player->setVelocity( mainCamera->velocity.x, mainCamera->velocity.y, mainCamera->velocity.z );
                break;
                
            case MOVE_CURSOR:
                menuCursor = !menuCursor;
                sounds->PlaySFX(1);
                
                break;
            
            case SELECT_OPTION:
                sounds->StopSong();
                vMove = hMove = 0;
                control.emptyQueue();
                
                if ( menuCursor ) {   
                    gameMode = 1;
                    sounds->PlaySong((lvlNum+1), true);
                }
                else if ( !menuCursor ) 
                    control.enQueue( QUIT_GAME );
                    
                break;
                
            case TOGGLE_LOD:
              //  detailLevel = !detailLevel;
                break;
            
            case QUIT_GAME:
            #if DEMO
                demo.close();
            #endif
                //delete currentLevel;
                exit(0);
                break; 
        }
    
        cmd = control.deQueue();    // get next command from queue
    }
}
//------------------------------------------------------------------------------
void Engine::processNormalKey(unsigned char key)
{
    key = toupper(key);

    switch ( key )
    {  
        case 32:
            if ( (player)->userControl && gameMode == 1 && (player)->state != DEAD ) control.enQueue(SHOOT_DOWN);
            
            #if DEMO
            if ( gameMode == 1 && player->state != DEAD )
                demo << 2 << " " << int(key) << " " << totalTime << endl;
            #endif
            break;
        case 'M':
            if ( (player)->userControl && gameMode == 1 && (player)->state != DEAD ) control.enQueue(MISSILE_DOWN);
        case 13:
            if ( gameMode == 0 )
                control.enQueue( SELECT_OPTION );
            break; 
        case 27:
            control.enQueue(TOGGLE_MENU);
            break;
            
        case 'W':
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
        {
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
             }   else if (paintColor) {
                  float red = last_red;
            float green = last_green;
            float blue = last_blue;
            float height = last_height;
            
            updateColor(red,green,blue);
             } }
                else {
                    c2->position.x = c3->position.x;
                    c2->position.z = c3->position.z;
                }
           }
            break;
        }
        case 'S':
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
              }  else if (paintColor) {
                  float red = last_red;
            float green = last_green;
            float blue = last_blue;
            float height = last_height;
            
            updateColor(red,green,blue);
              } }
                else {
                    c2->position.x = c3->position.x;
                    c2->position.z = c3->position.z;
                }
           }
            break;
        case 'D':
        {
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
            } else if(paintColor) {
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
        }
        
        case 'R':
        {
            if (mainCamera->id == 4) {
            mainCamera->position.x+=dynamic_cast<Camera*>(mainCamera)->up.x*2.0f;
            mainCamera->position.y+=dynamic_cast<Camera*>(mainCamera)->up.y*2.0f;
            mainCamera->position.z-=dynamic_cast<Camera*>(mainCamera)->up.z*2.0f;
            }
            if (mainCamera->id == 2 or mainCamera->id == 3) {
                mainCamera->position.y+=1.0f; //default scale factor for now
           }
            break;
        }
        case 'F':
        {
            if ( mainCamera->id == 4) {
            mainCamera->position.x-=dynamic_cast<Camera*>(mainCamera)->up.x*2.0f;
            mainCamera->position.y-=dynamic_cast<Camera*>(mainCamera)->up.y*2.0f;
            mainCamera->position.z+=dynamic_cast<Camera*>(mainCamera)->up.z*2.0f;
            }
            if (mainCamera->id == 2 or mainCamera->id == 3) {
                mainCamera->position.y-=1.0f; //default scale factor for now
           }
            break;
        }
        case 'X':
        {
            if (mainCamera->id == 4) {
            mainCamera->direction.setVector(0.0f, 0.0f, -1.0f);
            dynamic_cast<Camera*>(mainCamera)->up.setVector(0.0f, 1.0f, 0.0f);
            mainCamera->rotation.buildFromEuler(0.0f, 0.0f, 0.0f);
            }
            break;
        }
        case 'C':
            cam++;
            
            if ( cam > 3 )
                cam = 0;
                
            if ( cam == 0 )
                mainCamera = c1;
            else if (cam == 1)
                mainCamera = c2;
            else if (cam == 2)
                mainCamera = c3;
            else if (cam == 3)
                mainCamera = c4;
                
            currentLevel->setCamera(mainCamera);
            break;
        case 'T':
            currentLevel->saveTerrain("terrains/new_terrain.txt" );        
            break;
        case 'L':
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
        
        case 'G':
        {
            currentLevel->toggleGrid();
            break;
        } 
        case 'B':
        {
            currentLevel->toggleBoundingBoxes();
            break;
        }
        case 'U':
        {
            player->scriptNum = 1;
            player->currentScriptCommand = 0;
            break;
        }
        case 'J':
        {
            player->scriptNum = 3;
            player->currentScriptCommand = 0;
            break; 
        }
        case 'P':
        {
            paint = !paint;
            break;
        }
        case 'K':
        {
           
            float red = last_red;
            float green = last_green;
            float blue = last_blue;
            float height = last_height;
            
            updateColor(red,green,blue);
        }
        case 'O':
        {
            paintColor = !paintColor;
            break;
        }
        case '[':
        {
            int skip = 0;
            int xpos, zpos, type;
            float height, red, green, blue;
    
            getTerrainInfo(xpos, zpos, height, type, red, green, blue);
            
            height -= .05;
            
            updateTerrain(xpos,zpos,height, type, red, green, blue);
            break;
        }
        case ']':
        {
            int skip = 0;
            int xpos, zpos, type;
            float height, red, green, blue;
    
            getTerrainInfo(xpos, zpos, height, type, red, green, blue);
            
            height += .05;
            
            updateTerrain(xpos,zpos,height, type, red, green, blue);
            break;
        }
        case '-':
        {
            int skip = 0;
            int xpos, zpos, type;
            float height, red, green, blue;
    
            getTerrainInfo(xpos, zpos, height, type, red, green, blue);
            
            height = 0;
            
            updateTerrain(xpos,zpos,height, type, red, green, blue);
            break;
        }
        case '=':
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
        case 'V':
        {
            Vector pPos = dynamic_cast<Object*>(player)->position;
            mainCamera->setRotationEuler(0.0f, 90.0f, 0.0f);
            mainCamera->setPosition(pPos.x - 10.0f, pPos.y, pPos.z - 10.0f);
        }
        
    }
}

//------------------------------------------------------------------------------
void Engine::displayHUD(float ShipEnergy, int ShipLives, float EnemyEnergy, long score)
{
    char str[128];
    float EnergyBar = -0.99 + (0.068 * ShipEnergy);
    float BossEnergy = 0.99 - (0.017 * EnemyEnergy);
    
    glPushMatrix();
        glTranslatef (0.0f, 0.0f, -2.0f);
        glColor3f (1.0f, 1.0f, 1.0f);
        sprintf (str, "Lives: %i", ShipLives);
        glRasterPos2f (-1.0f, 0.7f);
        render_string(GLUT_BITMAP_HELVETICA_18, str);
        sprintf (str, "Enemy");
        glRasterPos2f (0.85f, 0.7f);
        render_string(GLUT_BITMAP_HELVETICA_18, str);
        
        // for fps //////////////////////////////
        sprintf (str, "Score: %08i", score );
        glRasterPos2f (-.2f, 0.7f);
        render_string(GLUT_BITMAP_HELVETICA_18, str);
        /////////////////////////////////////////
        
        // for fps //////////////////////////////
        sprintf (str, "Missiles: %i", (player)->numMissiles );
        glRasterPos2f (-1.0f, .75);
        render_string(GLUT_BITMAP_HELVETICA_18, str);
        /////////////////////////////////////////
        
        glBegin(GL_QUADS);
            glColor3f (1.0f, 0.0f, 0.0f);
            glVertex3f (-1.0f, 0.68f, 0.0f);
            glVertex3f (-1.0f, 0.63f, 0.0f);
            glVertex3f (-0.3f, 0.63f, 0.0f);
            glVertex3f (-0.3f, 0.68f, 0.0f);
            
            glColor3f (0.0f, 0.0f, 0.0f);
            glVertex3f (-0.99f, 0.67f, 0.001f);
            glVertex3f (-0.99f, 0.64f, 0.001f);
            glVertex3f (-0.31f, 0.64f, 0.001f);
            glVertex3f (-0.31f, 0.67f, 0.001f);
            
            glColor3f (0.0f, 1.0f, 1.0f);
            glVertex3f (-0.99f, 0.67f, 0.002f);
            glVertex3f (-0.99f, 0.64f, 0.002f);
            glVertex3f (EnergyBar, 0.64f, 0.002f);
            glVertex3f (EnergyBar, 0.67f, 0.002f);
            
            glColor3f (1.0f, 0.0f, 0.0f);
            glVertex3f (0.3f, 0.68f, 0.0f);
            glVertex3f (0.3f, 0.63f, 0.0f);
            glVertex3f (1.0f, 0.63f, 0.0f);
            glVertex3f (1.0f, 0.68f, 0.0f);
            
            glColor3f (0.0f, 0.0f, 0.0f);
            glVertex3f (0.31f, 0.67f, 0.001f);
            glVertex3f (0.31f, 0.64f, 0.001f);
            glVertex3f (0.99f, 0.64f, 0.001f);
            glVertex3f (0.99f, 0.67f, 0.001f);
            
            glColor3f (1.0f, 1.0f, 0.0f);
            glVertex3f (BossEnergy, 0.67f, 0.002f);
            glVertex3f (BossEnergy, 0.64f, 0.002f);
            glVertex3f (0.99f, 0.64f, 0.002f);
            glVertex3f (0.99f, 0.67f, 0.002f);
        glEnd();
        
    glPopMatrix();
}

//------------------------------------------------------------------------------
void Engine::displayDebug(void)
{
    char str[128];
    
    glPushMatrix();
        glTranslatef (0.0f, 0.0f, -2.0f);
        glColor3f (1.0f, 1.0f, 1.0f);
        sprintf (str, "plr pos: %.04f %.04f %.04f", player->position.x, player->position.y, player->position.z);
        glRasterPos2f (-1.0f, 0.55f);
        render_string(GLUT_BITMAP_9_BY_15, str);
        
        glColor3f (1.0f, 1.0f, 1.0f);
        sprintf (str, "cam pos: %.04f %.04f %.04f", mainCamera->position.x, mainCamera->position.y, mainCamera->position.z);
        glRasterPos2f (-1.0f, 0.48f);
        render_string(GLUT_BITMAP_9_BY_15, str);
        
        glColor3f (1.0f, 1.0f, 1.0f);
        sprintf (str, "cam dir: %.04f %.04f %.04f", mainCamera->direction.x, mainCamera->direction.y, mainCamera->direction.z);
        glRasterPos2f (-1.0f, 0.40f);
        render_string(GLUT_BITMAP_9_BY_15, str);
        
        glColor3f (1.0f, 1.0f, 1.0f);
        sprintf (str, "cam up : %.04f %.04f %.04f", (mainCamera)->up.x, (mainCamera)->up.y, (mainCamera)->up.z);
        glRasterPos2f (-1.0f, 0.33f);
        render_string(GLUT_BITMAP_9_BY_15, str);
        
        Vector tv;
        mainCamera->rotation.getEulerAngles(tv);
        
        glColor3f (1.0f, 1.0f, 1.0f);
        sprintf (str, "cam rot: %.04f %.04f %.04f", tv.x, tv.y, tv.z);
        glRasterPos2f (-1.0f, 0.26f);
        render_string(GLUT_BITMAP_9_BY_15, str);
        
        float fps = timer->getFPS();
        glColor3f (1.0f, 1.0f, 1.0f);
        sprintf (str, "fps    : %.04f", fps);
        glRasterPos2f (-1.0f, 0.18f);
        render_string(GLUT_BITMAP_9_BY_15, str);
        
        glColor3f (1.0f, 1.0f, 1.0f);
        sprintf (str, "cam id : %d", mainCamera->id);
        glRasterPos2f (-1.0f, 0.10f);
        render_string(GLUT_BITMAP_9_BY_15, str);
        
    glPopMatrix();
}


//------------------------------------------------------------------------------
void Engine::loadLevel()
{
    currentLevel->unloadLevel();
    //currentLevel->loadLevel( levelFile[lvlNum] );
    currentLevel->loadLevelLua(levelScript);
    boss = currentLevel->returnBoss();
}

//------------------------------------------------------------------------------
void Engine::newLevel()
{
    lvlNum++;

    if ( lvlNum > maxLevels )
    {
        gameMode = 2;
        numLives = 0;
        sounds->StopSong();
        sounds->PlaySong(5, false);
    }
    else
    {
        loadLevel();
        sounds->StopSong();
        sounds->PlaySong(lvlNum, true);
    }
}

//------------------------------------------------------------------------------
void Engine::loadModels()
{
    /*
    model = new ModelStorage[NUM_MODELS];
    char fileName[NUM_MODELS][30] = { 
        "./models/Ship.mdl", "./models/Enemy.mdl", "./models/Asteroid.mdl", "./models/Beam.mdl", "./models/Boss.mdl",
        "./models/SailBoat.mdl", "./models/BoatCannon.mdl", "./models/Arwing.mdl", "./models/NeskimosLogo.mdl",
        "./models/Explosion.mdl", "./models/BlueShot.mdl", "./models/GreenShot.mdl", "./models/missle.mdl",
        "./models/CannonBall.mdl", "./models/Powerup.mdl", "./models/Powerup2.mdl", "./models/Building.mdl",
        "./models/FishBoss.mdl", "./models/SnowBoss.mdl", "./models/cactus1.mdl", "./models/cactus2.mdl", "./models/EnemyMissle.mdl"
    };
       
    for ( int i = 0; i < NUM_MODELS; i++ ) {
        model[i].load( fileName[i] );
        //model[i].buildEdges(); $shadow
    }
    */
    
    
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
            
            cout << "Found model file: --" << filePath << "--Attempting to load...\n";
            // load model file into model storage //
            model = new ModelStorage();
            model->load(filePath);
            
            // insert model file into model hash //
            hashKey = string(de->d_name);    
            modelHash[hashKey] = model;
            
            cout << "Model stored with key: --" << hashKey << "--\n";
        }
    }
    
    closedir(dir);   
}

void Engine::processMouseMove(int x, int y)
{
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
    x = (int)(c2->position.x /5.0f);
    z = -(int)(c2->position.z /5.0f);
    currentLevel->getTerrainInfo(x,z,height,type,red,green,blue);
}


void Engine::updateTerrain(int &x, int &z, float &height, int &type, float &red, float &green, float &blue)
{
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
    if (mainCamera->id == 2 || mainCamera->id == 3) {
        int x = (int)(c2->position.x /5.0f);
        int z = -(int)(c2->position.z /5.0f);
        currentLevel->updateColor(x, z, red,green,blue);
        
        last_red = red;
        last_green = green;
        last_blue = blue;
    }
}


