#include "ScriptedObject.h"

//------------------------------------------------------------------------------
ScriptedObject::ScriptedObject( void )
{
    rotation.loadMultIdentity();
    baseDirection.setVector( 0.0f, 0.0f, 1.0f );
    direction.setVector( 0.0f, 0.0f, 1.0f );
    up.setVector(0.0f, 1.0f, 0.0f);
          
    speed = 0.0f;
    speedDir = 0;
            
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
    
    animCurrTime = NULL;  
    player = NULL;
    camera = NULL;
    
    L = NULL;
    
    suspend = false;
    suspendTime = 0.0f;
   
    //memVars[MEM_NEXT_SCRIPT_INDEX] = NO_SCRIPT;
    //memVars[MEM_NEXT_SCRIPT_COMMAND_INDEX] = 0;
}

//------------------------------------------------------------------------------
ScriptedObject::~ScriptedObject( void )
{
    if (L != NULL)
        lua_close(L);
}

void ScriptedObject::setLuaState(lua_State* tl)
{
}

//------------------------------------------------------------------------------
void ScriptedObject::setPosition( GLfloat x, GLfloat y, GLfloat z )
{ position.setVector( x, y, z ); }

//------------------------------------------------------------------------------
void ScriptedObject::setPosition( Vector t )
{ position = t; }
 
//------------------------------------------------------------------------------ 
void ScriptedObject::setRotationAxis( GLfloat vx, GLfloat vy, GLfloat vz, GLfloat vw )
{
    rotation.buildFromAxis( Vector( vx, vy, vz ), vw );
}

//------------------------------------------------------------------------------
void ScriptedObject::setRotationAxis( const Vector &v, GLfloat a )
{
    rotation.buildFromAxis( v, a );
}

//------------------------------------------------------------------------------
void ScriptedObject::setRotationEuler( GLfloat x, GLfloat y, GLfloat z )
{
    rotation.buildFromEuler( x, y, z );
}

//------------------------------------------------------------------------------
void ScriptedObject::setRotationEuler( const Vector &v )
{
    rotation.buildFromEuler( v );
}

//------------------------------------------------------------------------------            
void ScriptedObject::setRotationQuaternion( const Quaternion &q )
{
    rotation = q;
}

//------------------------------------------------------------------------------
void ScriptedObject::setRotationVelocity( GLfloat xAngle, GLfloat yAngle, GLfloat zAngle )
{
    rotationVelocity.setVector( xAngle, yAngle, zAngle );
}

//------------------------------------------------------------------------------            
void ScriptedObject::setRotationVelocity( const Vector &v )
{
    rotationVelocity = v; 
}

//------------------------------------------------------------------------------
void ScriptedObject::setVelocity( GLfloat x, GLfloat y, GLfloat z )
{ velocity.setVector( x, y, z ); }

//------------------------------------------------------------------------------
void ScriptedObject::setVelocity( const Vector &v )
{
    velocity = v;
}      

//------------------------------------------------------------------------------
void ScriptedObject::setTimer( float* temp )
{
    animCurrTime = temp;   
}

//------------------------------------------------------------------------------
int ScriptedObject::processBasicCommand( const ScriptCommand &t )
{
   int next = 1;
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
    return (next);
}

//------------------------------------------------------------------------------
void ScriptedObject::animateScript(float elapsedTime)
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
void ScriptedObject::setPlayerPtr( ScriptedObject* tPlayer )
{
    player = tPlayer;
}

//------------------------------------------------------------------------------
void ScriptedObject::setCameraPtr( ScriptedObject* tCamera)
{
    camera = tCamera;
}

//------------------------------------------------------------------------------
void ScriptedObject::setInterpolationVariable(int index)
{
    switch (index)
    {
        case 0:
            valInterpPtr = animCurrTime;
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
        default:
            valInterpPtr = animCurrTime;
    }
}

void ScriptedObject::loadScript(string name)
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
    // ScriptedObject.h)
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

void ScriptedObject::unloadScript()
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

