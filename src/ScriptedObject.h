#ifndef _SCRIPTED_OBJECT_H_
#define _SCRIPTED_OBJECT_H_

#include "constants.h"

extern "C" {
    #include "lua/lua.h"
    #include "lua/lualib.h"
    #include "lua/lauxlib.h"
    
   // static int newarray(lua_State *L);
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
        
        // script information //
        float scriptLastTime;       // last time of script command
        int scriptNum;              // index into AIScript array
        int currentScriptCommand;   // index into script[scriptNum][command]
        string scriptName;
        
        // pointer to current game clock //
        float *animCurrTime;    // pointer to engine main clock
        
        // scripting temporary storage //
        //float memVars[512];
        
        // pointer to camera and player //
        ScriptedObject* camera;
        ScriptedObject* player;
        
        lua_State* L;
        
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
        void setScript( int );
        void setScriptName( string );
        void setPlayerPtr( ScriptedObject* );
        void setCameraPtr( ScriptedObject* );
        void setLuaState(lua_State*);
        
        void setInterpolationVariable(int index);
    
        int processBasicCommand( const ScriptCommand & );
        void animateScript( void );
        
        // virtual functions
        virtual int processExtendedCommand( const ScriptCommand & ) = 0;
        virtual void printTypeName(void) = 0;
};

static ScriptedObject* lplayer;
static ScriptedObject* lcamera;

static int setPositionLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    ScriptedObject *scriptedObject = static_cast<ScriptedObject*>(lua_touserdata(L, 1));
    
	/* total the arguments */
	scriptedObject->setPosition(lua_tonumber(L,1),lua_tonumber(L,2),lua_tonumber(L,3));
	return 0;
}

static int getPositionLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    ScriptedObject *scriptedObject = static_cast<ScriptedObject*>(lua_touserdata(L, 1));
		
	/* push the sum */
	lua_pushnumber(L, scriptedObject->position.x);
	lua_pushnumber(L, scriptedObject->position.y);
	lua_pushnumber(L, scriptedObject->position.z);

	/* return the number of results */
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
		
	/* push the sum */
	lua_pushnumber(L, scriptedObject->velocity.x);
	lua_pushnumber(L, scriptedObject->velocity.y);
	lua_pushnumber(L, scriptedObject->velocity.z);

	/* return the number of results */
	return 3;
}

static int setRotationVelocityLua(lua_State *L) 
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    ScriptedObject *scriptedObject = static_cast<ScriptedObject*>(lua_touserdata(L, 1));
	/* total the arguments */
	scriptedObject->setRotationVelocity(lua_tonumber(L,1),lua_tonumber(L,2),lua_tonumber(L,3));
	return 0;
}

static int getPlayer(lua_State *L)
{
	lua_pushlightuserdata(L, (void*)lplayer);
	return 1;
}

static int getCamera(lua_State *L)
{		
	lua_pushlightuserdata(L, (void*)lcamera);
	return 1;
}



#endif

