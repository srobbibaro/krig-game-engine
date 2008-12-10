#ifndef _SCRIPTING_H_
#define _SCRIPTING_H_

extern "C" {
    #include "lua/lua.h"
    #include "lua/lualib.h"
    #include "lua/lauxlib.h"
}

#include "Object.h"
#include "ScriptedObject.h"
#include "GameLevel.h"
#include "Matrix.h"
#include "Engine.h"

class Scripting
{
    public:
        Scripting() {}
        ~Scripting( void ) {}
        
    private:
        lua_State* lua_;

     
};

static Object* lplayer;
static Object* lcamera;
static GameLevel* lgameLevel;

static int setPositionLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));
    
	object->setPosition(lua_tonumber(L,2),lua_tonumber(L,3),lua_tonumber(L,4));
	return 0;
}

static int getPositionLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));
		
	lua_pushnumber(L, object->position.x);
	lua_pushnumber(L, object->position.y);
	lua_pushnumber(L, object->position.z);

    // the number of values returned
	return 3;
}

static int setVelocityLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));
	
	object->setVelocity(lua_tonumber(L, 2),lua_tonumber(L, 3),lua_tonumber(L, 4));
	return 0;
}

static int getVelocityLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));
		
	lua_pushnumber(L, object->velocity.x);
	lua_pushnumber(L, object->velocity.y);
	lua_pushnumber(L, object->velocity.z);

	// the number of values returned
	return 3;
}

static int setRotationVelocityLua(lua_State *L) 
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));
	object->setRotationVelocity(lua_tonumber(L,2),lua_tonumber(L,3),lua_tonumber(L,4));
	return 0;
}

static int getRotationVelocityLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));
		
	lua_pushnumber(L, object->rotationVelocity.x);
	lua_pushnumber(L, object->rotationVelocity.y);
	lua_pushnumber(L, object->rotationVelocity.z);

	// the number of values returned
	return 3;
}

static int setSpeedLua(lua_State *L) 
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));
	
	object->speedDir = (int)lua_tonumber(L, 2);
	object->speed    = lua_tonumber(L, 3);
	return 0;
}

static int getSpeedLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));
		
	lua_pushnumber(L, object->speedDir);
	lua_pushnumber(L, object->speed);

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
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));
	object->setRotationEuler(lua_tonumber(L,2),lua_tonumber(L,3),lua_tonumber(L,4));
	return 0;
}

static int getRotationLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));
    
    Vector tv;
    object->rotation.getEulerAngles(tv);
                
	lua_pushnumber(L, tv.x);
	lua_pushnumber(L, tv.y);
	lua_pushnumber(L, tv.z);

	// the number of values returned
	return 3;
}

static int getDirectionLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));
		
	lua_pushnumber(L, object->direction.x);
	lua_pushnumber(L, object->direction.y);
	lua_pushnumber(L, object->direction.z);

	// the number of values returned
	return 3;
}

static int getUpLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));
		
	lua_pushnumber(L, object->up.x);
	lua_pushnumber(L, object->up.y);
	lua_pushnumber(L, object->up.z);

	// the number of values returned
	return 3;
}

static int getOrthogonalLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));
	
    Vector rotationAxis;
            
    rotationAxis.crossProduct(object->up, object->direction);
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
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));
    
	object->position += Vector(lua_tonumber(L,2),lua_tonumber(L,3),lua_tonumber(L,4));
	return 0;
}

static int addRotationLua(lua_State *L) 
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));
    
    Quaternion qt;
    qt.buildFromEuler(lua_tonumber(L,2),lua_tonumber(L,3),lua_tonumber(L,4));
    object->rotation = object->rotation * qt;
    
	return 0;
}

static int addVelocityLua(lua_State *L) 
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));
	object->velocity += Vector(lua_tonumber(L,2),lua_tonumber(L,3),lua_tonumber(L,4));
	return 0;
}

static int addSpeedLua(lua_State *L) 
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));
	object->speed += lua_tonumber(L,2);
	return 0;
}

static int addRotationVelocityLua(lua_State *L) 
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));
	object->rotationVelocity += Vector(lua_tonumber(L,2),lua_tonumber(L,3),lua_tonumber(L,4));
	return 0;
}

static int setInterpolationRotationEndAxisLua(lua_State *L) 
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));
    
    Vector tv;
    tv.setVector(lua_tonumber(L,2),lua_tonumber(L,3),lua_tonumber(L,4));
    tv.normalize();
            
    object->rInterpEnd.buildFromAxis(tv, lua_tonumber(L,5));
	return 0;
}

static int setInterpolationRotationStartAxisLua(lua_State *L) 
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));
    
    Vector tv;
    tv.setVector(lua_tonumber(L,2),lua_tonumber(L,3),lua_tonumber(L,4));
    tv.normalize();
            
    object->rInterpStart.buildFromAxis(tv, lua_tonumber(L,5));
	return 0;
}

static int setInterpolationRotationStartLua(lua_State *L) 
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));
    
    object->rInterpStart.buildFromEuler(lua_tonumber(L,2),lua_tonumber(L,3),lua_tonumber(L,4));
	return 0;
}

static int setInterpolationRotationEndLua(lua_State *L) 
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));
    
    object->rInterpEnd.buildFromEuler(lua_tonumber(L,2),lua_tonumber(L,3),lua_tonumber(L,4));
	return 0;
}

static int setInterpolationEnableLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));
    
    int op = (int)lua_tonumber(L,2);
            
    if (op == 0)
        object->interp = false;
    else
        object->interp = true;
             
    return (0);
}

static int setInterpolationVariableLua(lua_State *L) 
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));
    
    int index1 = (int)lua_tonumber(L,2);
    
    object->setInterpolationVariable(index1);
            
    object->valInterpStart = lua_tonumber(L,3);
    object->valInterpEnd = lua_tonumber(L,4);
    
	return 0;
}

static int setRotationVelocityAxisLua(lua_State *L) 
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));
    
    object->rotationVelocity.setVector(lua_tonumber(L,2),lua_tonumber(L,3),lua_tonumber(L,4));
    object->rotationVelocity.normalize();
    object->rotationVelocity.scale(lua_tonumber(L,5)); 

	return 0;
}

static int setRotationAxisLua(lua_State *L) 
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));
    
    Vector tv;
    tv.setVector(lua_tonumber(L,2),lua_tonumber(L,3),lua_tonumber(L,4));
    tv.normalize();
            
    object->rotation.buildFromAxis(tv, lua_tonumber(L,5));
    
	return 0;
}

static int addRotationAxisLua(lua_State *L) 
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));
    
    Quaternion tq;
    Vector tv;
    tv.setVector(lua_tonumber(L,2),lua_tonumber(L,3),lua_tonumber(L,4));
    tv.normalize();
    tq.buildFromAxis(tv, lua_tonumber(L,5));
                
    object->rotation = object->rotation * tq;
    
	return 0;
}

static int getTimerLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));

    //#    
    lua_pushnumber(L, 0);
    return 1;
}

static int suspendLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));
    
    object->suspend = true;
    object->suspendTime = lua_tonumber(L, 2);
    
    return 0;
}

static int playSoundLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));
    
    const char *s = lua_tostring(L, 2);
    string sound = string(s);
    
    Sound *snd = object->s;
    
    if (snd != NULL) {
        snd->PlaySFX(sound);
    }
    
    return 0;
}

static int addObjectLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));
    
    const char *s = lua_tostring(L, 2);
    string script = string(s);
    
    object->temp = new ScriptedObject(script);
    
    object->temp->setSoundClass(object->s);
    object->temp->setPlayerPtr(object->player);
    object->temp->setCameraPtr(object->camera);
    object->temp->keyState = object->temp->keyState;
    
    object->temp->loadScript(script);
    
    object->add(object->temp);
    
	lua_pushlightuserdata(L, (void*)object->temp);
	return 1;
}

static int removeObjectLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));
    
    object->state = DEAD;
    return 0;
}

static int setModelLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    //Object *object = static_cast<Object*>(lua_touserdata(L, 1));
    ScriptedObject *object = static_cast<ScriptedObject*>(lua_touserdata(L, 1));
    
    const char *s = lua_tostring(L, 2);
    string model = string(s);
    
    object->unload();
    object->load(model);
    
    /*
    if (typeid( *object ) == typeid( ScriptedObject )) {
        const char *s = lua_tostring(L, -1);
        string model = string(s);
        object = static_cast<ScriptedObject*>(object);
        object->unload();
        object->load(model);
    }
    */
    
	return 0;
}




static int setScaleLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));
    
	object->setScale(lua_tonumber(L,2),lua_tonumber(L,3),lua_tonumber(L,4));
	return 0;
}

static int setScriptLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));
    
    const char *s = lua_tostring(L, 2);
    string script = string(s);
    
    object->unloadScript();
    object->loadScript(script);
    
    return 0;
}

static int addParticleSystemLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));
    
    object->setParticleSystem((int)lua_tonumber(L, 2));
    
	return 0;   
}

static int setBgMusicLua(lua_State *L)
{
    const char *s = lua_tostring(L, 1);
    lgameLevel->setMusicPath(string(s));
    
    return 0;
}

static int setSkyBoxLua(lua_State *L)
{
    int x = 3, y = 3;
    int num = 1;
    float **skyColors;
    
    skyColors = new float*[y];
    
    for (int i = 0; i < y; i++) {
        skyColors[i] = new float[x];
        for (int j = 0; j < x; j++) {
            skyColors[i][j] = lua_tonumber(L, num++);        
        }
    }

    lgameLevel->setSkyBox(skyColors, x, y);
    return 0;
}

static int setLightDirectionLua(lua_State *L)
{
    lgameLevel->setLightDirection(lua_tonumber(L, 1), lua_tonumber(L, 2), lua_tonumber(L, 3));
    return 0;
}

static int setTerrainLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Terrain *terrain = static_cast<Terrain*>(lua_touserdata(L, 1));
    
    const char *s = lua_tostring(L, 2);
    
    Vector *light = lgameLevel->getLight();
    
    terrain->loadTerrain(s, light);
}

static int vector_getScalarLua(lua_State *L)
{
    Vector t(lua_tonumber(L, 1), lua_tonumber(L, 2), lua_tonumber(L, 3));
    Vector u(lua_tonumber(L, 4), lua_tonumber(L, 5), lua_tonumber(L, 6));
    lua_pushnumber(L, t.getScaler(u));
    return 1;
}

static int engine_testKeyPressedLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));
    int key = (int)lua_tonumber(L, 2);
    
    int result = (int)(object->keyState->testKeyPressed(key));
    lua_pushnumber(L, result);
    
    return 1;
}

static int engine_testKeyReleasedLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));
    int key = (int)lua_tonumber(L, 2);

    int result = (int)(object->keyState->testKeyReleased(key));
    lua_pushnumber(L, result);
    return 1;
}
      
#endif
