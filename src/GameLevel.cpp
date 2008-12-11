#include "GameLevel.h"
#include "ScriptedObject.h"
#include "Scripting.h"

//------------------------------------------------------------------------------
GameLevel::GameLevel( unsigned int tLists)
{ 
    lists = tLists;
    complete = false;
    
    terrain = NULL;
    obj = NULL;
    player = NULL;
    camera = NULL;
    //boss = NULL;
    time = 0;
    
    musicPath = "";
    
    snd = NULL;
    
    /////
#if EDIT
    grid = true;
#else
    grid = false;
#endif
    bboxes = false;
    controlTriangles = false;
    
    q = new QuadTree();
    l = new DisplayList();
    
    L = NULL;
}

//------------------------------------------------------------------------------
GameLevel::~GameLevel()
{
    if ( terrain != NULL )
        delete terrain;
    if ( camera != NULL )
        delete camera;
}

//------------------------------------------------------------------------------
void GameLevel::drawLevel()
{
    // setup cel shading ///////////////////////
    glCallList( lists );
    
    // draw scene and shade /////////////////////
    terrain->drawObjects( dynamic_cast<Camera*>(camera) );

    // setup to draw outline ////////////////////
    glCallList( lists+1 );
    terrain->drawObjectOutlines( dynamic_cast<Camera*>(camera) );

    // reset draw mode to "normal"
    glCallList( lists+2 );
    
    if (grid) {
        terrain->drawGrid();    
        dynamic_cast<Camera*>(camera)->draw(camera);
    }
    
    if (bboxes)
        terrain->showCollisionBox();
        
    if (controlTriangles)
    terrain->showControlTriangle();
    
   /*
    drawText();
    */
             
    //terrain->drawShadows( light );
}

//------------------------------------------------------------------------------
void GameLevel::updateLevel(float elapsedTime)
{
    // Attempt to execute the script only if the lua state has already been
    // initialized with a script
    if (L == NULL)
        return;
    
    // Find the update function and call it
    lua_getglobal(L, "on_update");
	    
    // Push a pointer to the current object for use within the lua function
    lua_pushlightuserdata(L, (void*)terrain);
	   
	// Push the time passed since the last iteration of the game loop
    lua_pushnumber(L, elapsedTime);
    
    // Call the function with 2 argument and no return values
    lua_call(L, 2, 0);

    // get the result //
    //position.z = (float)lua_tonumber(L, -1);
    //position.y = (float)lua_tonumber(L, -2);
    //position.x = (float)lua_tonumber(L, -3);
    //lua_pop(L, 1);
    
    terrain->updateObjects( &light );

    l->clearList();
    dynamic_cast<Camera*>(camera)->frustum.extractFromProjectionMatrix(dynamic_cast<Camera*>(camera)->final);
    q->buildDisplayList(l, dynamic_cast<Camera*>(camera));
    terrain->l = l;
    
    dynamic_cast<Camera*>(camera)->draw(camera);
}

//------------------------------------------------------------------------------
void GameLevel::prepareLevel()
{
    terrain->prepareObjects();
}

//------------------------------------------------------------------------------
void GameLevel::animateLevel(float timeElapsed)
{
    time += timeElapsed;    // update levels current time
    
    terrain->animateObjects(timeElapsed, dynamic_cast<Camera*>(camera));
}

//------------------------------------------------------------------------------
bool GameLevel::loadLevelLua( string file )
{
    int numObjects;
    string modelKey;
    float angle;
    float x, y, z;
    Vector scale, rotation, position;
    Vector temp;
    
    complete = false;
    time = 0;
    eventBeginTime = 0;
    
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
    lua_register(L, "setPosition", setPositionLua);
    lua_register(L, "getPosition", getPositionLua);
    lua_register(L, "setVelocity", setVelocityLua);
    lua_register(L, "getVelocity", getVelocityLua);
    lua_register(L, "setRotationVelocity", setRotationVelocityLua);
    lua_register(L, "getRotationVelocity", getRotationVelocityLua);
    lua_register(L, "setSpeed", setSpeedLua);
    lua_register(L, "getSpeed", getSpeedLua);
    lua_register(L, "setRotation", setRotationLua);
    lua_register(L, "getRotation", getRotationLua);
    lua_register(L, "getCamera", getCameraLua);
    lua_register(L, "getPlayer", getPlayerLua);
    lua_register(L, "getDirection", getDirectionLua);
    lua_register(L, "getUp", getUpLua);
    lua_register(L, "getOrthogonal", getOrthogonalLua);
    lua_register(L, "addPosition", addPositionLua);
    lua_register(L, "addRotation", addRotationLua);
    lua_register(L, "addVelocity", addVelocityLua);
    lua_register(L, "addSpeed", addSpeedLua);
    lua_register(L, "addRotationVelocity", addRotationVelocityLua);   
    lua_register(L, "setInterpolationRotationStartAxis", setInterpolationRotationStartAxisLua);
    lua_register(L, "setInterpolationRotationEndAxis", setInterpolationRotationEndAxisLua);
    lua_register(L, "setInterpolationRotationStart", setInterpolationRotationStartLua);
    lua_register(L, "setInterpolationRotationEnd", setInterpolationRotationEndLua);
    lua_register(L, "setInterpolationEnable", setInterpolationEnableLua);
    lua_register(L, "setInterpolationVariable", setInterpolationVariableLua);
    lua_register(L, "setRotationVelocityAxis", setRotationVelocityAxisLua);
    lua_register(L, "setRotationAxis", setRotationAxisLua);
    lua_register(L, "addRotationAxis", addRotationAxisLua);
    lua_register(L, "getTimer", getTimerLua);
    lua_register(L, "suspend", suspendLua);
    lua_register(L, "playSound", playSoundLua);
    lua_register(L, "addObject", addObjectLua);
    lua_register(L, "removeObject", removeObjectLua);
    lua_register(L, "setModel", setModelLua);
    lua_register(L, "setScale", setScaleLua);
    lua_register(L, "setScript", setScriptLua);
    lua_register(L, "addParticleSystem", addParticleSystemLua);
    lua_register(L, "setBgMusic", setBgMusicLua);
    lua_register(L, "setSkyBox", setSkyBoxLua);
    lua_register(L, "setLightDirection", setLightDirectionLua);
    lua_register(L, "setTerrain", setTerrainLua);
  
    // load the script 
    cout << "Loading Lua script (level file): --" << file << "--\n";
	luaL_dofile(L, file.c_str());
    ////////////////////////////////////////////////////////   
    
    lplayer = player;
    lcamera = camera;
    lgameLevel = this;
    
    player->unloadScript();
    player->init();
    
    camera->unloadScript();
    camera->init();
    
    terrain->unloadScript();
    terrain->init();
    
    player->setPlayerPtr(player);
    player->setCameraPtr(camera);
    player->setSoundClass(snd);
    player->keyState = keyState;
    
    camera->setPlayerPtr(player);
    camera->setCameraPtr(camera);
    camera->setSoundClass(snd);
    camera->keyState = keyState;
    
    terrain->setSoundClass(snd);
    terrain->setPlayerPtr(player);
    terrain->setCameraPtr(camera);
    terrain->keyState = keyState;
     
     // Find the update function and call it
    lua_getglobal(L, "on_load");
	    
    // Push a pointer to the current object for use within the lua function
    lua_pushlightuserdata(L, (void*)terrain);
	   
    // Call the function with 1 argument and no return values
    lua_call(L, 1, 0);

    // get the result //
    //position.z = (float)lua_tonumber(L, -1);
    //position.y = (float)lua_tonumber(L, -2);
    //position.x = (float)lua_tonumber(L, -3);
    //lua_pop(L, 1);
                   
    terrain->add(player);
        
    
    ////////////////////////////////////////////
             
      cout << "building quad tree..." << endl;
      q->buildTree(terrain);
      //q->traverseTree();  
       
      cout << "building display list..." << endl;
      q->buildDisplayList(l, dynamic_cast<Camera*>(camera));
      //q->buildLeafList(l);
      cout << "traverse list..." << endl;
      l->traverseList();
      //Sleep(10000);
      //exit(1);
                                      
      cout << "finished building quad tree..." << endl;
            
      terrain->l = l;
   
     cout << "finished..." << endl << endl;
     //lua_close(L);
     //L = NULL;
     
     return (true);
}

void GameLevel::loadObject(lua_State* L, int number) 
{
    /*
    cout << "number: " << number << endl;
    
    lua_pushnumber(L, number);
    lua_gettable(L, -2);
    
    lua_pushstring(L, "type");
    lua_gettable(L, -2);
    int objectType = (int)lua_tonumber(L, -1);
    cout << "type: " << objectType << endl;
    lua_pop(L, 1);
    
    lua_pushstring(L, "script");
    lua_gettable(L, -2);
    const char *t = lua_tostring(L, -1);
    string script = "./scripts/" + string(t);
    cout << "Script: " << script << endl;
    lua_pop(L, 1);
    
    Vector position, rotation, scale;
     
    lua_pushstring(L, "position");
    lua_gettable(L, -2);
    loadVector(L, &position);
    cout << "Position: " << position.x << " " << position.y << " " << position.z << endl;
    lua_pop(L, 1);
        
    if ( number == 1 ) {
        // object[0] must be player
        objectType = OBJECT_PLAYER;
    }
    
    // The player will always be created before a level is loaded        
    if (player == NULL) {
        printf("The player object was not initialized prior to loading the level.\n");
        exit(1);   
    }
             
    switch( objectType )
    {
        case OBJECT_PLAYER:
            obj = dynamic_cast<Player*>(player);
            obj->setTimer( &time );
            break;                                       
        default:
            obj = new ScriptedObject(script);
            break;
    }
        
    if (obj != NULL) {
        // Set the object's orientation
        obj->setPosition( position );
        obj->setVelocity( 0.0f, 0.0f, 0.0f );
    
        // Set required resources for the object's use       
        obj->setSoundClass(snd);
        obj->setPlayerPtr(player);
        obj->setCameraPtr(camera);
                
        // This may not be the best place for this... it may cause a 
        // boot strapping issue. We may need to load all scripts after
        // all objects have been fully loaded.
        obj->loadScript(script);
        
        // Add the object to the level's objects list
        terrain->add(obj);
    } 
  
    lua_pop(L, 1);  
    */  
}

//------------------------------------------------------------------------------
void GameLevel::setPlayer( Player* tPlayer )
{
    player = (tPlayer);
}

//------------------------------------------------------------------------------
void GameLevel::setTerrain( Terrain* tTerrain )
{ terrain = tTerrain; }

//------------------------------------------------------------------------------
void GameLevel::setSoundClass( Sound *sound )
{ snd = sound; }

//------------------------------------------------------------------------------
void GameLevel::setCamera( Camera* tCamera )
{ camera = (tCamera); }

//------------------------------------------------------------------------------
Terrain* GameLevel::getTerrain( void )
{ return( (Terrain*)terrain ); }

//------------------------------------------------------------------------------
Camera* GameLevel::getCamera( void )
{ return( (Camera*)camera ); }

//------------------------------------------------------------------------------
Player* GameLevel::getPlayer( void )
{ return( (Player*)player ); }

//------------------------------------------------------------------------------
//Boss* GameLevel::returnBoss( void )
//{ return boss; }

//------------------------------------------------------------------------------
void GameLevel::drawSky()
{       
    //glPushMatrix();
        // -front sky //
        glBegin(GL_TRIANGLE_STRIP); 
            glColor3fv ( bgcolor[0]);
            glVertex3f (-1.0f, 1.0f, -1.0f);
            glColor3fv ( bgcolor[1]);
            glVertex3f (-1.0f, 0.0f, -1.0f);
            glColor3fv ( bgcolor[0]);
            glVertex3f (1.0f, 1.0f, -1.0f);
            glColor3fv ( bgcolor[1]);
            glVertex3f (1.0f, 0.0f, -1.0f);
        glEnd();
        glBegin(GL_TRIANGLE_STRIP);
            glVertex3f (-1.0f, 0.0f, -1.0f);
            glColor3fv ( bgcolor[2]);
            glVertex3f (-1.0f, -1.0f, -1.0f);
            glColor3fv ( bgcolor[1]);            
            glVertex3f (1.0f, 0.0f, -1.0f);
            glColor3fv ( bgcolor[2]);            
            glVertex3f (1.0f, -1.0f, -1.0f);         
        glEnd();
        
        // left sky //
        glBegin(GL_TRIANGLE_STRIP); 
            glColor3fv ( bgcolor[0]);
            glVertex3f (-1.0f, 1.0f, 1.0f);
            glColor3fv ( bgcolor[1]);
            glVertex3f (-1.0f, 0.0f, 1.0f);
            glColor3fv ( bgcolor[0]);
            glVertex3f (-1.0f, 1.0f, -1.0f);
            glColor3fv ( bgcolor[1]);
            glVertex3f (-1.0f, 0.0f, -1.0f);
        glEnd();
        glBegin(GL_TRIANGLE_STRIP);
            glVertex3f (-1.0f, 0.0f, 1.0f);
            glColor3fv ( bgcolor[2]);
            glVertex3f (-1.0f, -1.0f, 1.0f);
            glColor3fv ( bgcolor[1]);            
            glVertex3f (-1.0f, 0.0f, -1.0f);
            glColor3fv ( bgcolor[2]);            
            glVertex3f (-1.0f, -1.0f, -1.0f);          
        glEnd();
        
        // right sky //
        glBegin(GL_TRIANGLE_STRIP); 
            glColor3fv ( bgcolor[0]);
            glVertex3f (1.0f, 1.0f, -1.0f);
            glColor3fv ( bgcolor[1]);
            glVertex3f (1.0f, 0.0f, -1.0f);
            glColor3fv ( bgcolor[0]);
            glVertex3f (1.0f, 1.0f, 1.0f);
            glColor3fv ( bgcolor[1]);
            glVertex3f (1.0f, 0.0f, 1.0f); 
        glEnd();
        glBegin(GL_TRIANGLE_STRIP);
            glVertex3f (1.0f, 0.0f, -1.0f);
            glColor3fv ( bgcolor[2]);
            glVertex3f (1.0f, -1.0f, -1.0f);
            glColor3fv ( bgcolor[1]);            
            glVertex3f (1.0f, 0.0f, 1.0f);
            glColor3fv ( bgcolor[2]);            
            glVertex3f (1.0f, -1.0f, 1.0f);          
        glEnd();
        
        // -back sky //
        glBegin(GL_TRIANGLE_STRIP); 
            glColor3fv ( bgcolor[0]);
            glVertex3f (1.0f, 1.0f, 1.0f);
            glColor3fv ( bgcolor[1]);
            glVertex3f (1.0f, 0.0f, 1.0f);
            glColor3fv ( bgcolor[0]);
            glVertex3f (-1.0f, 1.0f, 1.0f);
            glColor3fv ( bgcolor[1]);
            glVertex3f (-1.0f, 0.0f, 1.0f);
            
        glEnd();
        glBegin(GL_TRIANGLE_STRIP);
            glVertex3f (1.0f, 0.0f, 1.0f);
            glColor3fv ( bgcolor[2]);
            glVertex3f (1.0f, -1.0f, 1.0f);
            glColor3fv ( bgcolor[1]);            
            glVertex3f (-1.0f, 0.0f, 1.0f);
            glColor3fv ( bgcolor[2]);            
            glVertex3f (-1.0f, -1.0f, 1.0f);        
        glEnd();
        
        
        // top sky //
        glBegin(GL_TRIANGLE_STRIP); 
            glColor3fv ( bgcolor[0]);
            glVertex3f (-1.0f, 1.0f, -1.0f);  
            glVertex3f (1.0f, 1.0f, -1.0f);
            glVertex3f (-1.0f, 1.0f, 1.0f);
            glVertex3f (1.0f, 1.0f, 1.0f);         
        glEnd();
        
         // bottom sky //
        glBegin(GL_TRIANGLE_STRIP); 
            glColor3fv ( bgcolor[2]);
            glVertex3f (1.0f, -1.0f, -1.0f);  
            glVertex3f (-1.0f, -1.0f, -1.0f);
            glVertex3f (1.0f, -1.0f, 1.0f);
            glVertex3f (-1.0f, -1.0f, 1.0f); 
        glEnd();
        
       
        
    //glPopMatrix();
}

//------------------------------------------------------------------------------
Object* GameLevel::findEnemy( void )
{
    float closest = 1000, temp;
    Object* obj = terrain;
    Object* retObj = NULL;
    
    while( obj->next != 0 )
    {
        obj = (Object*)obj->next;
        //#if ( obj->checkActiveEnemy() )
        {
            temp = findDistance( (Object*)player, obj );
            if ( temp < closest )
            {
                retObj = obj;
                closest = temp;
            }            
        }
    }
   
    return( retObj );
}

//------------------------------------------------------------------------------
float GameLevel::findDistance ( Object* obj1, Object* obj2 )
{ 
    return( sqrtf( ( obj1->position.x - obj2->position.x)*( obj1->position.x - obj2->position.x)+
          ( obj1->position.y - obj2->position.y)*( obj1->position.y - obj2->position.y)+
          ( obj1->position.z - obj2->position.z)*( obj1->position.z - obj2->position.z) ) );
}

//------------------------------------------------------------------------------
bool GameLevel::checkComplete(void)
{
    if ( complete == true ) {
        if ( eventBeginTime == 0 ) 
            eventBeginTime = time;
        else if ( (eventBeginTime + 7.0) > time )
            return true;
    }
    else
        return false;
}

//------------------------------------------------------------------------------
void GameLevel::unloadLevel()
{    
  //  boss = NULL;
  
  // Attempt to execute the on_unload function only if the lua state has 
    // already been initialized with a script
    if (L == NULL)
        return;
    
    // Find the update function and call it
    lua_getglobal(L, "on_unload");
	    
    // Push a pointer to the current object for use within the lua function
    lua_pushlightuserdata(L, (void*)terrain);
	   
    // Call the function with 1 argument and no return values
    lua_call(L, 1, 0);

    // get the result //
    //position.z = (float)lua_tonumber(L, -1);
    //position.y = (float)lua_tonumber(L, -2);
    //position.x = (float)lua_tonumber(L, -3);
    //lua_pop(L, 1);
    
    lua_close(L);
    L = NULL;
}

//------------------------------------------------------------------------------
void GameLevel::removePlayer()
{
    ObjectNode* obj = terrain->next, *temp;

    while( obj != NULL )
    {
        temp = obj->next;
        if ( typeid(*obj) == typeid(Player) ) {
            obj->remove();
            cout << "player found and removed";
           
        } else {
            obj->remove();
            delete obj;
        }
        
        obj = temp;
    }
}

//------------------------------------------------------------------------------
void GameLevel::processScripts()
{
/*
   ScriptCommand t;
   
   
   
   // process text scripts ///
    for ( int i = 0; i < numTextScripts; i++ ) {
      if ( !textScript[i].isRunning() )
         textScript[i].evaluateBeginExpression();
           
      while (  textScript[i].getNextCommand(t) ) {
          if ( t.objNum < numTextStrings )
            (scriptText[t.objNum]).processScriptCommand( t );
      }
   }
   
   */
}


//------------------------------------------------------------------------------
void GameLevel::animateText( float timeElapsed )
{
    //for ( int i = 0; i < numTextStrings; i++ ) 
        //scriptText[i].updateText( timeElapsed );
}

//------------------------------------------------------------------------------
void GameLevel::drawText()
{
    //for ( int i = 0; i < numTextStrings; i++ ) 
        //scriptText[i].displayText();
}

//------------------------------------------------------------------------------
void GameLevel::updateTerrain(int &x, int &z, float &height, int &type, float &red, float &green, float &blue)
{    
    if ( x >= 0 && x < terrain->xSize && z >= 0 && z <= terrain->zSize ) {
        terrain->vertex[x][z][1]= height;
        terrain->color[x][z][0] = red;
        terrain->color[x][z][1] = green;
        terrain->color[x][z][2] = blue;
        terrain->type[x][z] = type;
        
        terrain->calcTerrainNorm(&light);
    }
}

void GameLevel::updateColor(int &x, int &z, float &red, float &green, float &blue)
{     
    if ( x >= 0 && x < terrain->xSize && z >= 0 && z <= terrain->zSize ) {
        terrain->color[x][z][0] = red;
        terrain->color[x][z][1] = green;
        terrain->color[x][z][2] = blue;
        
        terrain->calcTerrainNorm(&light);
    }
}

void GameLevel::getTerrainInfo(int &x, int &z, float &height, int &type, float &red, float &green, float &blue)
{  
    if ( x >= 0 && x < terrain->xSize && z >= 0 && z <= terrain->zSize ) {
        type = terrain->type[x][z];
        height = terrain->vertex[x][z][1];
        red = terrain->color[x][z][0];
        green = terrain->color[x][z][1];
        blue = terrain->color[x][z][2];
    }   
}

void GameLevel::saveTerrain(char* filePath)
{
    terrain->saveTerrain(filePath, &light);
}

void GameLevel::toggleGrid(void) { grid = !grid;}
void GameLevel::toggleBoundingBoxes(void) { bboxes = !bboxes; }
void GameLevel::toggleControlTriangles(void) { controlTriangles = !controlTriangles; }

void GameLevel::setSkyBox(float** tBgColor, int x, int y)
{
    for (int i = 0; i < y; i++) {
        for (int j = 0; j < x; j++) {
            bgcolor[i][j] = tBgColor[i][j];
        }
    }
}
