#include <cstdlib>
#include <cstring>

#include "GameLevel.h"
#include "ScriptedObject.h"
#include "Scripting.h"

#include "LuaGL.h"

Object* g_script_camera;
Object* g_script_player;

extern int luaopen_opengl (lua_State *L);

//------------------------------------------------------------------------------
GameLevel::GameLevel( unsigned int lists)
{
    lists_ = lists;
    isComplete_ = false;

    terrain_ = NULL;

    player_ = NULL;
    camera_ = NULL;

    musicPath_ = "";
    scriptName_ = "";

    id_ = 0;

    /////
#if EDIT
    grid_ = true;
#else
    grid_ = false;
#endif
    bboxes_ = false;
    controlTriangles_ = false;

    luaState_ = NULL;

    elapsedTime_ = 0.0f;
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
    glCallList( lists_ );

    // draw scene and shade /////////////////////
    drawObjects();

    // setup to draw outline ////////////////////
    glCallList( lists_+1 );
    drawObjectOutlines();

    // reset draw mode to "normal"
    glCallList( lists_+2 );

    dynamic_cast<Camera*>(camera_)->draw(camera_);

    if (grid_) {
        terrain_->drawGrid();
    }

    if (bboxes_)
        ((Object*)objects_.head)->showCollisionBox();

    if (controlTriangles_)
        ((Object*)objects_.head)->showControlTriangle();

    //terrain->drawShadows( light );

    // Attempt to execute the script only if the lua state has already been
    // initialized with a script
    if (luaState_ == NULL)
        return;

    // Find the update function and call it
    lua_getglobal(luaState_, "on_draw");

    // Push a pointer to the current object for use within the lua function
    lua_pushlightuserdata(luaState_, (void*)terrain_);

	// Push the time passed since the last iteration of the game loop
    lua_pushnumber(luaState_, elapsedTime_);

    // Call the function with 2 argument and no return values
    lua_call(luaState_, 2, 0);
}

//------------------------------------------------------------------------------
void GameLevel::postDraw()
{
    // Attempt to execute the script only if the lua state has already been
    // initialized with a script
    if (luaState_ == NULL)
        return;

    // Find the update function and call it
    lua_getglobal(luaState_, "on_draw_screen");

    // Push a pointer to the current object for use within the lua function
    lua_pushlightuserdata(luaState_, (void*)terrain_);

	// Push the time passed since the last iteration of the game loop
    lua_pushnumber(luaState_, elapsedTime_);

    // Call the function with 2 argument and no return values
    lua_call(luaState_, 2, 0);
}

//------------------------------------------------------------------------------
void GameLevel::updateLevel()
{
    // Attempt to execute the script only if the lua state has already been
    // initialized with a script
    if (luaState_ == NULL)
        return;

    // Find the update function and call it
    lua_getglobal(luaState_, "on_update");

    // Push a pointer to the current object for use within the lua function
    lua_pushlightuserdata(luaState_, (void*)terrain_);

	// Push the time passed since the last iteration of the game loop
    lua_pushnumber(luaState_, elapsedTime_);

    // Call the function with 2 argument and no return values
    lua_call(luaState_, 2, 0);

    updateObjects( &lightDirection_ );

    displayList_.clearList();
    dynamic_cast<Camera*>(camera_)->frustum.extractFromProjectionMatrix(dynamic_cast<Camera*>(camera_)->final);
    quadTree_.buildDisplayList(&displayList_, dynamic_cast<Camera*>(camera_));
    terrain_->setDisplayList(&displayList_);

    dynamic_cast<Camera*>(camera_)->draw(camera_);
}

//------------------------------------------------------------------------------
void GameLevel::prepareLevel()
{
    prepareObjects();
}

//------------------------------------------------------------------------------
void GameLevel::animateLevel()
{
    animateObjects(elapsedTime_);
}

//------------------------------------------------------------------------------
bool GameLevel::loadLevelLua( string file )
{
    isComplete_ = false;
    id_ = 0;
    scriptName_ = file;

    // If the lua state has not been initialized for this object, attempt to
    // initialize it.
    if (file == "" || luaState_ != NULL)
        return false;

    luaState_ = lua_open();

    // If the lua state still could not be initialized, then exit the game.
    // ... we can do something smarter with this in the finished product.
    if (luaState_ == NULL) {
        PRINT_ERROR("Could not create Lua state.\n");
	    exit(-1);
    }

	// load Lua base libraries
	luaL_openlibs(luaState_);

	luaopen_opengl(luaState_);

    // Register our functions for use in lua (currently defined in
    // Object.h)
    registerFunctions(luaState_, 1);

    // load the script
    PRINT_DEBUG("Loading Lua level script '%s'...\n", file.c_str());

	luaL_dofile(luaState_, file.c_str());
    ////////////////////////////////////////////////////////

    terrain_ = new Terrain();
    player_ = new Player();

    PRINT_DEBUG("Terrain and Player allocated.\n");

    objects_.insertFront(terrain_);

    PRINT_DEBUG("Terrain stored in objects list.\n");

    if (camera_ == NULL) {
        PRINT_ERROR("Camera was not allocated in GameLevel class.\n");
        exit (1);
    }

    g_script_player = player_;
    g_script_camera = camera_;
    g_script_game_level = this;

    // Find the update function and call it
    PRINT_DEBUG("Calling Lua level script 'on_load' function...\n");

    lua_getglobal(luaState_, "on_load");

    // Push a pointer to the current object for use within the lua function
    lua_pushlightuserdata(luaState_, (void*)terrain_);

    // Call the function with 1 argument and no return values
    lua_call(luaState_, 1, 0);

    PRINT_DEBUG("Lua level script 'on_load' function complete.\n");

    // get the result //
    //position.z = (float)lua_tonumber(luaState_, -1);
    //position.y = (float)lua_tonumber(luaState_, -2);
    //position.x = (float)lua_tonumber(luaState_, -3);
    //lua_pop(luaState_, 1);

    objects_.insertFront(player_);
    ////////////////////////////////////////////

    PRINT_DEBUG("Building quad tree...\n");
    quadTree_.buildTree(terrain_);
    //q->traverseTree();

    PRINT_DEBUG("Building display list...\n");

    quadTree_.buildDisplayList(&displayList_, dynamic_cast<Camera*>(camera_));
    //q->buildLeafList(luaState_);

    PRINT_DEBUG("Traversing list...\n");
    displayList_.traverseList();

    PRINT_DEBUG("Finished building quad tree.\n");
    terrain_->setDisplayList(&displayList_);
    PRINT_DEBUG("Finished loading level.\n\n");

    return (true);
}

//------------------------------------------------------------------------------
bool GameLevel::loadLevelFromBufferLua( const char* buffer )
{
    isComplete_ = false;
    id_ = 0;
    //scriptName_ = file;

    // If the lua state has not been initialized for this object, attempt to
    // initialize it.
    if (buffer == "" || luaState_ != NULL)
        return false;

    luaState_ = lua_open();

    // If the lua state still could not be initialized, then exit the game.
    // ... we can do something smarter with this in the finished product.
    if (luaState_ == NULL) {
        PRINT_ERROR("Could not create Lua state.\n");
	    exit(-1);
    }

	// load Lua base libraries
	luaL_openlibs(luaState_);

	luaopen_opengl(luaState_);

    // Register our functions for use in lua (currently defined in
    // Object.h)
    registerFunctions(luaState_, 1);

    // load the script
    PRINT_DEBUG("Loading Lua level script from buffer...\n");

	//uaL_dofile(luaState_, file.c_str());
	luaL_loadbuffer(luaState_, buffer, strlen(buffer), "line") ||
                lua_pcall(luaState_, 0, 0, 0);

    ////////////////////////////////////////////////////////

    terrain_ = new Terrain();
    player_ = new Player();

    PRINT_DEBUG("Terrain and Player allocated.\n");

    objects_.insertFront(terrain_);

    PRINT_DEBUG("Terrain stored in objects list.\n");

    if (camera_ == NULL) {
        PRINT_ERROR("Camera was not allocated in GameLevel class.\n");
        exit (1);
    }

    g_script_player = player_;
    g_script_camera = camera_;
    g_script_game_level = this;

    // Find the update function and call it
    PRINT_DEBUG("Calling Lua level script 'on_load' function...\n");

    lua_getglobal(luaState_, "on_load");

    // Push a pointer to the current object for use within the lua function
    lua_pushlightuserdata(luaState_, (void*)terrain_);

    // Call the function with 1 argument and no return values
    lua_call(luaState_, 1, 0);

    PRINT_DEBUG("Lua level script 'on_load' function complete.\n");

    // get the result //
    //position.z = (float)lua_tonumber(luaState_, -1);
    //position.y = (float)lua_tonumber(luaState_, -2);
    //position.x = (float)lua_tonumber(luaState_, -3);
    //lua_pop(luaState_, 1);

    objects_.insertFront(player_);
    ////////////////////////////////////////////

    PRINT_DEBUG("Building quad tree...\n");

    quadTree_.buildTree(terrain_);
    //q->traverseTree();

    PRINT_DEBUG("Building display list...\n");

    quadTree_.buildDisplayList(&displayList_, dynamic_cast<Camera*>(camera_));
    //q->buildLeafList(luaState_);

    PRINT_DEBUG("Traversing list...\n");

    displayList_.traverseList();
    //sleep(10000);
    //exit(1);

    PRINT_DEBUG("Finished building quad tree.\n");

    terrain_->setDisplayList(&displayList_);

    PRINT_DEBUG("Finished loading level.\n\n");

    return (true);
}

//------------------------------------------------------------------------------
void GameLevel::setCamera( Camera* camera )
{ camera_ = camera; g_script_camera = camera_;}

//------------------------------------------------------------------------------
Terrain* GameLevel::getTerrain( void )
{ return( (Terrain*)terrain_ ); }

//------------------------------------------------------------------------------
Camera* GameLevel::getCamera( void )
{ return( (Camera*)camera_ ); }

//------------------------------------------------------------------------------
Player* GameLevel::getPlayer( void )
{ return( (Player*)player_ ); }

//------------------------------------------------------------------------------
void GameLevel::drawSky()
{
    //glPushMatrix();
        // -front sky //
        glBegin(GL_TRIANGLE_STRIP);
            glColor3fv ( bgcolor_[0]);
            glVertex3f (-1.0f, 1.0f, -1.0f);
            glColor3fv ( bgcolor_[1]);
            glVertex3f (-1.0f, 0.0f, -1.0f);
            glColor3fv ( bgcolor_[0]);
            glVertex3f (1.0f, 1.0f, -1.0f);
            glColor3fv ( bgcolor_[1]);
            glVertex3f (1.0f, 0.0f, -1.0f);
        glEnd();
        glBegin(GL_TRIANGLE_STRIP);
            glVertex3f (-1.0f, 0.0f, -1.0f);
            glColor3fv ( bgcolor_[2]);
            glVertex3f (-1.0f, -1.0f, -1.0f);
            glColor3fv ( bgcolor_[1]);
            glVertex3f (1.0f, 0.0f, -1.0f);
            glColor3fv ( bgcolor_[2]);
            glVertex3f (1.0f, -1.0f, -1.0f);
        glEnd();

        // left sky //
        glBegin(GL_TRIANGLE_STRIP);
            glColor3fv ( bgcolor_[0]);
            glVertex3f (-1.0f, 1.0f, 1.0f);
            glColor3fv ( bgcolor_[1]);
            glVertex3f (-1.0f, 0.0f, 1.0f);
            glColor3fv ( bgcolor_[0]);
            glVertex3f (-1.0f, 1.0f, -1.0f);
            glColor3fv ( bgcolor_[1]);
            glVertex3f (-1.0f, 0.0f, -1.0f);
        glEnd();
        glBegin(GL_TRIANGLE_STRIP);
            glVertex3f (-1.0f, 0.0f, 1.0f);
            glColor3fv ( bgcolor_[2]);
            glVertex3f (-1.0f, -1.0f, 1.0f);
            glColor3fv ( bgcolor_[1]);
            glVertex3f (-1.0f, 0.0f, -1.0f);
            glColor3fv ( bgcolor_[2]);
            glVertex3f (-1.0f, -1.0f, -1.0f);
        glEnd();

        // right sky //
        glBegin(GL_TRIANGLE_STRIP);
            glColor3fv ( bgcolor_[0]);
            glVertex3f (1.0f, 1.0f, -1.0f);
            glColor3fv ( bgcolor_[1]);
            glVertex3f (1.0f, 0.0f, -1.0f);
            glColor3fv ( bgcolor_[0]);
            glVertex3f (1.0f, 1.0f, 1.0f);
            glColor3fv ( bgcolor_[1]);
            glVertex3f (1.0f, 0.0f, 1.0f);
        glEnd();
        glBegin(GL_TRIANGLE_STRIP);
            glVertex3f (1.0f, 0.0f, -1.0f);
            glColor3fv ( bgcolor_[2]);
            glVertex3f (1.0f, -1.0f, -1.0f);
            glColor3fv ( bgcolor_[1]);
            glVertex3f (1.0f, 0.0f, 1.0f);
            glColor3fv ( bgcolor_[2]);
            glVertex3f (1.0f, -1.0f, 1.0f);
        glEnd();

        // -back sky //
        glBegin(GL_TRIANGLE_STRIP);
            glColor3fv ( bgcolor_[0]);
            glVertex3f (1.0f, 1.0f, 1.0f);
            glColor3fv ( bgcolor_[1]);
            glVertex3f (1.0f, 0.0f, 1.0f);
            glColor3fv ( bgcolor_[0]);
            glVertex3f (-1.0f, 1.0f, 1.0f);
            glColor3fv ( bgcolor_[1]);
            glVertex3f (-1.0f, 0.0f, 1.0f);

        glEnd();
        glBegin(GL_TRIANGLE_STRIP);
            glVertex3f (1.0f, 0.0f, 1.0f);
            glColor3fv ( bgcolor_[2]);
            glVertex3f (1.0f, -1.0f, 1.0f);
            glColor3fv ( bgcolor_[1]);
            glVertex3f (-1.0f, 0.0f, 1.0f);
            glColor3fv ( bgcolor_[2]);
            glVertex3f (-1.0f, -1.0f, 1.0f);
        glEnd();


        // top sky //
        glBegin(GL_TRIANGLE_STRIP);
            glColor3fv ( bgcolor_[0]);
            glVertex3f (-1.0f, 1.0f, -1.0f);
            glVertex3f (1.0f, 1.0f, -1.0f);
            glVertex3f (-1.0f, 1.0f, 1.0f);
            glVertex3f (1.0f, 1.0f, 1.0f);
        glEnd();

        // bottom sky //
        glBegin(GL_TRIANGLE_STRIP);
            glColor3fv ( bgcolor_[2]);
            glVertex3f (1.0f, -1.0f, -1.0f);
            glVertex3f (-1.0f, -1.0f, -1.0f);
            glVertex3f (1.0f, -1.0f, 1.0f);
            glVertex3f (-1.0f, -1.0f, 1.0f);
        glEnd();
    //glPopMatrix();
}

//------------------------------------------------------------------------------
Object* GameLevel::findEnemyOfType( int type )
{
    float closest = 1000, temp;
    Object* obj = static_cast <Object*>(objects_.head);
    Object* retObj = NULL;

    while( obj->next != 0 ) {
        obj = (Object*)obj->next;
        // TODO: Does the enemy need to be active?
        //if ( obj->checkActiveEnemy() )
        if (obj->getTypeId() == type && obj->getInView()) {
            temp = findDistance( (Object*)player_, obj );
            if ( temp < closest ) {
                retObj = obj;
                closest = temp;
            }
        }
    }

    return (retObj);
}

//------------------------------------------------------------------------------
float GameLevel::findDistance ( Object* obj1, Object* obj2 )
{
    Vector position1 = obj1->getPosition();
    Vector position2 = obj2->getPosition();

    return( sqrtf( ( position1.x - position2.x)*( position1.x - position2.x)+
          ( position1.y - position2.y)*( position1.y - position2.y)+
          ( position1.z - position2.z)*( position1.z - position2.z) ) );
}

//------------------------------------------------------------------------------
bool GameLevel::checkComplete(void)
{ return isComplete_; }

//------------------------------------------------------------------------------
void GameLevel::unloadLevel()
{
    // Attempt to execute the on_unload function only if the lua state has
    // already been initialized with a script
    if (luaState_ != NULL) {
        // Find the update function and call it
        lua_getglobal(luaState_, "on_unload");

        // Push a pointer to the current object for use within the lua function
        lua_pushlightuserdata(luaState_, (void*)terrain_);

        // Call the function with 1 argument and no return values
        lua_call(luaState_, 1, 0);
    }

    PRINT_DEBUG("Removing objects...\n");
    removeObjects();
    PRINT_DEBUG("done.\n");

    if (luaState_ != NULL)
        lua_close(luaState_);

    terrain_ = NULL;
    player_ = NULL;
    camera_ = NULL;
    luaState_ = NULL;
}

//------------------------------------------------------------------------------
void GameLevel::removeObjects()
{
    ObjectNode *objectNode = objects_.head;
    while (objectNode != NULL) {
        objects_.remove(objectNode);
        delete objectNode;
        objectNode = objects_.head;
    }
}

//------------------------------------------------------------------------------
void GameLevel::updateTerrain(int &x, int &z, float &height, int &type, float &red, float &green, float &blue)
{
    terrain_->setVertexHeight(x, z,height);
    terrain_->setVertexColor(x, z, Vector(red, green, blue));
    terrain_->setVertexType(x, z, type);

    //- this isn't optimized -- only do this when the height has changed and only
    // do it for the specified vertex
    terrain_->calcViewableTerrainNorm();
}

void GameLevel::updateColor(int &x, int &z, float &red, float &green, float &blue)
{
    terrain_->setVertexColor(x, z, Vector(red, green, blue));

    //- this isn't optimized -- only do this when the height has changed and only
    // do it for the specified vertex
    //terrain_->calcTerrainNorm(&lightDirection_);
}

void GameLevel::getTerrainInfo(int &x, int &z, float &height, int &type, float &red, float &green, float &blue)
{
    height = terrain_->getVertexHeight(x, z);
    type = terrain_->getVertexType(x, z);
    Vector color = terrain_->getVertexColor(x, z);

    red = color.x;
    green = color.y;
    blue = color.z;
}

void GameLevel::saveTerrain(const char* filePath)
{
    terrain_->save(filePath, &lightDirection_);
}

void GameLevel::toggleGrid(void) { grid_ = !grid_;}
void GameLevel::toggleBoundingBoxes(void) { bboxes_ = !bboxes_; }
void GameLevel::toggleControlTriangles(void) { controlTriangles_ = !controlTriangles_; }

void GameLevel::setSkyBox(float bgcolor[][3], int x, int y)
{
    for (int i = 0; i < y; i++) {
        for (int j = 0; j < x; j++) {
            bgcolor_[i][j] = bgcolor[i][j];
        }
    }
}
//------------------------------------------------------------------------------
void GameLevel::drawObjects()
{
    for(Object *object = static_cast<Object*>(objects_.head);
        object != NULL;
        object = static_cast<Object*>(object->next)) {
        //if ( active_ == true && state_ != DEAD && isVisible == true)
        if (object->getActive() && object->getInView() && object->getDrawEnabled() && object->getState() != DEAD)
            object->draw( dynamic_cast<Camera*>(camera_) );
    }
}

//------------------------------------------------------------------------------
void GameLevel::drawObjectOutlines()
{
    for(Object *object = static_cast<Object*>(objects_.head);
        object != NULL;
        object = static_cast<Object*>(object->next)) {
        //if ( active_ == true && state_ != DEAD && isVisible == true)
        if (object->getActive() && object->getInView() && object->getDrawEnabled() && object->getState() != DEAD)
            object->drawOutline( dynamic_cast<Camera*>(camera_) );
    }
}

//------------------------------------------------------------------------------
void GameLevel::drawShadows( Vector* l )
{
    for(Object *object = static_cast<Object*>(objects_.head);
        object != NULL;
        object = static_cast<Object*>(object->next)) {
        //if ( active_ == true && state_ != DEAD && isVisible == true)
        if (object->getActive() && object->getInView() && object->getDrawEnabled() && object->getState() != DEAD)
            object->drawShadow(l);
    }
}
//------------------------------------------------------------------------------
void GameLevel::updateObjects( Vector* light  )
{
    for(Object *object = static_cast<Object*>(objects_.head); object != NULL;) {
         //if ( active_ == true && state_ != DEAD )
        //if (isInView_ && state_ != DEAD)
        if (object->getActive() && object->getState() != DEAD)
            object->update(light);

        Object* tempObject = NULL;

        if (object->next != NULL)
            tempObject = static_cast<Object*>(object->next);

        if (object->getState() == DEAD) {
            objects_.remove(object);
            freeObjects_[object->getScriptName()].insertFront(object);

            PRINT_DEBUG_LVL(
                2,
                "Object removed: script='%s'. Added to free objects map (size=%d).\n",
                object->getScriptName().c_str(),
                freeObjects_[object->getScriptName()].size
            );
            //delete object;
        }

        object = tempObject;
    }
}

//------------------------------------------------------------------------------
void GameLevel::prepareObjects()
{
    for(Object *object = static_cast<Object*>(objects_.head);
        object != NULL;
        object = static_cast<Object*>(object->next)) {

        object->processCollisions( static_cast<Object*>(objects_.head));
    }
}

//------------------------------------------------------------------------------
void GameLevel::animateObjects(float timeElapsed)
{
    for(Object *object = static_cast<Object*>(objects_.head);
        object != NULL;
        object = static_cast<Object*>(object->next)) {

        if (object->getActive())
            object->animate(timeElapsed, dynamic_cast<Camera*>(camera_));
    }
}
