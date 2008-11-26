#ifndef _SCRIPTED_OBJECT_H_
#define _SCRIPTED_OBJECT_H_

#include "constants.h"

extern "C" {
    #include "lua/lua.h"
    #include "lua/lualib.h"
    #include "lua/lauxlib.h"
}

class ScriptedObject
{
    public:
        // object orientation //
        Vector position;            // x,y,z position of object
        Quaternion rotation;
        Vector baseDirection;       // base direction of object
        Vector direction;           // direction facing
        Vector up;
          
        // rate of change //
        Vector velocity;
        Vector rotationVelocity;
        float speed;                // speed of object, used in scripting  
        int speedDir; 
            
        // used for interpolation between 2 orientations //
        Quaternion rInterpStart;
        Quaternion rInterpEnd;
        
        int tieMemVarIndex;
        float valInterpStart;
        float valInterpEnd;
        float* valInterpPtr;
        bool interp;
        
        // Possibly needed?
        // pointer to current game clock //
        float *animCurrTime;    // pointer to engine main clock
           
        // pointer to camera and player //
        ScriptedObject* camera;
        ScriptedObject* player;
        
        
        // Necessary for the Lua implementation
        lua_State* L;
        string scriptName;
        
        bool suspend;
        float suspendTime;
        
   // public:
        ScriptedObject( void );
        virtual ~ScriptedObject( void );
        
        void setPosition( GLfloat, GLfloat, GLfloat );
        void setPosition( Vector );
        void setRotationAxis( GLfloat vx, GLfloat vy, GLfloat vz, GLfloat vw );
        void setRotationAxis( const Vector &v, GLfloat a );
        void setRotationEuler( GLfloat, GLfloat, GLfloat );
        void setRotationEuler( const Vector &v );
        void setRotationQuaternion( const Quaternion &q );
        void setVelocity( GLfloat, GLfloat, GLfloat );
        void setVelocity( const Vector &v );
        void setRotationVelocity( GLfloat xAngle, GLfloat yAngle, GLfloat zAngle );
        void setRotationVelocity( const Vector &v );
        void setTimer( float* );
        void setPlayerPtr( ScriptedObject* );
        void setCameraPtr( ScriptedObject* );
        void setLuaState(lua_State*);
        
        void loadScript(string file);
        void unloadScript();
        
        void setInterpolationVariable(int index);
    
        void animateScript( float elapsedTime );
        
        // virtual functions
        virtual void printTypeName(void) = 0;
};

static ScriptedObject* lplayer;
static ScriptedObject* lcamera;

static int setPositionLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    ScriptedObject *scriptedObject = static_cast<ScriptedObject*>(lua_touserdata(L, 1));
    
	scriptedObject->setPosition(lua_tonumber(L,2),lua_tonumber(L,3),lua_tonumber(L,4));
	return 0;
}

static int getPositionLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    ScriptedObject *scriptedObject = static_cast<ScriptedObject*>(lua_touserdata(L, 1));
		
	lua_pushnumber(L, scriptedObject->position.x);
	lua_pushnumber(L, scriptedObject->position.y);
	lua_pushnumber(L, scriptedObject->position.z);

    // the number of values returned
	return 3;
}

static int setVelocityLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    ScriptedObject *scriptedObject = static_cast<ScriptedObject*>(lua_touserdata(L, 1));
	
	scriptedObject->setVelocity(lua_tonumber(L, 2),lua_tonumber(L, 3),lua_tonumber(L, 4));
	return 0;
}

static int getVelocityLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    ScriptedObject *scriptedObject = static_cast<ScriptedObject*>(lua_touserdata(L, 1));
		
	lua_pushnumber(L, scriptedObject->velocity.x);
	lua_pushnumber(L, scriptedObject->velocity.y);
	lua_pushnumber(L, scriptedObject->velocity.z);

	// the number of values returned
	return 3;
}

static int setRotationVelocityLua(lua_State *L) 
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    ScriptedObject *scriptedObject = static_cast<ScriptedObject*>(lua_touserdata(L, 1));
	scriptedObject->setRotationVelocity(lua_tonumber(L,2),lua_tonumber(L,3),lua_tonumber(L,4));
	return 0;
}

static int getRotationVelocityLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    ScriptedObject *scriptedObject = static_cast<ScriptedObject*>(lua_touserdata(L, 1));
		
	lua_pushnumber(L, scriptedObject->rotationVelocity.x);
	lua_pushnumber(L, scriptedObject->rotationVelocity.y);
	lua_pushnumber(L, scriptedObject->rotationVelocity.z);

	// the number of values returned
	return 3;
}

static int setSpeedLua(lua_State *L) 
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    ScriptedObject *scriptedObject = static_cast<ScriptedObject*>(lua_touserdata(L, 1));
	
	scriptedObject->speedDir = (int)lua_tonumber(L, 2);
	scriptedObject->speed    = lua_tonumber(L, 3);
	return 0;
}

static int getSpeedLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    ScriptedObject *scriptedObject = static_cast<ScriptedObject*>(lua_touserdata(L, 1));
		
	lua_pushnumber(L, scriptedObject->speedDir);
	lua_pushnumber(L, scriptedObject->speed);

	// the number of values returned
	return 2;
}

static int getPlayerLua(lua_State *L)
{
	lua_pushlightuserdata(L, (void*)lplayer);
	return 1;
}

static int getCameraLua(lua_State *L)
{		
	lua_pushlightuserdata(L, (void*)lcamera);
	return 1;
}

static int setRotationLua(lua_State *L) 
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    ScriptedObject *scriptedObject = static_cast<ScriptedObject*>(lua_touserdata(L, 1));
	scriptedObject->setRotationEuler(lua_tonumber(L,2),lua_tonumber(L,3),lua_tonumber(L,4));
	return 0;
}

static int getRotationLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    ScriptedObject *scriptedObject = static_cast<ScriptedObject*>(lua_touserdata(L, 1));
    
    Vector tv;
    scriptedObject->rotation.getEulerAngles(tv);
                
	lua_pushnumber(L, tv.x);
	lua_pushnumber(L, tv.y);
	lua_pushnumber(L, tv.z);

	// the number of values returned
	return 3;
}

static int getDirectionLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    ScriptedObject *scriptedObject = static_cast<ScriptedObject*>(lua_touserdata(L, 1));
		
	lua_pushnumber(L, scriptedObject->direction.x);
	lua_pushnumber(L, scriptedObject->direction.y);
	lua_pushnumber(L, scriptedObject->direction.z);

	// the number of values returned
	return 3;
}

static int getUpLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    ScriptedObject *scriptedObject = static_cast<ScriptedObject*>(lua_touserdata(L, 1));
		
	lua_pushnumber(L, scriptedObject->up.x);
	lua_pushnumber(L, scriptedObject->up.y);
	lua_pushnumber(L, scriptedObject->up.z);

	// the number of values returned
	return 3;
}

static int getOrthogonalLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    ScriptedObject *scriptedObject = static_cast<ScriptedObject*>(lua_touserdata(L, 1));
	
    Vector rotationAxis;
            
    rotationAxis.crossProduct(scriptedObject->up, scriptedObject->direction);
    rotationAxis.normalize();
   	
	lua_pushnumber(L, rotationAxis.x);
	lua_pushnumber(L, rotationAxis.y);
	lua_pushnumber(L, rotationAxis.z);

	// the number of values returned
	return 3;
}

static int addPositionLua(lua_State *L) 
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    ScriptedObject *scriptedObject = static_cast<ScriptedObject*>(lua_touserdata(L, 1));
    
	scriptedObject->position += Vector(lua_tonumber(L,2),lua_tonumber(L,3),lua_tonumber(L,4));
	return 0;
}

static int addRotationLua(lua_State *L) 
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    ScriptedObject *scriptedObject = static_cast<ScriptedObject*>(lua_touserdata(L, 1));
    
    Quaternion qt;
    qt.buildFromEuler(lua_tonumber(L,2),lua_tonumber(L,3),lua_tonumber(L,4));
    scriptedObject->rotation = scriptedObject->rotation * qt;
    
	return 0;
}

static int addVelocityLua(lua_State *L) 
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    ScriptedObject *scriptedObject = static_cast<ScriptedObject*>(lua_touserdata(L, 1));
	scriptedObject->velocity += Vector(lua_tonumber(L,2),lua_tonumber(L,3),lua_tonumber(L,4));
	return 0;
}

static int addSpeedLua(lua_State *L) 
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    ScriptedObject *scriptedObject = static_cast<ScriptedObject*>(lua_touserdata(L, 1));
	scriptedObject->speed += lua_tonumber(L,2);
	return 0;
}

static int addRotationVelocityLua(lua_State *L) 
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    ScriptedObject *scriptedObject = static_cast<ScriptedObject*>(lua_touserdata(L, 1));
	scriptedObject->rotationVelocity += Vector(lua_tonumber(L,2),lua_tonumber(L,3),lua_tonumber(L,4));
	return 0;
}

static int setInterpolationRotationEndAxisLua(lua_State *L) 
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    ScriptedObject *scriptedObject = static_cast<ScriptedObject*>(lua_touserdata(L, 1));
    
    Vector tv;
    tv.setVector(lua_tonumber(L,2),lua_tonumber(L,3),lua_tonumber(L,4));
    tv.normalize();
            
    scriptedObject->rInterpEnd.buildFromAxis(tv, lua_tonumber(L,5));
	return 0;
}

static int setInterpolationRotationStartAxisLua(lua_State *L) 
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    ScriptedObject *scriptedObject = static_cast<ScriptedObject*>(lua_touserdata(L, 1));
    
    Vector tv;
    tv.setVector(lua_tonumber(L,2),lua_tonumber(L,3),lua_tonumber(L,4));
    tv.normalize();
            
    scriptedObject->rInterpStart.buildFromAxis(tv, lua_tonumber(L,5));
	return 0;
}

static int setInterpolationRotationStartLua(lua_State *L) 
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    ScriptedObject *scriptedObject = static_cast<ScriptedObject*>(lua_touserdata(L, 1));
    
    scriptedObject->rInterpStart.buildFromEuler(lua_tonumber(L,2),lua_tonumber(L,3),lua_tonumber(L,4));
	return 0;
}

static int setInterpolationRotationEndLua(lua_State *L) 
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    ScriptedObject *scriptedObject = static_cast<ScriptedObject*>(lua_touserdata(L, 1));
    
    scriptedObject->rInterpEnd.buildFromEuler(lua_tonumber(L,2),lua_tonumber(L,3),lua_tonumber(L,4));
	return 0;
}

static int setInterpolationEnableLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    ScriptedObject *scriptedObject = static_cast<ScriptedObject*>(lua_touserdata(L, 1));
    
    int op = (int)lua_tonumber(L,2);
            
    if (op == 0)
        scriptedObject->interp = false;
    else
        scriptedObject->interp = true;
             
    return (0);
}

static int setInterpolationVariableLua(lua_State *L) 
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    ScriptedObject *scriptedObject = static_cast<ScriptedObject*>(lua_touserdata(L, 1));
    
    int index1 = (int)lua_tonumber(L,2);
    
    scriptedObject->setInterpolationVariable(index1);
            
    scriptedObject->valInterpStart = lua_tonumber(L,3);
    scriptedObject->valInterpEnd = lua_tonumber(L,4);
    
	return 0;
}

static int setRotationVelocityAxisLua(lua_State *L) 
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    ScriptedObject *scriptedObject = static_cast<ScriptedObject*>(lua_touserdata(L, 1));
    
    scriptedObject->rotationVelocity.setVector(lua_tonumber(L,2),lua_tonumber(L,3),lua_tonumber(L,4));
    scriptedObject->rotationVelocity.normalize();
    scriptedObject->rotationVelocity.scale(lua_tonumber(L,5)); 

	return 0;
}

static int setRotationAxisLua(lua_State *L) 
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    ScriptedObject *scriptedObject = static_cast<ScriptedObject*>(lua_touserdata(L, 1));
    
    Vector tv;
    tv.setVector(lua_tonumber(L,2),lua_tonumber(L,3),lua_tonumber(L,4));
    tv.normalize();
            
    scriptedObject->rotation.buildFromAxis(tv, lua_tonumber(L,5));
    
	return 0;
}

static int addRotationAxisLua(lua_State *L) 
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    ScriptedObject *scriptedObject = static_cast<ScriptedObject*>(lua_touserdata(L, 1));
    
    Quaternion tq;
    Vector tv;
    tv.setVector(lua_tonumber(L,2),lua_tonumber(L,3),lua_tonumber(L,4));
    tv.normalize();
    tq.buildFromAxis(tv, lua_tonumber(L,5));
                
    scriptedObject->rotation = scriptedObject->rotation * tq;
    
	return 0;
}

static int getTimerLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    ScriptedObject *scriptedObject = static_cast<ScriptedObject*>(lua_touserdata(L, 1));
    
    lua_pushnumber(L, *scriptedObject->animCurrTime);
    return 1;
}

static int suspendLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    ScriptedObject *scriptedObject = static_cast<ScriptedObject*>(lua_touserdata(L, 1));
    
    scriptedObject->suspend = true;
    scriptedObject->suspendTime = lua_tonumber(L, 2);
    
    return 0;
}

       
#endif

