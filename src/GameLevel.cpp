#include <stdlib.h>

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

    terrain_ = NULL;

    player = NULL;
    camera = NULL;

    musicPath = "";
    scriptName = "";

    id = 0;

    music_ = new Music();
    objects_ = new ObjectList();

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
    drawObjects();

    // setup to draw outline ////////////////////
    glCallList( lists+1 );
    drawObjectOutlines();

    // reset draw mode to "normal"
    glCallList( lists+2 );

    if (grid) {
        terrain_->drawGrid();
        dynamic_cast<Camera*>(camera)->draw(camera);
    }

    //-if (bboxes)
        //-terrain->showCollisionBox();

    //-if (controlTriangles)
        //-terrain->showControlTriangle();

    //terrain->drawShadows( light );

    // Attempt to execute the script only if the lua state has already been
    // initialized with a script
    if (L == NULL)
        return;

    // Find the update function and call it
    lua_getglobal(L, "on_draw");

    // Push a pointer to the current object for use within the lua function
    lua_pushlightuserdata(L, (void*)terrain_);

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
    lua_pushlightuserdata(L, (void*)terrain_);

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
    lua_pushlightuserdata(L, (void*)terrain_);

	// Push the time passed since the last iteration of the game loop
    lua_pushnumber(L, elapsedTime);

    // Call the function with 2 argument and no return values
    lua_call(L, 2, 0);

    updateObjects( &lightDirection_ );

    displayList_->clearList();
    dynamic_cast<Camera*>(camera)->frustum.extractFromProjectionMatrix(dynamic_cast<Camera*>(camera)->final);
    quadTree_->buildDisplayList(displayList_, dynamic_cast<Camera*>(camera));
    terrain_->setDisplayList(displayList_);

    dynamic_cast<Camera*>(camera)->draw(camera);
}

//------------------------------------------------------------------------------
void GameLevel::prepareLevel()
{
    prepareObjects();
}

//------------------------------------------------------------------------------
void GameLevel::animateLevel()
{
    animateObjects(elapsedTime);
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

    terrain_ = new Terrain();
    player = new Player();

    objects_->insertFront(terrain_);

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
    lua_pushlightuserdata(L, (void*)terrain_);

    // Call the function with 1 argument and no return values
    lua_call(L, 1, 0);

    // get the result //
    //position.z = (float)lua_tonumber(L, -1);
    //position.y = (float)lua_tonumber(L, -2);
    //position.x = (float)lua_tonumber(L, -3);
    //lua_pop(L, 1);

    objects_->insertFront(player);
    ////////////////////////////////////////////

    cout << "building quad tree..." << endl;
    quadTree_->buildTree(terrain_);
    //q->traverseTree();

    cout << "building display list..." << endl;
    quadTree_->buildDisplayList(displayList_, dynamic_cast<Camera*>(camera));
    //q->buildLeafList(l);
    cout << "traverse list..." << endl;
    displayList_->traverseList();
    //Sleep(10000);
    //exit(1);

    cout << "finished building quad tree..." << endl;

    terrain_->setDisplayList(displayList_);

    cout << "finished..." << endl << endl;
    return (true);
}

//------------------------------------------------------------------------------
void GameLevel::setCamera( Camera* tCamera )
{ camera = (tCamera); lcamera = camera;}

//------------------------------------------------------------------------------
Terrain* GameLevel::getTerrain( void )
{ return( (Terrain*)terrain_ ); }

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
Object* GameLevel::findEnemyOfType( int type )
{
    float closest = 1000, temp;
    Object* obj = static_cast <Object*>(objects_->head);
    Object* retObj = NULL;

    while( obj->next != 0 ) {
        obj = (Object*)obj->next;
        //#if ( obj->checkActiveEnemy() )
        if (obj->getTypeId() == type && obj->getInView()) {
            temp = findDistance( (Object*)player, obj );
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
    if (L != NULL) {
        // Find the update function and call it
        lua_getglobal(L, "on_unload");

        // Push a pointer to the current object for use within the lua function
        lua_pushlightuserdata(L, (void*)terrain_);

        // Call the function with 1 argument and no return values
        lua_call(L, 1, 0);
    }

    cout << "before remove..." << endl;
    removeObjects();
    cout << "after remove..." << endl;

    if (quadTree_ != NULL)
        delete quadTree_;

    if (displayList_ != NULL)
        delete displayList_;

    if (L != NULL)
        lua_close(L);

    if (music_ != NULL)
        delete music_;

    if (objects_ != NULL)
        delete objects_;

    terrain_ = NULL;
    player = NULL;
    camera = NULL;
    L = NULL;
    music_ = NULL;
    objects_ = NULL;
}

//------------------------------------------------------------------------------
void GameLevel::removeObjects()
{
    ObjectNode *objectNode = objects_->head;
    while (objectNode != NULL) {
        objects_->remove(objectNode);
        delete objectNode;
        objectNode = objects_->head;
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
    terrain_->calcTerrainNorm(&lightDirection_);
}

void GameLevel::updateColor(int &x, int &z, float &red, float &green, float &blue)
{
    terrain_->setVertexColor(x, z, Vector(red, green, blue));

    //- this isn't optimized -- only do this when the height has changed and only
    // do it for the specified vertex
    terrain_->calcTerrainNorm(&lightDirection_);
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

void GameLevel::saveTerrain(char* filePath)
{
    terrain_->save(filePath, &lightDirection_);
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
//------------------------------------------------------------------------------
void GameLevel::drawObjects()
{
    for(Object *object = static_cast<Object*>(objects_->head);
        object != NULL;
        object = static_cast<Object*>(object->next)) {
        //if ( active == true && state != DEAD && isVisible == true)
        if (object->getActive() && object->getInView() && object->getDrawEnabled() && object->getState() != DEAD)
            object->draw( dynamic_cast<Camera*>(camera) );
    }
}

//------------------------------------------------------------------------------
void GameLevel::drawObjectOutlines()
{
    for(Object *object = static_cast<Object*>(objects_->head);
        object != NULL;
        object = static_cast<Object*>(object->next)) {
        //if ( active == true && state != DEAD && isVisible == true)
        if (object->getActive() && object->getInView() && object->getDrawEnabled() && object->getState() != DEAD)
            object->drawOutline( dynamic_cast<Camera*>(camera) );
    }
}

//------------------------------------------------------------------------------
void GameLevel::drawShadows( Vector* l )
{
    for(Object *object = static_cast<Object*>(objects_->head);
        object != NULL;
        object = static_cast<Object*>(object->next)) {
        //if ( active == true && state != DEAD && isVisible == true)
        if (object->getActive() && object->getInView() && object->getDrawEnabled() && object->getState() != DEAD)
            object->drawShadow(l);
    }
}
//------------------------------------------------------------------------------
void GameLevel::updateObjects( Vector* light  )
{
    for(Object *object = static_cast<Object*>(objects_->head); object != NULL;) {
         //if ( active == true && state != DEAD )
        //if (isInView && state != DEAD)
        if (object->getActive() && object->getState() != DEAD)
            object->update(light);

        Object* tempObject = NULL;

        if (object->next != NULL)
            tempObject = static_cast<Object*>(object->next);

        if (object->getState() == DEAD) {
            objects_->remove(object);


            freeObjects_[object->getScriptName()].insertFront(object);

            cout << "key: " << object->getScriptName() << " size: " << freeObjects_[object->getScriptName()].size << endl;


            //delete object;
        }

        object = tempObject;
    }
}

//------------------------------------------------------------------------------
void GameLevel::prepareObjects()
{
    for(Object *object = static_cast<Object*>(objects_->head);
        object != NULL;
        object = static_cast<Object*>(object->next)) {

        object->processCollisions( static_cast<Object*>(objects_->head));
    }
}

//------------------------------------------------------------------------------
void GameLevel::animateObjects(float timeElapsed)
{
    for(Object *object = static_cast<Object*>(objects_->head);
        object != NULL;
        object = static_cast<Object*>(object->next)) {

        if (object->getActive())
            object->animate(timeElapsed, dynamic_cast<Camera*>(camera));
    }
}

