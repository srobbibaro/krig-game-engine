#include "GameLevel.h"
#include "Snow.h"

//------------------------------------------------------------------------------
GameLevel::GameLevel( unsigned int tLists)
{ 
    lists = tLists;
    complete = false;
    
    terrain = NULL;
    obj = NULL;
    player = NULL;
    camera = NULL;
    boss = NULL;
    camera = NULL;
    weather = NULL;
    time = 0;
    
    
    /////
    grid = true;
    bboxes = false;
    controlTriangles = false;
    
    q = new QuadTree();
    l = new DisplayList();
}

//------------------------------------------------------------------------------
GameLevel::~GameLevel()
{
    if ( terrain != NULL )
        delete terrain;
    if ( camera != NULL )
        delete camera;
    if ( weather != NULL )
        delete weather; 
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
    
    if (weatherEffect > 0)
        weather->draw();
    
    if (grid) {
        terrain->drawGrid();    
        dynamic_cast<Camera*>(camera)->draw();
    }
    
    if (bboxes)
        terrain->showCollisionBox();
        
    if (controlTriangles)
    terrain->showControlTriangle();
    
   /*
    drawText();
    */
    // draw weather effects //
             
    //terrain->drawShadows( light );
}

//------------------------------------------------------------------------------
void GameLevel::updateLevel()
{
    terrain->updateObjects( &light );

    l->clearList();
    dynamic_cast<Camera*>(camera)->frustum.extractFromProjectionMatrix(dynamic_cast<Camera*>(camera)->final);
    q->buildDisplayList(l, dynamic_cast<Camera*>(camera));
    terrain->l = l;
    
    dynamic_cast<Camera*>(camera)->draw();
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

    if (weatherEffect > 0)
        weather->update(timeElapsed);
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
        
    // initialize Lua 
    lua_State* L = lua_open();
    
    if (L == NULL) {
        printf("Error creating Lua state.\n");
		exit(-1);
    }

	// load Lua base libraries 
	luaL_openlibs(L);
  
    // load the script 
    cout << "Loading Lua script (level file): --" << file << "--\n";
	luaL_dofile(L, file.c_str());
	
	// Read the number of objects
    lua_getglobal(L, "numObjects");
    numObjects = (int)lua_tonumber(L, -1);
    lua_pop(L, 1);
    cout << "Number of objects in " << file << ": " << numObjects << endl;
    
    // Load the light direction
    lua_getglobal(L, "lightDirection"); 
    loadVector(L, &light); 
    light.normalize();
    cout << "Light direction: " << light.x << " " << light.y << " " << light.z << endl;
     
    // Read the name of the terrain file  
    lua_getglobal(L, "terrain");
    const char *j = lua_tostring(L, -1);
    string terrainPath = "./terrains/" + string(j);
    cout << "Terrain: " << terrainPath << endl;
    lua_pop(L, 1);
    
    // Load the terrain
    terrain->loadTerrain( terrainPath.c_str(), &light );
    ////////////////////////////////////////////////////////   
    
    // load in the sky (and weather type) //////////////////
    lua_getglobal(L, "weatherEffect");
    weatherEffect = (int)lua_tonumber(L, -1);
    if (weatherEffect < 0 || weatherEffect > 2)
         weatherEffect = 0;    
    cout << "Weather effect: " << weatherEffect << endl;
    lua_pop(L, 1); 
      
    lua_getglobal(L, "bgColor");
    for (int i = 0; i < 3; i++) { 
        for (int j = 0; j < 3; j++) {     
                lua_pushnumber(L, (i*3)+j+1);
                lua_gettable(L, -2);
                bgcolor[i][j] = (float)lua_tonumber(L, -1);
                lua_pop(L, 1);
        }
        cout << "bgColor i=" << i << " value=" 
             << bgcolor[i][0] << "," << bgcolor[i][1] << "," << bgcolor[i][2] << endl;
    }    
    ////////////////////////////////////////////////////////
    
    // Read the song name to play
    lua_getglobal(L, "bgMusic");
    const char *k = lua_tostring(L, -1);
    musicPath = "./music/" + string(k);
    cout << "bgMusic: " << musicPath << endl;
    lua_pop(L, 1);
    
    // orient camera's initial setup ///////////////////////
    cout << "Setting up Camera..." << endl;
    lua_getglobal(L, "cameraPosition"); 
    loadVector(L, &position); 
    lua_getglobal(L, "cameraRotation");
    loadVector(L, &rotation);
    
    lua_getglobal(L, "cameraScript"); 
    const char *t = lua_tostring(L, -1);
    string script = "./scripts/" + string(t);
     
    //z = -z;                                      // camera correction 
    camera->setRotationEuler( rotation.x, rotation.y, rotation.z );
    camera->setPosition( position.x, position.y, position.z );
    camera->setTimer( &time );   
    //////////////////////////////////////////////
    
    player->unloadScript();
    player->init();
    camera->unloadScript();
    camera->init();
    
    // load each object //////////////////////////////////////
    lua_getglobal(L, "objects");
    for ( int i = 0; i < numObjects; i++ ) {
        loadObject(L, i+1);
    }
                   
    camera->setPlayerPtr(player);
    camera->setCameraPtr(camera);
    camera->loadScript(script);
        
    // setup the weather effect ////////////////
    if (weatherEffect > 0) {
        weather = new Snow(camera);
        weather->update(4.0f);
    }
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
     lua_close(L);
     L = NULL;
     
     return (true);
}

void GameLevel::loadVector(lua_State* L, Vector *v)
{
    // x
    lua_pushnumber(L, 1);
    lua_gettable(L, -2);
    v->x = (float)lua_tonumber(L, -1);
    lua_pop(L, 1);
    
    // y
    lua_pushnumber(L, 2);
    lua_gettable(L, -2);
    v->y = (float)lua_tonumber(L, -1);
    lua_pop(L, 1);
    
    // z
    lua_pushnumber(L, 3);
    lua_gettable(L, -2);
    v->z = (float)lua_tonumber(L, -1);
    lua_pop(L, 1);
}

void GameLevel::loadObject(lua_State* L, int number) 
{
    cout << "number: " << number << endl;
    
    lua_pushnumber(L, number);
    lua_gettable(L, -2);
    
    lua_pushstring(L, "type");
    lua_gettable(L, -2);
    int objectType = (int)lua_tonumber(L, -1);
    cout << "type: " << objectType << endl;
    lua_pop(L, 1);
   
    
    lua_pushstring(L, "modelKey");
    lua_gettable(L, -2);
    const char *s = lua_tostring(L, -1);
    string modelKey = string(s);
    cout << "Model Key: " << modelKey << endl;
    lua_pop(L, 1);
    
    
    lua_pushstring(L, "script");
    lua_gettable(L, -2);
    const char *t = lua_tostring(L, -1);
    string script = "./scripts/" + string(t);
    cout << "Script: " << script << endl;
    lua_pop(L, 1);
    
    Vector position, rotation, scale;
    
    lua_pushstring(L, "scale");
    lua_gettable(L, -2);
    loadVector(L, &scale);
    cout << "Scale: " << scale.x << " " << scale.y << " " << scale.z << endl;
    lua_pop(L, 1);
    
    lua_pushstring(L, "rotation");
    lua_gettable(L, -2);
    loadVector(L, &rotation);
    cout << "Rotation: " << rotation.x << " " << rotation.y << " " << rotation.z << endl;
    lua_pop(L, 1);
    
    lua_pushstring(L, "position");
    lua_gettable(L, -2);
    loadVector(L, &position);
    cout << "Position: " << position.x << " " << position.y << " " << position.z << endl;
    lua_pop(L, 1);
        
    if ( number == 1 ) {
        // object[0] must be player
        objectType = OBJECT_PLAYER;
    }
    else if ( number == 2 ) {
        // object[1] must be boss
        objectType = OBJECT_BOSS;
    }
    else {
        if ( objectType == OBJECT_PLAYER || objectType == OBJECT_BOSS ) {
            objectType = -1;
        }
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
       case OBJECT_ENEMY_SHIP:  // Enemy ship
            obj = new EnemyShip( modelKey, script, &time, dynamic_cast<Player*>(player) );
            break;
       case OBJECT_ASTEROID: // Asteroid
            obj = new Asteroid( modelKey, script, &time );
            break;
       case OBJECT_BEAM: // Beam
            obj = new Beam( modelKey, script, &time );
            break;
       case OBJECT_SAIL_BOAT: // Sail Boat
            obj = new SailBoat( modelKey, script, &time );
            break;
       case OBJECT_BOATCANNON: // Boat Cannon
            obj = new BoatCannon( modelKey, script, &time, (player) );
            break;                    
       case OBJECT_BOSS:
            // there MUST BE EXACTLY ONE boss
            if (boss == NULL) {
                obj = new Boss ( modelKey, script, &time, dynamic_cast<Player*>(player), &complete );                    
                boss = (Boss*)obj;
            }      
            break;      
       case OBJECT_POWERUP_1:
            obj = new Powerup(1, &time);
            break;
       case OBJECT_POWERUP_2:
            obj = new Powerup(2, &time);
            break;                                       
       default:
            break;
    }
        
    if (obj != NULL) {
        // Set the object's orientation
        obj->setRotationEuler( rotation.x, rotation.y, rotation.z);
        obj->setPosition( position.x, position.y, position.z );
        obj->setScaleFactor( scale.x, scale.y, scale.z );
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
Boss* GameLevel::returnBoss( void )
{ return boss; }

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
        if ( obj->checkActiveEnemy() )
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
    if ( weather != NULL )
        delete weather;    
    
    boss = NULL;
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
