//////////////////////////////////////////////////////////////////
// Authors     : Steve Robbibaro     sar2                       //
//             : Andrew Yochum       amyst14                    //
//                                                              //
// Filename    : objects.cpp                                    //
//                                                              //
// Description : Functions displaying models and storing their  //
//               transformation data                            //
//                                                              //
// Date        : 12-1-03                                        //
//////////////////////////////////////////////////////////////////

#include "Object.h"
#include "Scripting.h"
#include "Snow.h"

GLint numObjects;
Object* object;

//------------------------------------------------------------------------------
Object::Object() 
: ObjectNode()
{
    scale.setVector( 1.0f, 1.0f, 1.0f );
    scaleRate.setVector( 0.0f, 0.0f, 0.0f );
    
    velocity.setVector(0.0f, 0.0f, 0.0f);
    position.setVector(0.0f, 0.0f, 0.0f);
    setRotationEuler(0.0f, 0.0f, 0.0f);
    
    state = NORMAL; 
    active = false;
    //#animStartTime = 0.0f;
    isVisible = true;
    s = NULL;
    collision = false;
    //#isEnemy = false;
       
    //#testActiveZoneEnable = true;
    
    particleSystem = NULL;
    L = NULL;
    
    typeId = 0;
    
    init();
}

//------------------------------------------------------------------------------
Object::~Object()
{
    s = NULL;
    
     if (L != NULL)
        lua_close(L);
        
    if (particleSystem != NULL)
        delete particleSystem;
    
    particleSystem = NULL;
}

 
//------------------------------------------------------------------------------
void Object::setScale( GLfloat x, GLfloat y, GLfloat z )
{ scale.setVector( x, y, z ); }

//------------------------------------------------------------------------------
void Object::setScale( const Vector &v )
{
    scale = v;
}

//------------------------------------------------------------------------------
void Object::setState( unsigned char tState )
{ state = tState; }

//------------------------------------------------------------------------------
void Object::setScaleRate( GLfloat x, GLfloat y, GLfloat z )
{
    scaleRate.setVector( x, y, z );
}

//------------------------------------------------------------------------------
void Object::setScaleRate( const Vector &v )
{
    scaleRate = v;
}

//------------------------------------------------------------------------------
void Object::setSoundClass ( Sound *snd )
{ s = snd; }

//------------------------------------------------------------------------------
void Object::drawObjects( Object* c )
{
    if ( active == true && state != DEAD && isVisible == true )
        draw(c);
    
    if ( next != NULL )
        ((Object*)next)->drawObjects(c);
}
    
//------------------------------------------------------------------------------
void Object::drawObjectOutlines( Object* c )
{
    if ( active == true && state != DEAD && isVisible == true)
        drawOutline( c );
    
    if ( next != NULL )
    {    
        ((Object*)next)->drawObjectOutlines(c);
    }
}

//------------------------------------------------------------------------------
void Object::drawShadows( Vector* l )
{
    if ( active == true && state != DEAD && isVisible == true )
        drawShadow( l );
    
    if ( next != NULL )
    {    
        ((Object*)next)->drawShadows(l);
    }
}
//------------------------------------------------------------------------------
void Object::updateObjects( Vector* light  )
{
    if ( active == true && state != DEAD )
        update( light );
    
    if ( next != NULL )
        ((Object*)next)->updateObjects( light );
        
    if ( state == DEAD )
    {
        remove();
        delete this;
    }
}

//------------------------------------------------------------------------------
void Object::processCollisions( Object* temp )
{
    Vector boxA[2];
    Vector boxB[2];  
		
    boxA[0] = collisionBox[0];
    boxA[1] = collisionBox[1];
    boxB[0].setVector(temp->collisionBox[0].x, temp->collisionBox[0].y, temp->collisionBox[0].z );
    boxB[1].setVector(temp->collisionBox[1].x, temp->collisionBox[1].y, temp->collisionBox[1].z );

    if ( (Object*)this != temp && active == true && state == NORMAL && (collision == true || temp->state != INVUL ) 
    && this != NULL && temp != NULL )
    {
        if(
            ((boxA[0].x >= boxB[0].x && boxA[0].x <= boxB[1].x) ||
            (boxA[1].x >= boxB[0].x && boxA[1].x <= boxB[1].x)  ||
            (boxB[0].x >= boxA[0].x && boxB[0].x <= boxA[1].x)  ||
            (boxB[1].x >= boxA[0].x && boxB[1].x <= boxA[1].x)) &&
            ((boxA[0].y >= boxB[0].y && boxA[0].y <= boxB[1].y) ||
            (boxA[1].y >= boxB[0].y && boxA[1].y <= boxB[1].y)  ||
            (boxB[0].y >= boxA[0].y && boxB[0].y <= boxA[1].y)  ||
            (boxB[1].y >= boxA[0].y && boxB[1].y <= boxA[1].y)) &&
            ((boxA[0].z >= boxB[0].z && boxA[0].z <= boxB[1].z) ||
            (boxA[1].z >= boxB[0].z && boxA[1].z <= boxB[1].z)  ||
            (boxB[0].z >= boxA[0].z && boxB[0].z <= boxA[1].z)  ||
            (boxB[1].z >= boxA[0].z && boxB[1].z <= boxA[1].z)) 
        )
        {
            handleCollision( temp );
            
            if ( next != NULL )
                ((Object*)next)->processCollisions(temp);
        }
    }

    if ( temp->next != NULL )
    {    
        processCollisions(((Object*)temp->next));
    }
}

//------------------------------------------------------------------------------
void Object::prepareObjects()
{
        if ( next != NULL )
        {
            ((Object*)next)->prepareObjects();
        }
        processCollisions( getRoot() );
}

//------------------------------------------------------------------------------
Object* Object::getRoot()
{
    Object* temp = this;

    while ( temp->prev != NULL ) {
        temp = (Object*)temp->prev;
    }
     
    return temp;
}


//------------------------------------------------------------------------------
void Object::showCollisionBox( void )
{
	glDisable( GL_CULL_FACE );
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
	glPushMatrix();
		
        glColor3f(1.0f, 0.0f, 0.0f);
			
        glBegin( GL_QUADS );
            
        glVertex3f( collisionBox[0].x, collisionBox[1].y, collisionBox[1].z );
        glVertex3f( collisionBox[0].x, collisionBox[0].y, collisionBox[1].z );
        glVertex3f( collisionBox[1].x, collisionBox[0].y, collisionBox[1].z );
        glVertex3f( collisionBox[1].x, collisionBox[1].y, collisionBox[1].z );
        
        glVertex3f( collisionBox[0].x, collisionBox[1].y, collisionBox[1].z );
        glVertex3f( collisionBox[0].x, collisionBox[1].y, collisionBox[0].z );
        glVertex3f( collisionBox[0].x, collisionBox[0].y, collisionBox[0].z );
        glVertex3f( collisionBox[0].x, collisionBox[0].y, collisionBox[1].z );
        
        glVertex3f( collisionBox[1].x, collisionBox[1].y, collisionBox[1].z );
        glVertex3f( collisionBox[1].x, collisionBox[0].y, collisionBox[1].z );
        glVertex3f( collisionBox[1].x, collisionBox[0].y, collisionBox[0].z );
        glVertex3f( collisionBox[1].x, collisionBox[1].y, collisionBox[0].z );
                
        glVertex3f( collisionBox[1].x, collisionBox[1].y, collisionBox[0].z );
        glVertex3f( collisionBox[1].x, collisionBox[0].y, collisionBox[0].z );
        glVertex3f( collisionBox[0].x, collisionBox[0].y, collisionBox[0].z );
        glVertex3f( collisionBox[0].x, collisionBox[1].y, collisionBox[0].z );
        
        glVertex3f( collisionBox[0].x, collisionBox[1].y, collisionBox[0].z );
        glVertex3f( collisionBox[0].x, collisionBox[1].y, collisionBox[1].z );
        glVertex3f( collisionBox[1].x, collisionBox[1].y, collisionBox[1].z );
        glVertex3f( collisionBox[1].x, collisionBox[1].y, collisionBox[0].z );

        glVertex3f( collisionBox[0].x, collisionBox[0].y, collisionBox[0].z );
        glVertex3f( collisionBox[1].x, collisionBox[0].y, collisionBox[0].z );
        glVertex3f( collisionBox[1].x, collisionBox[0].y, collisionBox[1].z );
        glVertex3f( collisionBox[0].x, collisionBox[0].y, collisionBox[1].z );
           
        glEnd();
     
    glPopMatrix();	
	glEnable( GL_CULL_FACE );
	glPolygonMode(GL_FRONT, GL_FILL);	
 
    if ( next != NULL )
        ((Object*)next)->showCollisionBox();
}

//------------------------------------------------------------------------------
void Object::showControlTriangle()
{
	glDisable( GL_CULL_FACE );
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
	glPushMatrix();
		
        glColor3f(1.0f, 0.0f, 0.0f);
        glBegin( GL_TRIANGLES );
            
        glVertex3f( controlPoints[0].x, controlPoints[0].y, controlPoints[0].z );
        glVertex3f( controlPoints[1].x, controlPoints[1].y, controlPoints[1].z );
        glVertex3f( controlPoints[2].x, controlPoints[2].y, controlPoints[2].z );
        glEnd();
     
    glPopMatrix();	
	glEnable( GL_CULL_FACE );
	glPolygonMode(GL_FRONT, GL_FILL);	
 
    if ( next != NULL )
        ((Object*)next)->showControlTriangle();
}

//------------------------------------------------------------------------------
void Object::animateObjects( float timeElapsed, Object* c )
{   
    animate( timeElapsed, c );
     
    if ( next != NULL )
        ((Object*)next)->animateObjects( timeElapsed, c );
        
}

//------------------------------------------------------------------------------
float Object::calcTriangleCenter( void )
{
    float th1 = ( .5 )*controlPoints[0].y + (.5*controlPoints[1].y );
    float finalHeight = ( .5 )*th1 + ( .5 * controlPoints[2].y );
    
    return( finalHeight );
}

//------------------------------------------------------------------------------
bool Object::testActiveZone( Object* c )
{   
   //#if ( !testActiveZoneEnable )
   //#   return true;

    return ( 
        ( position.z > ( c->position.z - ACTIVE_VOLUME_LENGTH ) ) && 
        ( position.z < ( c->position.z + ACTIVE_VOLUME_LENGTH ) ) && // near-far
         
        ( position.x > ( c->position.x - ACTIVE_VOLUME_LENGTH ) ) && 
        ( position.x < ( c->position.x + ACTIVE_VOLUME_LENGTH ) ) && // left-right
                       
        ( position.y > ( c->position.y - ACTIVE_VOLUME_LENGTH ) ) &&  
        ( position.y < ( c->position.y + ACTIVE_VOLUME_LENGTH ) )    // up-down
    );
}



//------------------------------------------------------------------------------
/*
int Object::processExtendedCommand( const ScriptCommand &t )
{  
    int next = 1;
    
    //cout << "Extended Object Routine: " << t.routine << endl << endl;
    int index1, index2, index3, op;
   
    switch ( t.routine )
    {
        case SCRIPT_SET_SCALE_MEM:
            index1 = (int)t.p1;
            index2 = (int)t.p2;
            index3 = (int)t.p3;
            
            if ( index1 >= 0 && index1 < MEM_LEN &&
                 index2 >= 0 && index2 < MEM_LEN &&
                 index3 >= 0 && index3 < MEM_LEN )
             {
                scaleFactor.setVector( memVars[index1], memVars[index2], memVars[index3] );
             }
            break;
        case SCRIPT_SET_SCALE_RATE_MEM:
            index1 = (int)t.p1;
            index2 = (int)t.p2;
            index3 = (int)t.p3;
            
            if ( index1 >= 0 && index1 < MEM_LEN &&
                 index2 >= 0 && index2 < MEM_LEN &&
                 index3 >= 0 && index3 < MEM_LEN )
             {
                scaleRate.setVector( memVars[index1], memVars[index2], memVars[index3] );
             }
            break;
        case SCRIPT_ADD_SCALE_MEM:
            index1 = (int)t.p1;
            index2 = (int)t.p2;
            index3 = (int)t.p3;
            
            if ( index1 >= 0 && index1 < MEM_LEN &&
                 index2 >= 0 && index2 < MEM_LEN &&
                 index3 >= 0 && index3 < MEM_LEN )
             {
                scaleFactor.x += memVars[index1];
                scaleFactor.y += memVars[index2];
                scaleFactor.z += memVars[index3];
             }
            break;
        case SCRIPT_ADD_SCALE_RATE_MEM:
            index1 = (int)t.p1;
            index2 = (int)t.p2;
            index3 = (int)t.p3;
            
            if ( index1 >= 0 && index1 < MEM_LEN &&
                 index2 >= 0 && index2 < MEM_LEN &&
                 index3 >= 0 && index3 < MEM_LEN )
             {
                scaleRate.x += memVars[index1];
                scaleRate.y += memVars[index2];
                scaleRate.z += memVars[index3];
             }
            break;

        case SCRIPT_SET_SCALE_VAL:
            scaleFactor.setVector(t.p1, t.p2, t.p3);
            break;
        case SCRIPT_SET_SCALE_RATE_VAL:
            scaleRate.setVector(t.p1, t.p2, t.p3);
            break;
        case SCRIPT_ADD_SCALE_VAL:
            scaleFactor.x += t.p1;
            scaleFactor.y += t.p2;
            scaleFactor.z += t.p3;
            break;
        case SCRIPT_ADD_SCALE_RATE_VAL:
            scaleRate.x += t.p1;
            scaleRate.y += t.p2;
            scaleRate.z += t.p3;
            break;

        case SCRIPT_SET_TEST_ZONE_ENABLE_VAL:
            op = (int)t.p1;
            
            if (op == 0)
                testActiveZoneEnable = false;
            else
                testActiveZoneEnable = true;
            
            break;
        case SCRIPT_SET_STATE_VAL:
            state = (int)t.p1;
            break;
        case SCRIPT_STORE_STATE_IN_MEM:
            index1 = (int)t.p1;
            
            if ( index1 >= 0 && index1 < MEM_LEN ) {
                memVars[index1] = (float)state;
            }
            break;
        
        case SCRIPT_STORE_SCALE_IN_MEM:
            index1 = (int)t.p1;
            index2 = (int)t.p2;
            index3 = (int)t.p3;
            
            if ( index1 >= 0 && index1 < MEM_LEN &&
                 index2 >= 0 && index2 < MEM_LEN &&
                 index3 >= 0 && index3 < MEM_LEN )
             {
                    memVars[index1] = scaleFactor.x;
                    memVars[index2] = scaleFactor.y;
                    memVars[index3] = scaleFactor.z;
            }
            break;
        case SCRIPT_STORE_SCALE_RATE_IN_MEM:
            index1 = (int)t.p1;
            index2 = (int)t.p2;
            index3 = (int)t.p3;
            
            if ( index1 >= 0 && index1 < MEM_LEN &&
                 index2 >= 0 && index2 < MEM_LEN &&
                 index3 >= 0 && index3 < MEM_LEN )
             {
                    memVars[index1] = scaleRate.x;
                    memVars[index2] = scaleRate.y;
                    memVars[index3] = scaleRate.z;
            }
            break;
    }
    
    
    return( next );
}

*/

void Object::setLuaState(lua_State* tl)
{
}

//------------------------------------------------------------------------------
void Object::setPosition( GLfloat x, GLfloat y, GLfloat z )
{ position.setVector( x, y, z ); }

//------------------------------------------------------------------------------
void Object::setPosition( Vector t )
{ position = t; }
 
//------------------------------------------------------------------------------ 
void Object::setRotationAxis( GLfloat vx, GLfloat vy, GLfloat vz, GLfloat vw )
{
    rotation.buildFromAxis( Vector( vx, vy, vz ), vw );
}

//------------------------------------------------------------------------------
void Object::setRotationAxis( const Vector &v, GLfloat a )
{
    rotation.buildFromAxis( v, a );
}

//------------------------------------------------------------------------------
void Object::setRotationEuler( GLfloat x, GLfloat y, GLfloat z )
{
    rotation.buildFromEuler( x, y, z );
}

//------------------------------------------------------------------------------
void Object::setRotationEuler( const Vector &v )
{
    rotation.buildFromEuler( v );
}

//------------------------------------------------------------------------------            
void Object::setRotationQuaternion( const Quaternion &q )
{
    rotation = q;
}

//------------------------------------------------------------------------------
void Object::setRotationVelocity( GLfloat xAngle, GLfloat yAngle, GLfloat zAngle )
{
    rotationVelocity.setVector( xAngle, yAngle, zAngle );
}

//------------------------------------------------------------------------------            
void Object::setRotationVelocity( const Vector &v )
{
    rotationVelocity = v; 
}

//------------------------------------------------------------------------------
void Object::setVelocity( GLfloat x, GLfloat y, GLfloat z )
{ velocity.setVector( x, y, z ); }

//------------------------------------------------------------------------------
void Object::setVelocity( const Vector &v )
{
    velocity = v;
}      

//------------------------------------------------------------------------------
void Object::setTimer( float* temp )
{
    //#animCurrTime = temp;   
}


//------------------------------------------------------------------------------
//int Object::processBasicCommand( const ScriptCommand &t )
//{
//   int next = 1;
   /*
   //cout << "routine: " << t.routine << endl << endl;
   int index1, index2, index3, index4, op;
   
    switch (t.routine)
    {
        // load another script to execute //
        case SCRIPT_SET_SCRIPT:
            setScript((int)t.p1);
            next = 0;
            break;

        // extended basic //        
      
            break;
        
    }  
   
   */
//    return (next);
//}

//------------------------------------------------------------------------------
void Object::animateScript(float elapsedTime)
{
    // Attempt to execute the script only if the lua state has already been
    // initialized with a script
    if (L == NULL)
        return;
        
    // If the script is suspended, do not run until time has elapsed
    if (suspend) {
        suspendTime -= elapsedTime; 
        if (suspendTime > 0.0f) 
            return;
        suspend = false;
        suspendTime = 0.0f;
    }
    
    // Find the update function and call it
    lua_getglobal(L, "on_update");
	    
    // Push a pointer to the current object for use within the lua function
    lua_pushlightuserdata(L, (void*)this);
	   
	// Push the time passed since the last iteration of the game loop
    lua_pushnumber(L, elapsedTime);
    
    // Call the function with 2 argument and no return values
    lua_call(L, 2, 0);

    // get the result //
    //position.z = (float)lua_tonumber(L, -1);
    //position.y = (float)lua_tonumber(L, -2);
    //position.x = (float)lua_tonumber(L, -3);
    //lua_pop(L, 1);
}

//------------------------------------------------------------------------------
void Object::setPlayerPtr( Object* tPlayer )
{
    player = tPlayer;
}

//------------------------------------------------------------------------------
void Object::setCameraPtr( Object* tCamera)
{
    camera = tCamera;
}

//------------------------------------------------------------------------------
void Object::setInterpolationVariable(int index)
{
    switch (index)
    {
        case 0:
            //#valInterpPtr = animCurrTime;
            break;
        case 1:
            valInterpPtr = &position.x;
            break;
        case 2:
            valInterpPtr = &position.y;
            break;
        case 3:
            valInterpPtr = &position.z;
            break;
        //#default:
            //#valInterpPtr = animCurrTime;
    }
}

void Object::init(void)
{
    rotation.loadMultIdentity();
    baseDirection.setVector( 0.0f, 0.0f, 1.0f );
    direction.setVector( 0.0f, 0.0f, 1.0f );
    up.setVector(0.0f, 1.0f, 0.0f);
          
    speed = 0.0f;
    speedDir = 0;
    
    velocity.setVector(0.0f, 0.0f, 0.0f);
            
    rInterpStart.loadMultIdentity();
    rInterpEnd.loadMultIdentity();
    
    tieMemVarIndex = 0;
    valInterpStart = 0.0f;
    valInterpEnd = 0.0f;
    interp = false;
        
    //scriptLastTime = 0.0f;
    //scriptNum = NO_SCRIPT;
    //scriptName = NO_SCRIPT;
    //currentScriptCommand = 0;
    
    //#animCurrTime = NULL;  
    player = NULL;
    camera = NULL;
    
    L = NULL;
    
    suspend = false;
    suspendTime = 0.0f;
   
    //memVars[MEM_NEXT_SCRIPT_INDEX] = NO_SCRIPT;
    //memVars[MEM_NEXT_SCRIPT_COMMAND_INDEX] = 0;
    
    if (particleSystem != NULL)
        delete particleSystem;
    
    particleSystem = NULL;
}

void Object::loadScript(string name)
{
    scriptName = name;
    
    // If the lua state has not been initialized for this object, attempt to
    // initialize it.     
    if (scriptName == "" || L != NULL)
        return;
        
    L = lua_open();
    
    // If the lua state still could not be initialized, then exit the game.
    // ... we can do something smarter with this in the finished product.
    if (L == NULL) {
        printf("Error creating Lua state.\n");
	    exit(-1);
    }

    // Load the base lua libraries
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
    
    lua_register(L, "vector_getScalar", vector_getScalarLua); 
    lua_register(L, "engine_testKeyPressed", engine_testKeyPressedLua); 
    lua_register(L, "engine_testKeyReleased", engine_testKeyReleasedLua); 
    lua_register(L, "vector_normalize", vector_normalizeLua); 
    lua_register(L, "vector_dotProduct", vector_dotProductLua); 
    lua_register(L, "vector_crossProduct", vector_crossProductLua); 
    lua_register(L, "getTypeId", getTypeIdLua); 
    lua_register(L, "setTypeId", setTypeIdLua); 
    lua_register(L, "setScaleRate", setScaleRateLua);     
    lua_register(L, "getScriptValue", getScriptValueLua);
        
    // Load this object's animation script
    luaL_dofile(L, scriptName.c_str());
        
    // Set player and camera pointers for use within exposed c functions.
    lplayer = player;
    lcamera = camera;  
        
    // Find the update function and call it
    lua_getglobal(L, "on_load");
	    
    // Push a pointer to the current object for use within the lua function
    lua_pushlightuserdata(L, (void*)this);
	   
    // Call the function with 1 argument and no return values
    lua_call(L, 1, 0);

    // get the result //
    //position.z = (float)lua_tonumber(L, -1);
    //position.y = (float)lua_tonumber(L, -2);
    //position.x = (float)lua_tonumber(L, -3);
    //lua_pop(L, 1);
}

void Object::unloadScript()
{
    // Attempt to execute the on_unload function only if the lua state has 
    // already been initialized with a script
    if (L == NULL)
        return;
    
    // Find the update function and call it
    lua_getglobal(L, "on_unload");
	    
    // Push a pointer to the current object for use within the lua function
    lua_pushlightuserdata(L, (void*)this);
	   
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

void Object::setParticleSystem(int particleSystemNumber)
{
     
            // setup the weather effect ////////////////
            if (particleSystemNumber > 0 && particleSystem == NULL) {
                particleSystem = new Snow(this);
                particleSystem->update(4.0f);
            }
        
}


