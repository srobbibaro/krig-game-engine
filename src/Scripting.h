#ifndef _SCRIPTING_H_
#define _SCRIPTING_H_

extern "C" {
    #include "lua5.1/lua.h"
    #include "lua5.1/lualib.h"
    #include "lua5.1/lauxlib.h"
}

#include "Object.h"
#include "text.h"
#include "ScriptedObject.h"
#include "GameLevel.h"
#include "matrix.h"
#include "Engine.h"

extern Object* g_script_player;
extern Object* g_script_camera;
extern GameLevel* g_script_game_level;
extern Engine* g_script_engine;

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

    Vector position = object->getPosition();

    returnVector(L, position);
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

    Vector velocity = object->getVelocity();

	returnVector(L, velocity);
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

    Vector rotationVelocity = object->getRotationVelocity();

	returnVector(L, rotationVelocity);
	return 1;
}

static int setSpeedLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));

	object->setSpeed(lua_tonumber(L, 2),lua_tonumber(L, 3),lua_tonumber(L, 4));
	return 0;
}

static int setSpeedvLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));

	object->setSpeed(loadVector(L));
	return 0;
}

static int getSpeedLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));

    Vector speed = object->getSpeed();

	returnVector(L, speed);
	return 1;
}

static int getPlayerLua(lua_State *L)
{
	lua_pushlightuserdata(L, (void*)g_script_player);
	return 1;
}

static int getCameraLua(lua_State *L)
{
	lua_pushlightuserdata(L, (void*)g_script_camera);
	return 1;
}

static int setRotationLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));
	object->setRotationEuler(lua_tonumber(L,2),lua_tonumber(L,3),lua_tonumber(L,4));
	object->setRotationChanged(true);
	return 0;
}

static int setRotationvLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));
	object->setRotationEuler(loadVector(L));
    object->setRotationChanged(true);
	return 0;
}

static int getRotationLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));

    Vector tv;

    object->getRotation().getEulerAngles(tv);

	returnVector(L, tv);
	return 1;
}

static int getDirectionLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));

    Vector direction = object->getDirection();

	returnVector(L, direction);
	return 1;
}

static int getUpLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));

    Vector up = object->getUp();

	returnVector(L, up);
	return 1;
}

static int getOrthogonalLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));

    Vector orth = object->getOrth();

	returnVector(L, orth);
	return 1;
}

static int addPositionLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));

    Vector position = object->getPosition();

    position += Vector(lua_tonumber(L,2),lua_tonumber(L,3),lua_tonumber(L,4));

    object->setPosition(position);

	return 0;
}

static int addPositionvLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));

    Vector position = object->getPosition();

	position += Vector(loadVector(L));

	object->setPosition(position);
	return 0;
}

static int addRotationLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));

    Quaternion qt;
    qt.buildFromEuler(lua_tonumber(L,2),lua_tonumber(L,3),lua_tonumber(L,4));
    object->setRotation(object->getRotation() * qt);

	return 0;
}

static int camera_addRotationLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Camera *object = static_cast<Camera*>(lua_touserdata(L, 1));

    Quaternion qt;
    qt.buildFromEuler(lua_tonumber(L,2),lua_tonumber(L,3),lua_tonumber(L,4));
    object->worldRotation = object->worldRotation * qt;

	return 0;
}

static int camera_getFrustumPlaneLua(lua_State *L)
{
    int plane_num = lua_tonumber(L,1);
    float a, b, c, d;

    static_cast<Camera*>(g_script_camera)->getFrustum()->getPlaneDefinition(plane_num, a, b, c, d);

    lua_newtable(L);

    lua_pushnumber(L, 1);
    lua_pushnumber(L, a);
    lua_rawset(L, -3);

    lua_pushnumber(L, 2);
    lua_pushnumber(L, b);
    lua_rawset(L, -3);

    lua_pushnumber(L, 3);
    lua_pushnumber(L, c);
    lua_rawset(L, -3);

    lua_pushnumber(L, 4);
    lua_pushnumber(L, d);
    lua_rawset(L, -3);

	return 1;
}

static int addRotationvLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));

    Quaternion qt;
    qt.buildFromEuler(loadVector(L));
    object->setRotation(object->getRotation() * qt);

	return 0;
}

static int addVelocityLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));

    Vector velocity = object->getVelocity();

	velocity += Vector(lua_tonumber(L,2),lua_tonumber(L,3),lua_tonumber(L,4));

	object->setVelocity(velocity);

	return 0;
}

static int addVelocityvLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));

    Vector velocity = object->getVelocity();

	velocity += Vector(loadVector(L));

	object->setVelocity(velocity);
	return 0;
}

static int addSpeedLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));

    Vector speed = object->getSpeed();

	speed += Vector(lua_tonumber(L,2),lua_tonumber(L,3),lua_tonumber(L,4));

	object->setSpeed(speed);
	return 0;
}

static int addSpeedvLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));

    Vector speed = object->getSpeed();

	speed += Vector(loadVector(L));

	object->setSpeed(speed);

	return 0;
}

static int addRotationVelocityLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));

    Vector rotationVelocity = object->getRotationVelocity();

	rotationVelocity += Vector(lua_tonumber(L,2),lua_tonumber(L,3),lua_tonumber(L,4));

	object->setRotationVelocity(rotationVelocity);

	return 0;
}

static int addRotationVelocityvLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));

    Vector rotationVelocity = object->getRotationVelocity();

	rotationVelocity += Vector(loadVector(L));

	object->setRotationVelocity(rotationVelocity);

	return 0;
}

static int addScaleLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));

    Vector scale = object->getScale();

	scale += Vector(lua_tonumber(L,2),lua_tonumber(L,3),lua_tonumber(L,4));

	object->setScale(scale);

	return 0;
}

static int addScalevLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));

    Vector scale = object->getScale();

    scale += Vector(loadVector(L));

	object->setScale(scale);

	return 0;
}

static int addScaleRateLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));

    Vector scaleRate = object->getScaleRate();

	scaleRate += Vector(lua_tonumber(L,2),lua_tonumber(L,3),lua_tonumber(L,4));

	object->setScaleRate(scaleRate);

	return 0;
}

static int addScaleRatevLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);

    Object *object = static_cast<Object*>(lua_touserdata(L, 1));

    Vector scaleRate = object->getScaleRate();

    scaleRate += Vector(loadVector(L));

	object->setScaleRate(scaleRate);

	return 0;
}

static int setInterpolationRotationEndAxisLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));

    Vector tv;
    tv.setVector(lua_tonumber(L,2),lua_tonumber(L,3),lua_tonumber(L,4));
    tv.normalize();

    Quaternion rInterpEnd = object->getRInterpEnd();

    rInterpEnd.buildFromAxis(tv, lua_tonumber(L,5));

    object->setRInterpEnd(rInterpEnd);

	return 0;
}

static int setInterpolationRotationEndAxisvLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));

    Vector tv = loadVector(L);
    tv.normalize();

    Quaternion rInterpEnd = object->getRInterpEnd();

    rInterpEnd.buildFromAxis(tv, lua_tonumber(L,2));

    object->setRInterpEnd(rInterpEnd);

	return 0;
}

static int setInterpolationRotationStartAxisLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));

    Quaternion rInterpStart = object->getRInterpStart();

    Vector tv;
    tv.setVector(lua_tonumber(L,2),lua_tonumber(L,3),lua_tonumber(L,4));
    tv.normalize();

    rInterpStart.buildFromAxis(tv, lua_tonumber(L,5));

    object->setRInterpStart(rInterpStart);

	return 0;
}

static int setInterpolationRotationStartAxisvLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));

    Quaternion rInterpStart = object->getRInterpStart();

    Vector tv = loadVector(L);
    tv.normalize();

    rInterpStart.buildFromAxis(tv, lua_tonumber(L,5));

    object->setRInterpStart(rInterpStart);

	return 0;
}

static int setInterpolationRotationStartLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));

    Quaternion rInterpStart = object->getRInterpStart();

    rInterpStart.buildFromEuler(lua_tonumber(L,2),lua_tonumber(L,3),lua_tonumber(L,4));

    object->setRInterpStart(rInterpStart);

	return 0;
}

static int setInterpolationRotationStartvLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));

    Quaternion rInterpStart = object->getRInterpStart();

    rInterpStart.buildFromEuler(loadVector(L));

    object->setRInterpStart(rInterpStart);

	return 0;
}

static int setInterpolationRotationEndLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));

    Quaternion rInterpEnd = object->getRInterpEnd();

    rInterpEnd.buildFromEuler(lua_tonumber(L,2),lua_tonumber(L,3),lua_tonumber(L,4));

    object->setRInterpEnd(rInterpEnd);

	return 0;
}

static int setInterpolationRotationEndvLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));

    Quaternion rInterpEnd = object->getRInterpEnd();

    rInterpEnd.buildFromEuler(loadVector(L));

    object->setRInterpEnd(rInterpEnd);

	return 0;
}

static int setInterpolationEnableLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));

    // TODO: there's probably a better wy to do this...
    int op = (int)lua_tonumber(L,2);

    if (op == 0)
        object->setIsInterpolationEnabled(false);
    else
        object->setIsInterpolationEnabled(true);

    return (0);
}

static int setInterpolationVariableBeginValueLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));
    object->setValInterpBegin(lua_tonumber(L, 2));
	return 0;
}

static int setInterpolationVariableEndValueLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));
    object->setValInterpEnd(lua_tonumber(L, 2));
	return 0;
}

static int setInterpolationVariableCurrentValueLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));
    object->setValInterpCurrent(lua_tonumber(L, 2));
	return 0;
}

static int setRotationVelocityAxisLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));

    Vector rotationVelocity = object->getRotationVelocity();

    rotationVelocity.setVector(lua_tonumber(L,2),lua_tonumber(L,3),lua_tonumber(L,4));
    rotationVelocity.normalize();
    rotationVelocity.scale(lua_tonumber(L,5));

    object->setRotationVelocity(rotationVelocity);

	return 0;
}

static int setRotationVelocityAxisvLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));

    Vector rotationVelocity = object->getRotationVelocity();

    rotationVelocity = loadVector(L);
    rotationVelocity.normalize();
    rotationVelocity.scale(lua_tonumber(L,2));

    object->setRotationVelocity(rotationVelocity);

	return 0;
}

static int setRotationAxisLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));

    Vector tv;
    tv.setVector(lua_tonumber(L,2),lua_tonumber(L,3),lua_tonumber(L,4));
    //tv.normalize();

    Quaternion rotation;

    rotation.buildFromAxis(tv, lua_tonumber(L,5));
   	object->setRotationChanged(true);

   	object->setRotation(rotation);

	return 0;
}

static int setRotationAxisvLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));

    Vector tv = loadVector(L);
    //tv.normalize();

    Quaternion rotation;

    rotation.buildFromAxis(tv, lua_tonumber(L,2));

    Vector v;
    rotation.getEulerAngles(v);

   	object->setRotationChanged(true);

   	object->setRotation(rotation);

	return 0;
}

static int addRotationAxisLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));

    Quaternion tq;
    Vector tv;
    tv.setVector(lua_tonumber(L,2),lua_tonumber(L,3),lua_tonumber(L,4));
    //tv.normalize();
    tq.buildFromAxis(tv, lua_tonumber(L,5));

    Quaternion rotation = object->getRotation();

    rotation = rotation * tq;

    object->setRotation(rotation);

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

    Quaternion rotation = object->getRotation();

    rotation = rotation * tq;

    object->setRotation(rotation);

	return 0;
}

static int getTimerLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));

    // TODO: Is this logic working properly?
    lua_pushnumber(L, 0);
    return 1;
}

static int suspendLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));
    object->setSuspendTime(lua_tonumber(L, 2));
    return 0;
}

static int playSoundLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));

    const char *s = lua_tostring(L, 2);
    string sound = string(s);

    SoundFX *soundFx = g_script_engine->getSoundFxClass();

    if (soundFx != NULL)
        soundFx->PlaySFX(sound);

    return 0;
}

static int addObjectLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));

    const char *s = lua_tostring(L, 2);
    string script = string(s);

    int n = lua_gettop(L);
    n = n - 2;

    if (n > 8) n = 8;

    float args[8];

    for (int i = 0; i < n; i++) {
        args[i] = lua_tonumber(L, i+3);
    }

    ScriptedObject * temp = g_script_game_level->addObject(script, args, n);

	lua_pushlightuserdata(L, (void*)temp);
	return 1;
}

static int addTextLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));

    const char *s = lua_tostring(L, 2);
    string script = string(s);

    const char *t = lua_tostring(L, 3);
    string text = string(t);

    int n = lua_gettop(L);
    n = n - 3;

    if (n > 8) n = 8;

    float args[8];

    for (int i = 0; i < n; i++) {
        args[i] = lua_tonumber(L, i+4);
    }

    ScriptTextType *temp = g_script_game_level->addScriptTextType(script, args, n);
    temp->text = text;

	lua_pushlightuserdata(L, (void*)temp);
	return 1;
}

static int removeObjectLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));

    object->setState(DEAD);
    return 0;
}

static int setModelLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    //Object *object = static_cast<Object*>(lua_touserdata(L, 1));
    ScriptedObject *object = static_cast<ScriptedObject*>(lua_touserdata(L, 1));

    const char *s = lua_tostring(L, 2);
    string model = string(s);

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

static int terrain_getVertexHeightLua(lua_State *L)
{
    int x = lua_tonumber(L,1);
    int z = lua_tonumber(L,2);

    float height = (float)g_script_game_level->getTerrain()->getVertexHeight(x, z);

    lua_pushnumber(L, height);

	return 1;
}

static int terrain_getVertexColorLua(lua_State *L)
{
    int x = lua_tonumber(L,1);
    int z = lua_tonumber(L,2);

    Vector color = g_script_game_level->getTerrain()->getVertexColor(x, z);

    returnVector(L, color);

	return 1;
}

static int terrain_getVertexTypeLua(lua_State *L)
{
    int x = lua_tonumber(L,1);
    int z = lua_tonumber(L,2);

    float type = g_script_game_level->getTerrain()->getVertexType(x, z);

    lua_pushnumber(L, type);

	return 1;
}

static int terrain_setVertexHeightLua(lua_State *L)
{
    int x = lua_tonumber(L,1);
    int z = lua_tonumber(L,2);
    float height = lua_tonumber(L,3);

    g_script_game_level->getTerrain()->setVertexHeight(x, z, height);

	return 0;
}

static int terrain_setVertexColorLua(lua_State *L)
{
    int x = lua_tonumber(L,1);
    int z = lua_tonumber(L,2);
    Vector color = loadVector(L);

    g_script_game_level->getTerrain()->setVertexColor(x, z, color);

	return 0;
}

static int terrain_setVertexTypeLua(lua_State *L)
{
    int x = lua_tonumber(L,1);
    int z = lua_tonumber(L,2);
    int type = lua_tonumber(L,3);

    g_script_game_level->getTerrain()->setVertexType(x, z, type);

	return 0;
}

static int terrain_getHeightLua(lua_State *L)
{
    float x = lua_tonumber(L,1);
    float z = lua_tonumber(L,2);

    float height = (float)g_script_game_level->getTerrain()->getHeight(x, z);

    lua_pushnumber(L, height);

	return 1;
}

static int setScaleLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));

	object->setScale(lua_tonumber(L,2),lua_tonumber(L,3),lua_tonumber(L,4));
	object->setScaleChanged(true);
	//object->update(lgameLevel->getLightDirection());
	return 0;
}

static int setScalevLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));

	object->setScale(loadVector(L));
    object->setScaleChanged(true);
	return 0;
}

static int setScriptLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));

    const char *s = lua_tostring(L, 2);
    string script = string(s);

    float args[8];

    object->unloadScript();
    object->setScript(script);
    object->loadScript(script, args, 0);

    return 0;
}

static int enableAlwaysLitLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));
    object->setIsAlwaysLit(true);
    return 0;
}

static int disableAlwaysLitLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));
    object->setIsAlwaysLit(false);
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
    g_script_game_level->setMusicPath(string(s));
    g_script_game_level->getMusic()->StopSong();
    g_script_game_level->getMusic()->PlaySong(s, repeat);
    return 0;
}

static int stopBgMusicLua(lua_State *L)
{
    g_script_game_level->getMusic()->StopSong();
    return 0;
}

static int pauseBgMusicLua(lua_State *L)
{
    g_script_game_level->getMusic()->PauseSong();
    return 0;
}

static int setSkyBoxLua(lua_State *L)
{
    int num = 1;

    float skyColors[3][3];
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            skyColors[i][j] = lua_tonumber(L, num++);
        }
    }

    g_script_game_level->setSkyBox(skyColors, 3, 3);
    return 0;
}

static int setLightDirectionLua(lua_State *L)
{
    g_script_game_level->setLightDirection(lua_tonumber(L, 1), lua_tonumber(L, 2), lua_tonumber(L, 3));
    return 0;
}

static int setLightDirectionvLua(lua_State *L)
{
    Vector t = loadVector(L);
    g_script_game_level->setLightDirection(t.x, t.y, t.z);
    return 0;
}

static int setTerrainLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Terrain *terrain = static_cast<Terrain*>(lua_touserdata(L, 1));

    const char *s = lua_tostring(L, 2);

    Vector *lightDirection = g_script_game_level->getLightDirection();
    terrain->load(s, lightDirection);
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
    int result = (int)(g_script_engine->getKeyState()->testKeyPressed(key));
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
    int result = (int)(g_script_engine->getKeyState()->testKeyReleased(key));
    lua_pushnumber(L, result);
    return 1;
}

static int engine_testSpecialKeyPressedLua(lua_State *L)
{
    //luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    //Object *object = static_cast<Object*>(lua_touserdata(L, 1));
    //int key = (int)lua_tonumber(L, 2);
    int key = (int)lua_tonumber(L, 1);
    //int result = (int)(object->keyState->testKeyPressed(key));
    int result = (int)(g_script_engine->getSpecialKeyState()->testKeyPressed(key));
    lua_pushnumber(L, result);

    return 1;
}

static int engine_testSpecialKeyReleasedLua(lua_State *L)
{
    //luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    //Object *object = static_cast<Object*>(lua_touserdata(L, 1));
    //int key = (int)lua_tonumber(L, 2);
    int key = (int)lua_tonumber(L, 1);
    //int result = (int)(object->keyState->testKeyReleased(key));
    int result = (int)(g_script_engine->getSpecialKeyState()->testKeyReleased(key));
    lua_pushnumber(L, result);
    return 1;
}

static int engine_getMouseCoordinatesLua(lua_State *L)
{
    float x = g_script_engine->getMouseX();
    float y = g_script_engine->getMouseY();

    lua_newtable(L);

    lua_pushnumber(L, 1);
    lua_pushnumber(L, x);
    lua_rawset(L, -3);

    lua_pushnumber(L, 2);
    lua_pushnumber(L, y);
    lua_rawset(L, -3);
    return 1;
}

static int level_findObjectOfTypeLua(lua_State *L)
{
    int type = (int)lua_tonumber(L, 1);
    Object *temp = g_script_game_level->findEnemyOfType(type);

    if (temp != NULL)
        lua_pushlightuserdata(L, (void*)temp);
    else
        lua_pushnil(L);

    return 1;
}


static int setTypeIdLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));
    object->setTypeId((int)lua_tonumber(L, 2));
    return 0;
}

static int getTypeIdLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));
    lua_pushnumber(L, object->getTypeId());
    return 1;
}

static int setScaleRateLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));

    Vector scaleRate = object->getScaleRate();

    scaleRate.setVector(lua_tonumber(L, 2), lua_tonumber(L, 3), lua_tonumber(L, 4));

    object->setScaleRate(scaleRate);
    return 0;
}

static int setScaleRatevLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));

    Vector scaleRate = object->getScaleRate();

    scaleRate = loadVector(L);

    object->setScaleRate(scaleRate);

    return 0;
}

static int getScaleLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));

    Vector scale = object->getScale();

	returnVector(L, scale);
	return 1;
}

static int getScaleRateLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));

    Vector scaleRate = object->getScaleRate();

	returnVector(L, scaleRate);
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

static int setScriptValueLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));
    const char *s = lua_tostring(L, 2);
    float value = (int)lua_tonumber(L, 3);

    object->setScriptValue(s, value);
    return 0;
}

static int loadLevelLua(lua_State *L)
{
    const char *s = lua_tostring(L, 1);
    g_script_engine->loadLevel(s);

    return 0;
}

static int loadLevelFromBufferLua(lua_State *L)
{
    const char *s = lua_tostring(L, 1);
    g_script_engine->loadLevel(s);

    return 0;
}

static int shutdownLua(lua_State *L)
{
    g_script_engine->shutdown();

    return 0;
}

static int pauseLua(lua_State *L)
{
    g_script_engine->pause();

    return 0;
}

static int renderTextLua(lua_State *L)
{
    const char *s = lua_tostring(L, 1);
    float x = lua_tonumber(L, 2);
    float y = lua_tonumber(L, 3);

    g_script_engine->renderText(s, x, y);

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
    lua_pushnumber(L, g_script_engine->getFps());
    return 1;
}

static int getCameraIdLua(lua_State *L)
{
    lua_pushnumber(L, g_script_game_level->getCamera()->id_);
    return 1;
}

static int getLightDirectionLua(lua_State *L)
{
    returnVector(L, *g_script_game_level->getLightDirection());
    return 1;
}

static int setCompleteLua(lua_State *L)
{
    g_script_game_level->setComplete(lua_tonumber(L, 1));
    return 0;
}

static int testLevelCompleteLua(lua_State *L)
{
    lua_pushnumber(L, g_script_game_level->checkComplete());
    return 1;
}

static int getLevelIdLua(lua_State *L)
{
    lua_pushnumber(L, g_script_game_level->getId());
    return 1;
}

static int setLevelIdLua(lua_State *L)
{
    g_script_game_level->setId((int)lua_tonumber(L, 1));
    return 0;
}

static int enableCollisionDetectionLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));
    object->setCollisionDetectionEnabled(true);
    return 0;
}

static int disableCollisionDetectionLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));
    object->setCollisionDetectionEnabled(false);
    return 0;
}

static int getCollisionDetectionEnabledLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));
    lua_pushnumber(L, object->getCollisionDetectionEnabled());
    return 1;
}

static int setActiveLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));
    object->setActive(true);
    return 0;
}

static int setInactiveLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));
    object->setActive(false);
    return 0;
}

static int getActiveLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));
    lua_pushnumber(L, object->getActive());
    return 1;
}

static int enableDrawLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));
    object->setDrawEnabled(true);
    return 0;
}

static int disableDrawLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));
    object->setDrawEnabled(false);
    return 0;
}

static int getDrawEnabledLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));
    lua_pushnumber(L, object->getDrawEnabled());
    return 1;
}

static int getBoundingSphereRadiusLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));
    lua_pushnumber(L, object->getBoundingSphere().getRadius());
    return 1;
}

static int getInViewLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Object *object = static_cast<Object*>(lua_touserdata(L, 1));
    lua_pushnumber(L, object->getInView());
    return 1;
}

static int swapLevelLua(lua_State *L)
{
    g_script_engine->swapLevel();
    return 0;
}

static int setFadeRateLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    ScriptTextType *object = static_cast<ScriptTextType*>(lua_touserdata(L, 1));
    object->fadeRate = lua_tonumber(L, 2);
    return 0;
}

static int getFadeRateLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    ScriptTextType *object = static_cast<ScriptTextType*>(lua_touserdata(L, 1));
    lua_pushnumber(L, object->fadeRate);
    return 1;
}

static int setAlphaLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    ScriptTextType *object = static_cast<ScriptTextType*>(lua_touserdata(L, 1));
    object->color[3] = lua_tonumber(L, 2);
    return 0;
}

static int getAlphaLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    ScriptTextType *object = static_cast<ScriptTextType*>(lua_touserdata(L, 1));
    lua_pushnumber(L, object->color[3]);
    return 1;
}

static int setColorLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    ScriptTextType *object = static_cast<ScriptTextType*>(lua_touserdata(L, 1));
    object->color[0] = lua_tonumber(L, 2);
    object->color[1] = lua_tonumber(L, 3);
    object->color[2] = lua_tonumber(L, 4);
    return 0;
}

static int setColorvLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    ScriptTextType *object = static_cast<ScriptTextType*>(lua_touserdata(L, 1));
    Vector t = loadVector(L);
    object->color[0] = t.x;
    object->color[1] = t.y;
    object->color[2] = t.z;
	return 0;
}

static int getColorLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    ScriptTextType *object = static_cast<ScriptTextType*>(lua_touserdata(L, 1));
    Vector t; t.setVector(object->color[0], object->color[1], object->color[2]);
    returnVector(L, t);
    return 1;
}

static int setWidthLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    ScriptTextType *object = static_cast<ScriptTextType*>(lua_touserdata(L, 1));
    object->width = lua_tonumber(L, 2);
    return 0;
}

static int getWidthLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    ScriptTextType *object = static_cast<ScriptTextType*>(lua_touserdata(L, 1));
    lua_pushnumber(L, object->width);
    return 1;
}

static int orientOnTerrainLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Model *object = static_cast<Model*>(lua_touserdata(L, 1));

    Quaternion rotation;
    rotation.buildFromEuler(lua_tonumber(L,2),lua_tonumber(L,3),lua_tonumber(L,4));

    object->orientOnTerrain(g_script_game_level->getTerrain(), rotation);

    return 0;
}

static int setHeightFromTerrainLua(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
    Model *object = static_cast<Model*>(lua_touserdata(L, 1));

    float offset = lua_tonumber(L, 2);

    object->setHeightFromTerrain(g_script_game_level->getTerrain(), offset);

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
    lua_register(L, "setSpeedv", setSpeedvLua);
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
    lua_register(L, "addSpeedv", addSpeedvLua);
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
    lua_register(L, "setInterpolationVariableBeginValue", setInterpolationVariableBeginValueLua);
    lua_register(L, "setInterpolationVariableEndValue", setInterpolationVariableEndValueLua);
    lua_register(L, "setInterpolationVariableCurrentValue", setInterpolationVariableCurrentValueLua);
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
    lua_register(L, "engine_testSpecialKeyPressed", engine_testSpecialKeyPressedLua);
    lua_register(L, "engine_testSpecialKeyReleased", engine_testSpecialKeyReleasedLua);
    lua_register(L, "engine_getMouseCoordinates", engine_getMouseCoordinatesLua);
    lua_register(L, "level_findObjectOfType", level_findObjectOfTypeLua);
    lua_register(L, "vector_normalize", vector_normalizeLua);
    lua_register(L, "vector_dotProduct", vector_dotProductLua);
    lua_register(L, "vector_crossProduct", vector_crossProductLua);
    lua_register(L, "getTypeId", getTypeIdLua);
    lua_register(L, "setTypeId", setTypeIdLua);
    lua_register(L, "getScriptValue", getScriptValueLua);
    lua_register(L, "setScriptValue", setScriptValueLua);
    lua_register(L, "getCollisionDetectionEnabled", getCollisionDetectionEnabledLua);
    lua_register(L, "enableCollisionDetection", enableCollisionDetectionLua);
    lua_register(L, "disableCollisionDetection", disableCollisionDetectionLua);
    lua_register(L, "getActive", getActiveLua);
    lua_register(L, "setActive", setActiveLua);
    lua_register(L, "setInactive", setInactiveLua);
    lua_register(L, "getDrawEnabled", getDrawEnabledLua);
    lua_register(L, "enableDraw", enableDrawLua);
    lua_register(L, "disableDraw", disableDrawLua);
    lua_register(L, "getBoundingSphereRadius", getBoundingSphereRadiusLua);
    lua_register(L, "getInView", getInViewLua);
    lua_register(L, "addText", addTextLua);
    lua_register(L, "enableAlwaysLit", enableAlwaysLitLua);
    lua_register(L, "disableAlwaysLit", disableAlwaysLitLua);
    lua_register(L, "orientOnTerrain", orientOnTerrainLua);
    lua_register(L, "setHeightFromTerrain", setHeightFromTerrainLua);
    lua_register(L, "terrain_getVertexHeight", terrain_getVertexHeightLua);
    lua_register(L, "terrain_getVertexColor", terrain_getVertexColorLua);
    lua_register(L, "terrain_getVertexType", terrain_getVertexTypeLua);
    lua_register(L, "terrain_setVertexHeight", terrain_setVertexHeightLua);
    lua_register(L, "terrain_setVertexColor", terrain_setVertexColorLua);
    lua_register(L, "terrain_setVertexType", terrain_setVertexTypeLua);
    lua_register(L, "terrain_getHeight", terrain_getHeightLua);
    lua_register(L, "camera_addRotation", camera_addRotationLua);
    lua_register(L, "camera_getFrustumPlane", camera_getFrustumPlaneLua);

    // text based functions
    lua_register(L, "setFadeRate", setFadeRateLua);
    lua_register(L, "getFadeRate", getFadeRateLua);
    lua_register(L, "setAlpha", setAlphaLua);
    lua_register(L, "getAlpha", getAlphaLua);
    lua_register(L, "setColor", setColorLua);
    lua_register(L, "setColorv", setColorvLua);
    lua_register(L, "getColor", getColorLua);
    lua_register(L, "setWidth", setWidthLua);
    lua_register(L, "getWidth", getWidthLua);

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
        lua_register(L, "shutdown", shutdownLua);
    }

    if (level == 0) {
        lua_register(L, "loadLevel", loadLevelLua);
        lua_register(L, "loadLevelFromBuffer", loadLevelFromBufferLua);
        lua_register(L, "pause", pauseLua);
        lua_register(L, "testLevelComplete", testLevelCompleteLua);
        lua_register(L, "getLevelId", getLevelIdLua);
        lua_register(L, "setLevelId", setLevelIdLua);
        lua_register(L, "swapLevel", swapLevelLua);
    }
}

/*
void GameLevel::loadObject(lua_State* L, int number)
{
    lua_pushnumber(L, number);
    lua_gettable(L, -2);

    lua_pushstring(L, "type");
    lua_gettable(L, -2);
    int objectType = (int)lua_tonumber(L, -1);
    lua_pop(L, 1);

    lua_pushstring(L, "script");
    lua_gettable(L, -2);
    const char *t = lua_tostring(L, -1);
    string script = "./scripts/" + string(t);
    lua_pop(L, 1);

    Vector position, rotation, scale;

    lua_pushstring(L, "position");
    lua_gettable(L, -2);
    loadVector(L, &position);
    lua_pop(L, 1);

    if ( number == 1 ) {
        // object[0] must be player
        objectType = OBJECT_PLAYER;
    }

    // The player will always be created before a level is loaded
    if (player == NULL) {
        PRINT_ERROR("The player object was not initialized prior to loading the level.\n");
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
