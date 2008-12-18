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

/*
class Scripting
{
    public:
        Scripting() {}
        ~Scripting( void ) {}
        
    private:
        lua_State* lua_; 
};
*/

extern Object* lplayer;
extern Object* lcamera;
extern GameLevel* lgameLevel;
extern Engine* lengine;

// helpers
static Vector loadVector(lua_State *L)
{
    Vector t;
    
    // x
    lua_pushnumber(L, 1);
    lua_gettable(L, -2);
    t.x = (float)lua_tonumber(L, -1);
    lua_pop(L, 1);
    
    // y
    lua_pushnumber(L, 2);
    lua_gettable(L, -2);
    t.y = (float)lua_tonumber(L, -1);
    lua_pop(L, 1);
    
    // z
    lua_pushnumber(L, 3);
    lua_gettable(L, -2);
    t.z = (float)lua_tonumber(L, -1);
    lua_pop(L, 1);
    
    lua_pop(L, 1);
    
    //cout << "Loaded vector: x=" << t.x << " y=" << t.y << " z=" << t.z << endl;
    
    return t;
}

static void returnVector(lua_State *L, Vector &t)
{
    lua_newtable(L);
    
    lua_pushnumber(L, 1);
    lua_pushnumber(L, t.x);
    lua_rawset(L, -3);
    
    lua_pushnumber(L, 2);
    lua_pushnumber(L, t.y);
    lua_rawset(L, -3);
    
    lua_pushnumber(L, 3);
    lua_pushnumber(L, t.z);
    lua_rawset(L, -3);    
}  

static int setPositionLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));
    
	object->setPosition(lua_tonumber(L,2),lua_tonumber(L,3),lua_tonumber(L,4));
	return 0;
}

static int setPositionvLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));
    
	object->setPosition(loadVector(L));
	return 0;
}

static int getPositionLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));
	
    returnVector(L, object->position); 	
	return 1;
}

static int setVelocityLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));
	
	object->setVelocity(lua_tonumber(L, 2),lua_tonumber(L, 3),lua_tonumber(L, 4));
	return 0;
}

static int setVelocityvLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));
	
	object->setVelocity(loadVector(L));
	return 0;
}

static int getVelocityLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));
		
	returnVector(L, object->velocity);
	return 1;
}

static int setRotationVelocityLua(lua_State *L) 
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));
	object->setRotationVelocity(lua_tonumber(L,2),lua_tonumber(L,3),lua_tonumber(L,4));
	return 0;
}

static int setRotationVelocityvLua(lua_State *L) 
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));
	object->setRotationVelocity(loadVector(L));
	return 0;
}

static int getRotationVelocityLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));
		
	returnVector(L, object->rotationVelocity);
	return 1;
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

static int setRotationvLua(lua_State *L) 
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));
	object->setRotationEuler(loadVector(L));
	return 0;
}

static int getRotationLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));
    
    Vector tv;
    object->rotation.getEulerAngles(tv);
                
	returnVector(L, tv);
	return 1;
}

static int getDirectionLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));
		
	returnVector(L, object->direction);
	return 1;
}

static int getUpLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));
		
	returnVector(L, object->up);
	return 1;
}

static int getOrthogonalLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));
	
    Vector rotationAxis;
            
    rotationAxis.crossProduct(object->up, object->direction);
    rotationAxis.normalize();
   	
	returnVector(L, rotationAxis);
	return 1;
}

static int addPositionLua(lua_State *L) 
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));
    
	object->position += Vector(lua_tonumber(L,2),lua_tonumber(L,3),lua_tonumber(L,4));
	return 0;
}

static int addPositionvLua(lua_State *L) 
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));
    
	object->position += Vector(loadVector(L));
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

static int addRotationvLua(lua_State *L) 
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));
    
    Quaternion qt;
    qt.buildFromEuler(loadVector(L));
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

static int addVelocityvLua(lua_State *L) 
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));
	object->velocity += Vector(loadVector(L));
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

static int addRotationVelocityvLua(lua_State *L) 
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));
	object->rotationVelocity += Vector(loadVector(L));
	return 0;
}

static int addScaleLua(lua_State *L) 
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));
	object->scale += Vector(lua_tonumber(L,2),lua_tonumber(L,3),lua_tonumber(L,4));
	return 0;
}

static int addScalevLua(lua_State *L) 
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));
	object->scale += Vector(loadVector(L));
	return 0;
}

static int addScaleRateLua(lua_State *L) 
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));
	object->scaleRate += Vector(lua_tonumber(L,2),lua_tonumber(L,3),lua_tonumber(L,4));
	return 0;
}

static int addScaleRatevLua(lua_State *L) 
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));
	object->scaleRate += Vector(loadVector(L));
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

static int setInterpolationRotationEndAxisvLua(lua_State *L) 
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));
    
    Vector tv = loadVector(L);
    tv.normalize();
            
    object->rInterpEnd.buildFromAxis(tv, lua_tonumber(L,2));
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

static int setInterpolationRotationStartAxisvLua(lua_State *L) 
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));
    
    Vector tv = loadVector(L);
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

static int setInterpolationRotationStartvLua(lua_State *L) 
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));
    
    object->rInterpStart.buildFromEuler(loadVector(L));
	return 0;
}

static int setInterpolationRotationEndLua(lua_State *L) 
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));
    
    object->rInterpEnd.buildFromEuler(lua_tonumber(L,2),lua_tonumber(L,3),lua_tonumber(L,4));
	return 0;
}

static int setInterpolationRotationEndvLua(lua_State *L) 
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));
    
    object->rInterpEnd.buildFromEuler(loadVector(L));
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

static int setRotationVelocityAxisvLua(lua_State *L) 
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));
    
    object->rotationVelocity = loadVector(L);
    object->rotationVelocity.normalize();
    object->rotationVelocity.scale(lua_tonumber(L,2)); 

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

static int setRotationAxisvLua(lua_State *L) 
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));
    
    Vector tv = loadVector(L);
    tv.normalize();
            
    object->rotation.buildFromAxis(tv, lua_tonumber(L,2));
    
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

static int addRotationAxisvLua(lua_State *L) 
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));
    
    Quaternion tq;
    Vector tv = loadVector(L);
    tv.normalize();
    tq.buildFromAxis(tv, lua_tonumber(L,2));
                
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
    
    object->temp = new ScriptedObject();
    
    object->temp->setSoundClass(object->s);
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

static int setScalevLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));
    
	object->setScale(loadVector(L));
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

static int playBgMusicLua(lua_State *L)
{
    const char *s = lua_tostring(L, 1);
    int repeat = (int)lua_tonumber(L, 2);
    lgameLevel->setMusicPath(string(s));
    lgameLevel->getSoundClass()->StopSong();
    lgameLevel->getSoundClass()->PlaySong(s, repeat);
    
    return 0;
}

static int stopBgMusicLua(lua_State *L)
{
    lgameLevel->getSoundClass()->StopSong();
    return 0;
}

static int pauseBgMusicLua(lua_State *L)
{
    lgameLevel->getSoundClass()->PauseSong();
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

static int setLightDirectionvLua(lua_State *L)
{
    Vector t = loadVector(L);
    lgameLevel->setLightDirection(t.x, t.y, t.z);
    return 0;
}

static int setTerrainLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Terrain *terrain = static_cast<Terrain*>(lua_touserdata(L, 1));
    
    const char *s = lua_tostring(L, 2);
    
    Vector *light = lgameLevel->getLight();
    terrain->load(s, light);
    return 0;
}

static int vector_getScalarLua(lua_State *L)
{
    Vector t = loadVector(L);
    Vector u = loadVector(L);
    lua_pushnumber(L, t.getScaler(u));
    return 1;
}

static int vector_normalizeLua(lua_State *L)
{
    Vector t = loadVector(L);
    t.normalize();
    returnVector(L, t);
    return 1;
}

static int vector_dotProductLua(lua_State *L)
{
    Vector t = loadVector(L);
    Vector u = loadVector(L);
    float angle = t.dotProduct(u);
    lua_pushnumber(L, angle);
    return 1;
}

static int vector_crossProductLua(lua_State *L)
{
    Vector t = loadVector(L);
    Vector u = loadVector(L);
    Vector result;
    result.crossProduct(t, u);
    returnVector(L, result);
    return 1;
}

static int engine_testKeyPressedLua(lua_State *L)
{
    //luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    //Object *object = static_cast<Object*>(lua_touserdata(L, 1));
    //int key = (int)lua_tonumber(L, 2);
    int key = (int)lua_tonumber(L, 1);
    //int result = (int)(object->keyState->testKeyPressed(key));
    int result = (int)(lengine->getKeyState()->testKeyPressed(key));
    lua_pushnumber(L, result);
    
    return 1;
}

static int engine_testKeyReleasedLua(lua_State *L)
{
    //luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    //Object *object = static_cast<Object*>(lua_touserdata(L, 1));
    //int key = (int)lua_tonumber(L, 2);
    int key = (int)lua_tonumber(L, 1);
    //int result = (int)(object->keyState->testKeyReleased(key));
    int result = (int)(lengine->getKeyState()->testKeyReleased(key));
    lua_pushnumber(L, result);
    return 1;
}

static int setTypeIdLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));
    object->typeId = (int)lua_tonumber(L, 2);
    return 0;    
}

static int getTypeIdLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));
    lua_pushnumber(L, object->typeId);
    return 1;    
}

static int setScaleRateLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));
    object->scaleRate.setVector(lua_tonumber(L, 2), lua_tonumber(L, 3), lua_tonumber(L, 4));
    return 0;    
}

static int setScaleRatevLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));
    object->scaleRate = loadVector(L);
    return 0;    
}

static int getScaleLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));
		
	returnVector(L, object->scale);
	return 1;
}

static int getScaleRateLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));
		
	returnVector(L, object->scaleRate);
	return 1;
}

static int getScriptValueLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));
    const char *s = lua_tostring(L, 2);
    
    lua_pushnumber(L, object->getScriptValue(s));
    return 1;    
}

static int loadLevelLua(lua_State *L)
{
    const char *s = lua_tostring(L, 1);
    lengine->loadLevel(s);
    
    return 0;
}

static int shutdownLua(lua_State *L)
{
    lengine->shutdown();
    
    return 0;
}

static int pauseLua(lua_State *L)
{
    lengine->pause();
    
    return 0;
}

static int renderTextLua(lua_State *L)
{
    const char *s = lua_tostring(L, 1);
    float x = lua_tonumber(L, 2);
    float y = lua_tonumber(L, 3);
    
    lengine->renderText(s, x, y);
    
    return 0;
}

static int displayTextLua(lua_State *L)
{
    const char *s = lua_tostring(L, 1);
    char * t = (char*)s;
    float x = lua_tonumber(L, 2);
    float y = lua_tonumber(L, 3);
    float z = lua_tonumber(L, 4);
    float sx = lua_tonumber(L, 5);
    float sy = lua_tonumber(L, 6);
    
    displayText(t, x, y, z, sx, sy);
    
    return 0;
}

static int getFpsLua(lua_State *L)
{
    lua_pushnumber(L, lengine->getFps());
    return 1;
}

static int getCameraIdLua(lua_State *L)
{
    lua_pushnumber(L, lgameLevel->getCamera()->id);
    return 1;
}

static int getLightDirectionLua(lua_State *L)
{
    returnVector(L, *lgameLevel->getLight());
    return 1;
}

static int setCompleteLua(lua_State *L)
{
    lgameLevel->setComplete(lua_tonumber(L, 1));
    return 0;
}

static int testLevelCompleteLua(lua_State *L)
{
    lua_pushnumber(L, lgameLevel->checkComplete());
    return 1;
}

static int getLevelIdLua(lua_State *L)
{
    lua_pushnumber(L, lgameLevel->getId());
    return 1;
}

static int setLevelIdLua(lua_State *L)
{
    lgameLevel->setId(lua_tonumber(L, 1));
    return 0;
}

static int registerFunctions(lua_State *L, int level)
{
    lua_register(L, "setPosition", setPositionLua);
    lua_register(L, "setPositionv", setPositionvLua);
    lua_register(L, "getPosition", getPositionLua);
    lua_register(L, "setVelocity", setVelocityLua);
    lua_register(L, "setVelocityv", setVelocityvLua);
    lua_register(L, "getVelocity", getVelocityLua);
    lua_register(L, "setRotationVelocity", setRotationVelocityLua);
    lua_register(L, "setRotationVelocityv", setRotationVelocityvLua);
    lua_register(L, "getRotationVelocity", getRotationVelocityLua);
    lua_register(L, "setSpeed", setSpeedLua);
    lua_register(L, "getSpeed", getSpeedLua);
    lua_register(L, "setRotation", setRotationLua);
    lua_register(L, "setRotationv", setRotationvLua);
    lua_register(L, "getRotation", getRotationLua);
    lua_register(L, "getCamera", getCameraLua);
    lua_register(L, "getPlayer", getPlayerLua);
    lua_register(L, "getDirection", getDirectionLua);
    lua_register(L, "getUp", getUpLua);
    lua_register(L, "getOrthogonal", getOrthogonalLua);
    lua_register(L, "addPosition", addPositionLua);
    lua_register(L, "addPositionv", addPositionvLua);
    lua_register(L, "addRotation", addRotationLua);
    lua_register(L, "addRotationv", addRotationvLua);
    lua_register(L, "addVelocity", addVelocityLua);
    lua_register(L, "addVelocityv", addVelocityvLua);
    lua_register(L, "addSpeed", addSpeedLua);
    lua_register(L, "addRotationVelocity", addRotationVelocityLua);   
    lua_register(L, "addRotationVelocityv", addRotationVelocityvLua);   
    lua_register(L, "addScale", addScaleLua);   
    lua_register(L, "addScalev", addScalevLua);   
    lua_register(L, "addScaleRate", addScaleRateLua);   
    lua_register(L, "addScaleRatev", addScaleRatevLua);   
    lua_register(L, "setInterpolationRotationStartAxis", setInterpolationRotationStartAxisLua);
    lua_register(L, "setInterpolationRotationStartAxisv", setInterpolationRotationStartAxisvLua);
    lua_register(L, "setInterpolationRotationEndAxis", setInterpolationRotationEndAxisLua);
    lua_register(L, "setInterpolationRotationEndAxisv", setInterpolationRotationEndAxisvLua);
    lua_register(L, "setInterpolationRotationStart", setInterpolationRotationStartLua);
    lua_register(L, "setInterpolationRotationStartv", setInterpolationRotationStartvLua);
    lua_register(L, "setInterpolationRotationEnd", setInterpolationRotationEndLua);
    lua_register(L, "setInterpolationRotationEndv", setInterpolationRotationEndvLua);
    lua_register(L, "setInterpolationEnable", setInterpolationEnableLua);
    lua_register(L, "setInterpolationVariable", setInterpolationVariableLua);
    lua_register(L, "setRotationVelocityAxis", setRotationVelocityAxisLua);
    lua_register(L, "setRotationAxis", setRotationAxisLua);
    lua_register(L, "setRotationAxisv", setRotationAxisvLua);
    lua_register(L, "addRotationAxis", addRotationAxisLua);
    lua_register(L, "addRotationAxisv", addRotationAxisvLua);
    lua_register(L, "getTimer", getTimerLua);
    lua_register(L, "suspend", suspendLua);
    lua_register(L, "playSound", playSoundLua);
    lua_register(L, "addObject", addObjectLua);
    lua_register(L, "removeObject", removeObjectLua);
    lua_register(L, "setModel", setModelLua);
    lua_register(L, "setScale", setScaleLua);
    lua_register(L, "setScalev", setScalevLua);
    lua_register(L, "setScript", setScriptLua);
    lua_register(L, "setScaleRate", setScaleRateLua);   
    lua_register(L, "setScaleRatev", setScaleRatevLua);
    lua_register(L, "getScale", getScaleLua);
    lua_register(L, "getScaleRate", getScaleRateLua);
    lua_register(L, "addParticleSystem", addParticleSystemLua);     
    lua_register(L, "vector_getScalar", vector_getScalarLua); 
    lua_register(L, "engine_testKeyPressed", engine_testKeyPressedLua); 
    lua_register(L, "engine_testKeyReleased", engine_testKeyReleasedLua); 
    lua_register(L, "vector_normalize", vector_normalizeLua); 
    lua_register(L, "vector_dotProduct", vector_dotProductLua); 
    lua_register(L, "vector_crossProduct", vector_crossProductLua); 
    lua_register(L, "getTypeId", getTypeIdLua); 
    lua_register(L, "setTypeId", setTypeIdLua); 
    lua_register(L, "getScriptValue", getScriptValueLua);
    
    // game level only
    if (level < 2) {
        lua_register(L, "playBgMusic", playBgMusicLua);
        lua_register(L, "stopBgMusic", stopBgMusicLua);
        lua_register(L, "pauseBgMusic", pauseBgMusicLua);
        lua_register(L, "setSkyBox", setSkyBoxLua);
        lua_register(L, "setLightDirection", setLightDirectionLua);
        lua_register(L, "setLightDirectionv", setLightDirectionvLua);
        lua_register(L, "setTerrain", setTerrainLua);
        lua_register(L, "displayText", displayTextLua);
        lua_register(L, "renderText", renderTextLua);
        lua_register(L, "getFps", getFpsLua);
        lua_register(L, "getCameraId", getCameraIdLua);
        lua_register(L, "getLightDirection", getLightDirectionLua);
        lua_register(L, "setComplete", setCompleteLua);
    }
    
    if (level == 0) {
        lua_register(L, "loadLevel", loadLevelLua);  
        lua_register(L, "shutdown", shutdownLua);  
        lua_register(L, "pause", pauseLua);  
        lua_register(L, "testLevelComplete", testLevelCompleteLua);
        lua_register(L, "getLevelId", getLevelIdLua);
        lua_register(L, "setLevelId", setLevelIdLua);
    }
}

/*
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
}
*/


#endif
