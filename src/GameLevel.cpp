#include "GameLevel.h"
#include "ScriptedObject.h"
#include "Scripting.h"

#include "lua/LuaGL.h"

Object* lcamera;
Object* lplayer;

extern int luaopen_opengl (lua_State *L);

//------------------------------------------------------------------------------
GameLevel::GameLevel( unsigned int tLists)
{ 
    lists = tLists;
    complete = false;
    
    terrain = NULL;

    player = NULL;
    camera = NULL;

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
    
    elapsedTime = 0.0f;
}

//------------------------------------------------------------------------------
GameLevel::~GameLevel()
{
    unloadLevel();
}

//------------------------------------------------------------------------------
void GameLevel::drawLevel()
{
    // setup cel shading ///////////////////////
    glCallList( lists );
    
    // draw scene and shade /////////////////////
    terrain->drawObjects( dynamic_cast<Camera*>(camera) );
    
    // Attempt to execute the script only if the lua state has already been
    // initialized with a script
    if (L == NULL)
        return;
    
    // Find the update function and call it
    lua_getglobal(L, "on_draw");
	    
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
void GameLevel::postDraw()
{
    // Attempt to execute the script only if the lua state has already been
    // initialized with a script
    if (L == NULL)
        return;
    
    // Find the update function and call it
    lua_getglobal(L, "on_draw_screen");
	    
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

   /*
    drawText();
    */
             
    //terrain->drawShadows( light );
}

//------------------------------------------------------------------------------
void GameLevel::updateLevel()
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
void GameLevel::animateLevel()
{
    time += elapsedTime;    // update levels current time
    
    terrain->animateObjects(elapsedTime, dynamic_cast<Camera*>(camera));
}

//------------------------------------------------------------------------------
bool GameLevel::loadLevelLua( string file )
{   
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
	
	luaopen_opengl(L);
	
    // Register our functions for use in lua (currently defined in 
    // Object.h)
    registerFunctions(L, 1);
    
    // load the script 
    cout << "Loading Lua script (level file): --" << file << "--\n";
	luaL_dofile(L, file.c_str());
    ////////////////////////////////////////////////////////   
    
    terrain = new Terrain();
    player = new Player();
    
    if (camera == NULL) {
        cout << "Error: Camera was not allocated in GameLevel class.\n";
        Sleep(5000);
        exit (1);
    }
    
    lplayer = player;
    lcamera = camera;
    lgameLevel = this;
    
    player->setSoundClass(snd);
    player->keyState = keyState;
    
    camera->setSoundClass(snd);
    camera->keyState = keyState;
    
    terrain->setSoundClass(snd);
    terrain->keyState = keyState;
    
    lplayer = player;
    lcamera = camera;
     
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
    // Attempt to execute the on_unload function only if the lua state has 
    // already been initialized with a script
    if (L != NULL) {
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
    }
    
    removeObjects();
    
    if (q != NULL)
        delete q;
        
    if (l != NULL)
        delete l;
    
    if (L != NULL)
        lua_close(L);
        
    terrain = NULL;
    player = NULL;
    camera = NULL;
    L = NULL;
    snd = NULL;   
}

//------------------------------------------------------------------------------
void GameLevel::removeObjects()
{
    if (terrain == NULL)
        return;
        
    ObjectNode* obj = terrain, *temp;

    while( obj != NULL ) {
        temp = obj->next;
        obj->remove();
        delete obj;
        
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
void GameLevel::animateText()
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
    terrain->save(filePath, &light);
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
