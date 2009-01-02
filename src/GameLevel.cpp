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
    isComplete_ = false;
    
    terrain = NULL;

    player = NULL;
    camera = NULL;
    
    musicPath = "";
    scriptName = "";
    
    id = 0;
    
    //music_ = new Music();
    
    /////
#if EDIT
    grid = true;
#else
    grid = false;
#endif
    bboxes = false;
    controlTriangles = false;
    
    quadTree_ = new QuadTree();
    displayList_ = new DisplayList();
    
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
    
    //terrain->drawShadows( light );
    
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
  
    terrain->updateObjects( &lightDirection_ );

    displayList_->clearList();
    dynamic_cast<Camera*>(camera)->frustum.extractFromProjectionMatrix(dynamic_cast<Camera*>(camera)->final);
    quadTree_->buildDisplayList(displayList_, dynamic_cast<Camera*>(camera));
    terrain->l = displayList_;
    
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
    terrain->animateObjects(elapsedTime, dynamic_cast<Camera*>(camera));
}

//------------------------------------------------------------------------------
bool GameLevel::loadLevelLua( string file )
{   
    isComplete_ = false;
    id = 0;
    scriptName = file;
    
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
        exit (1);
    }
    
    lplayer = player;
    lcamera = camera;
    lgameLevel = this;
     
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
    quadTree_->buildTree(terrain);
    //q->traverseTree();  
       
    cout << "building display list..." << endl;
    quadTree_->buildDisplayList(displayList_, dynamic_cast<Camera*>(camera));
    //q->buildLeafList(l);
    cout << "traverse list..." << endl;
    displayList_->traverseList();
    //Sleep(10000);
    //exit(1);
                                      
    cout << "finished building quad tree..." << endl;
            
    terrain->l = displayList_;
   
    cout << "finished..." << endl << endl;
    return (true);
}

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
{ return isComplete_; }

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
    }
    
    removeObjects();
    
    music_->StopSong();
    
    if (quadTree_ != NULL)
        delete quadTree_;
        
    if (displayList_ != NULL)
        delete displayList_;
    
    if (L != NULL)
        lua_close(L);
     
    //if (music_ != NULL)
    //    delete music_;
        
    terrain = NULL;
    player = NULL;
    camera = NULL;
    L = NULL;
    //music_ = NULL;   
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
void GameLevel::updateTerrain(int &x, int &z, float &height, int &type, float &red, float &green, float &blue)
{    
    if ( x >= 0 && x < terrain->xSize && z >= 0 && z <= terrain->zSize ) {
        terrain->vertex[x][z][1]= height;
        terrain->color[x][z][0] = red;
        terrain->color[x][z][1] = green;
        terrain->color[x][z][2] = blue;
        terrain->type[x][z] = type;
        
        terrain->calcTerrainNorm(&lightDirection_);
    }
}

void GameLevel::updateColor(int &x, int &z, float &red, float &green, float &blue)
{     
    if ( x >= 0 && x < terrain->xSize && z >= 0 && z <= terrain->zSize ) {
        terrain->color[x][z][0] = red;
        terrain->color[x][z][1] = green;
        terrain->color[x][z][2] = blue;
        
        terrain->calcTerrainNorm(&lightDirection_);
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
    terrain->save(filePath, &lightDirection_);
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
