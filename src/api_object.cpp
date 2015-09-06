/**
 * @file api_object.cpp
 * @brief Krig Game Engine Lua Scripting API - Game Object
 *
 * Game object focused API functionality.
 */
#include "api.h"
#include "api_object.h"

#if DOXYGEN_ONLY
/**
 * Get the specified object's position.
 * @param GameObjectReference
 * @return Vector - position.
 */
Vector get_position(GameObjectReference);
#endif
static int get_position(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  Object *object = static_cast<Object*>(lua_touserdata(L, 1));

  Vector position = object->getPosition();

  returnVector(L, position);
  return 1;
}

#if DOXYGEN_ONLY
/**
 * Set the specified object's position.
 * @param GameObjectReference
 * @param float x coordinate.
 * @param float y coordinate.
 * @param float z coordinate.
 * @return n/a
 */
void set_position(GameObjectReference, float, float, float);

/**
 * Set the specified object's position.
 * @param GameObjectReference
 * @param Vector
 * @return n/a
 */
void set_position(GameObjectReference, Vector);
#endif
static int set_position(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  Object *object = static_cast<Object*>(lua_touserdata(L, 1));

  object->setPosition(loadVector(L));
  return 0;
}

#if DOXYGEN_ONLY
/**
 * Get the specified object's velocity.
 * @param GameObjectReference
 * @return Vector - velocity.
 */
Vector get_velocity(GameObjectReference);
#endif
static int get_velocity(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  Object *object = static_cast<Object*>(lua_touserdata(L, 1));

  Vector velocity = object->getVelocity();

  returnVector(L, velocity);
  return 1;
}

#if DOXYGEN_ONLY
/**
 * Set the specified object's velocity.
 * @param GameObjectReference
 * @param float x component.
 * @param float y component.
 * @param float z component.
 * @return n/a
 */
void set_velocity(GameObjectReference, float, float, float);

/**
 * Set the specified object's velocity.
 * @param GameObjectReference
 * @param Vector
 * @return n/a
 */
void set_velocity(GameObjectReference, Vector);
#endif

static int set_velocity(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  Object *object = static_cast<Object*>(lua_touserdata(L, 1));

  object->setVelocity(loadVector(L));
  return 0;
}

#if DOXYGEN_ONLY
/**
 * Fetch the specified object's rotational velocity.
 * @param GameObjectReference
 * @return Vector - rotational velocity.
 */
Vector get_rotation_velocity(GameObjectReference);
#endif
static int get_rotation_velocity(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  Object *object = static_cast<Object*>(lua_touserdata(L, 1));

  Vector rotationVelocity = object->getRotationVelocity();

  returnVector(L, rotationVelocity);
  return 1;
}

#if DOXYGEN_ONLY
/**
 * Set the specified object's rotational velocity.
 * @param GameObjectReference
 * @param float rotation velocity x component.
 * @param float rotation velocity y component.
 * @param float rotation velocity z component.
 * @return n/a
 */
void set_rotation_velocity(GameObjectReference, float, float, float);

/**
 * Set the specified object's rotational velocity.
 * @param GameObjectReference
 * @param Vector rotation velocity.
 * @return n/a
 */
void set_rotation_velocity(GameObjectReference, Vector);
#endif
static int set_rotation_velocity(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  Object *object = static_cast<Object*>(lua_touserdata(L, 1));
  object->setRotationVelocity(loadVector(L));
  return 0;
}

#if DOXYGEN_ONLY
/**
 * Set the specified object's speed.
 * @param GameObjectReference
 * @param float speed in direction of the object.
 * @param float speed in up direction of the object.
 * @param float speed in orthoginal direction of the object.
 * @return n/a
 */
void set_speed(GameObjectReference, float, float, float);

/**
 * Set the specified object's speed.
 * @param GameObjectReference
 * @param Vector speed.
 * @return n/a
 */
void set_speed(GameObjectReference, Vector);
#endif
static int set_speed(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  Object *object = static_cast<Object*>(lua_touserdata(L, 1));

  object->setSpeed(loadVector(L));
  return 0;
}

#if DOXYGEN_ONLY
/**
 * Fetch the specified object's speed.
 * @param GameObjectReference
 * @return Vector - speed.
 */
Vector get_speed(GameObjectReference);
#endif
static int get_speed(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  Object *object = static_cast<Object*>(lua_touserdata(L, 1));

  Vector speed = object->getSpeed();

  returnVector(L, speed);
  return 1;
}

#if DOXYGEN_ONLY
/**
 * Set the specified object's rotation.
 * @param GameObjectReference
 * @param float x rotation component.
 * @param float y rotation component.
 * @param float z rotation component.
 * @return n/a
 */
void set_rotation(GameObjectReference, float, float, float);

/**
 * Set the specified object's rotation.
 * @param GameObjectReference
 * @param Vector rotation.
 * @return n/a
 */
void set_rotation(GameObjectReference, Vector);
#endif
static int set_rotation(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  Object *object = static_cast<Object*>(lua_touserdata(L, 1));
  object->setRotationEuler(loadVector(L));
  object->setRotationChanged(true);
  return 0;
}

#if DOXYGEN_ONLY
/**
 * Fetch the specified object's rotation.
 * @param GameObjectReference
 * @return Vector - rotation.
 */
Vector get_rotation(GameObjectReference);
#endif
static int get_rotation(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  Object *object = static_cast<Object*>(lua_touserdata(L, 1));

  Vector tv;

  object->getRotation().getEulerAngles(tv);

  returnVector(L, tv);
  return 1;
}

#if DOXYGEN_ONLY
/**
 * Get the specified object's direction.
 * @param GameObjectReference
 * @return Vector - direction.
 */
Vector get_direction(GameObjectReference);
#endif
static int get_direction(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  Object *object = static_cast<Object*>(lua_touserdata(L, 1));

  Vector direction = object->getDirection();

  returnVector(L, direction);
  return 1;
}

#if DOXYGEN_ONLY
/**
 * Get the specified object's up vector.
 * @param GameObjectReference
 * @return Vector - up vector.
 */
Vector get_up(GameObjectReference);
#endif
static int get_up(lua_State *L)
{
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  Object *object = static_cast<Object*>(lua_touserdata(L, 1));

  Vector up = object->getUp();

  returnVector(L, up);
  return 1;
}

#if DOXYGEN_ONLY
/**
 * Get the specified object's orthogonal vector (orthogonal to up and direction vectors).
 * @param GameObjectReference
 * @return Vector - orthogonal vector.
 */
Vector get_orthogonal(GameObjectReference);
#endif
static int get_orthogonal(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  Object *object = static_cast<Object*>(lua_touserdata(L, 1));

  Vector orth = object->getOrth();

  returnVector(L, orth);
  return 1;
}

#if DOXYGEN_ONLY
/**
 * Add to the specified object's position.
 * @param GameObjectReference
 * @param float x coordinate.
 * @param float y coordinate.
 * @param float z coordinate.
 * @return n/a
 */
void add_position(GameObjectReference, float, float, float);

/**
 * Add to the specified object's position.
 * @param GameObjectReference
 * @param Vector coordinates.
 * @return n/a
 */
void add_position(GameObjectReference, Vector);
#endif
static int add_position(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  Object *object = static_cast<Object*>(lua_touserdata(L, 1));

  Vector position = object->getPosition();

  position += Vector(loadVector(L));

  object->setPosition(position);
  return 0;
}

#if DOXYGEN_ONLY
/**
 * Add to the specified object's rotation.
 * @param GameObjectReference
 * @param float x rotation component.
 * @param float y rotation component.
 * @param float z rotation component.
 * @return n/a
 */
void add_rotation(GameObjectReference, float, float, float);

/**
 * Add to the specified object's rotation.
 * @param GameObjectReference
 * @param Vector rotation.
 * @return n/a
 */
void add_rotation(GameObjectReference, Vector);
#endif
static int add_rotation(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  Object *object = static_cast<Object*>(lua_touserdata(L, 1));

  Quaternion qt;
  qt.buildFromEuler(loadVector(L));
  object->setRotation(object->getRotation() * qt);

  return 0;
}

#if DOXYGEN_ONLY
/**
 * Add to the specified object's velocity.
 * @param GameObjectReference
 * @param float x component.
 * @param float y component.
 * @param float z component.
 * @return n/a
 */
void add_velocity(GameObjectReference, float, float, float);

/**
 * Add to the specified object's velocity.
 * @param GameObjectReference
 * @param Vector
 * @return n/a
 */
void add_velocityv(GameObjectReference, Vector);
#endif
static int add_velocity(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  Object *object = static_cast<Object*>(lua_touserdata(L, 1));

  Vector velocity = object->getVelocity();

  velocity += Vector(loadVector(L));

  object->setVelocity(velocity);
  return 0;
}

#if DOXYGEN_ONLY
/**
 * Add to the specified object's speed.
 * @param GameObjectReference
 * @param float speed in direction of the object.
 * @param float speed in up direction of the object.
 * @param float speed in orthoginal direction of the object.
 * @return n/a
 */
void add_speed(GameObjectReference, float, float, float);

/**
 * Add to the specified object's speed.
 * @param GameObjectReference
 * @param Vector speed.
 * @return n/a
 */
void add_speed(GameObjectReference, Vector);
#endif
static int add_speed(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  Object *object = static_cast<Object*>(lua_touserdata(L, 1));

  Vector speed = object->getSpeed();

  speed += Vector(loadVector(L));

  object->setSpeed(speed);

  return 0;
}

#if DOXYGEN_ONLY
/**
 * Add to the specified object's rotational velocity.
 * @param GameObjectReference
 * @param float rotation velocity x component.
 * @param float rotation velocity y component.
 * @param float rotation velocity z component.
 * @return n/a
 */
void add_rotation_velocity(GameObjectReference, float, float, float);

/**
 * Add to the specified object's rotational velocity.
 * @param GameObjectReference
 * @param Vector rotation velocity.
 * @return n/a
 */
void add_rotation_velocity(GameObjectReference, Vector);
#endif
static int add_rotation_velocity(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  Object *object = static_cast<Object*>(lua_touserdata(L, 1));

  Vector rotationVelocity = object->getRotationVelocity();

  rotationVelocity += Vector(loadVector(L));

  object->setRotationVelocity(rotationVelocity);

  return 0;
}

#if DOXYGEN_ONLY
/**
 * Add to the scale for the specified object.
 * @param GameObjectReference
 * @param x scale factor.
 * @param y scale factor.
 * @param z scale factor.
 * @return n/a
 */
void add_scale(GameObjectReference, float, float, float);

/**
 * Add to the scale for the specified object.
 * @param GameObjectReference
 * @param Vector scale factor.
 * @return n/a
 */
void add_scale(GameObjectReference, Vector);
#endif
static int add_scale(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  Object *object = static_cast<Object*>(lua_touserdata(L, 1));

  Vector scale = object->getScale();

  scale += Vector(loadVector(L));

  object->setScale(scale);

  return 0;
}

#if DOXYGEN_ONLY
/**
 * Add to the rate that the specified game object should scale each game cycle.
 * @param GameObjectReference
 * @param x scale rate.
 * @param y scale rate.
 * @param z scale rate.
 * @return n/a
 */
void add_scale_rate(GameObjectReference, float, float, float);

/**
 * Add to the rate that the specified game object should scale each game cycle.
 * @param GameObjectReference
 * @param Vector scale rate.
 * @return n/a
 */
void add_scale_rate(GameObjectReference, Vector);
#endif
static int add_scale_rate(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);

  Object *object = static_cast<Object*>(lua_touserdata(L, 1));

  Vector scaleRate = object->getScaleRate();

  scaleRate += Vector(loadVector(L));

  object->setScaleRate(scaleRate);

  return 0;
}

#if DOXYGEN_ONLY
/**
 * Set the end rotation (by axis) for the specified game object's interpolation.
 * @param GameObjectReference
 * @param float x component.
 * @param float y component.
 * @param float z component.
 * @param float degrees.
 * @return n/a
 */
void set_interpolation_rotation_end_axis(GameObjectReference, float, float, float, float);

/**
 * Set the end rotation (by axis) for the specified game object's interpolation.
 * @param GameObjectReference
 * @param Vector axis.
 * @param float degrees.
 * @return n/a
 */
void set_interpolation_rotation_end_axis(GameObjectReference, Vector, float);
#endif
static int set_interpolation_rotation_end_axis(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  Object *object = static_cast<Object*>(lua_touserdata(L, 1));

  int index = 2;
  Vector tv = loadVector(L, index);
  tv.normalize();

  Quaternion rInterpEnd = object->getRInterpEnd();

  rInterpEnd.buildFromAxis(tv, lua_tonumber(L,index));

  object->setRInterpEnd(rInterpEnd);

  return 0;
}

#if DOXYGEN_ONLY
/**
 * Set the beginning rotation (by axis) for the specified game object's interpolation.
 * @param GameObjectReference
 * @param float x component.
 * @param float y component.
 * @param float z component.
 * @param float degrees.
 * @return n/a
 */
void set_interpolation_rotation_start_axis(GameObjectReference, float, float, float, float);

/**
 * Set the beginning rotation (by axis) for the specified game object's interpolation.
 * @param GameObjectReference
 * @param Vector axis.
 * @param float degrees.
 * @return n/a
 */
void set_interpolation_rotation_start_axis(GameObjectReference, Vector, float);
#endif
static int set_interpolation_rotation_start_axis(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  Object *object = static_cast<Object*>(lua_touserdata(L, 1));

  Quaternion rInterpStart = object->getRInterpStart();

  int index = 2;
  Vector tv = loadVector(L, index);
  tv.normalize();

  rInterpStart.buildFromAxis(tv, lua_tonumber(L, index));

  object->setRInterpStart(rInterpStart);

  return 0;
}

#if DOXYGEN_ONLY
/**
 * Set the beginning rotation for the specified game object's interpolation.
 * @param GameObjectReference
 * @param float x coordinate.
 * @param float y coordinate.
 * @param float z coordinate.
 * @return n/a
 */
void set_interpolation_rotation_start(GameObjectReference, float, float, float);

/**
 * Set the beginning rotation for the specified game object's interpolation.
 * @param GameObjectReference
 * @param Vector coordinates.
 * @return n/a
 */
void set_interpolation_rotation_start(GameObjectReference, Vector);
#endif
static int set_interpolation_rotation_start(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  Object *object = static_cast<Object*>(lua_touserdata(L, 1));

  Quaternion rInterpStart = object->getRInterpStart();

  rInterpStart.buildFromEuler(loadVector(L));

  object->setRInterpStart(rInterpStart);

  return 0;
}

#if DOXYGEN_ONLY
/**
 * Set the end rotation for the specified game object's interpolation.
 * @param GameObjectReference
 * @param float x coordinate.
 * @param float y coordinate.
 * @param float z coordinate.
 * @return n/a
 */
void set_interpolation_rotation_end(GameObjectReference, float, float, float);

/**
 * Set the end rotation for the specified game object's interpolation.
 * @param GameObjectReference
 * @param Vector coordinates.
 * @return n/a
 */
void set_interpolation_rotation_end(GameObjectReference, Vector);
#endif
static int set_interpolation_rotation_end(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  Object *object = static_cast<Object*>(lua_touserdata(L, 1));

  Quaternion rInterpEnd = object->getRInterpEnd();

  rInterpEnd.buildFromEuler(loadVector(L));

  object->setRInterpEnd(rInterpEnd);

  return 0;
}

#if DOXYGEN_ONLY
/**
 * Enable/Disable interpolation for the specified object.
 * @param GameObjectReference
 * @param bool enable/disable.
 * @return n/a
 */
void set_interpolation_enable(GameObjectReference, bool);
#endif
static int set_interpolation_enable(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  Object *object = static_cast<Object*>(lua_touserdata(L, 1));

  // TODO: there's probably a better way to do this...
  int op = (int)lua_tonumber(L,2);

  if (op == 0)
    object->setIsInterpolationEnabled(false);
  else
    object->setIsInterpolationEnabled(true);

  return (0);
}

#if DOXYGEN_ONLY
/**
 * Set beginning value of interpolation variable's value range.
 * @param GameObjectReference
 * @param float value.
 * @return n/a
 */
void set_interpolation_variable_begin_value(GameObjectReference, float);
#endif
static int set_interpolation_variable_begin_value(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  Object *object = static_cast<Object*>(lua_touserdata(L, 1));
  object->setValInterpBegin(lua_tonumber(L, 2));
  return 0;
}

#if DOXYGEN_ONLY
/**
 * Set end value of interpolation variable's value range.
 * @param GameObjectReference
 * @param float value.
 * @return n/a
 */
void set_interpolation_variable_end_value(GameObjectReference, float);
#endif
static int set_interpolation_variable_end_value(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  Object *object = static_cast<Object*>(lua_touserdata(L, 1));
  object->setValInterpEnd(lua_tonumber(L, 2));
  return 0;
}

#if DOXYGEN_ONLY
/**
 * Set current value of interpolation variable's value range.
 * @param GameObjectReference
 * @param float value.
 * @return n/a
 */
void set_interpolation_variable_current_value(GameObjectReference, float);
#endif
static int set_interpolation_variable_current_value(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  Object *object = static_cast<Object*>(lua_touserdata(L, 1));
  object->setValInterpCurrent(lua_tonumber(L, 2));
  return 0;
}

#if DOXYGEN_ONLY
/**
 * Set the specified object's rotational velocity from an axis and angle.
 * @param GameObjectReference
 * @param float x rotation velocity component.
 * @param float y rotation velocity component.
 * @param float z rotation velocity component.
 * @param float angle.
 * @return n/a
 */
void set_rotation_velocity_axis(GameObjectReference, float, float, float, float);

/**
 * Set the specified object's rotational velocity.
 * @param GameObjectReference
 * @param Vector rotation velocity.
 * @return n/a
 */
void set_rotation_velocity_axis(GameObjectReference, Vector, float);
#endif
static int set_rotation_velocity_axis(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  Object *object = static_cast<Object*>(lua_touserdata(L, 1));

  Vector rotationVelocity = object->getRotationVelocity();

  int index = 2;
  rotationVelocity = loadVector(L, index);
  rotationVelocity.normalize();
  rotationVelocity.scale(lua_tonumber(L, index));

  object->setRotationVelocity(rotationVelocity);

  return 0;
}

#if DOXYGEN_ONLY
/**
 * Set the specified object's rotation.
 * @param GameObjectReference
 * @param float x rotation component.
 * @param float y rotation component.
 * @param float z rotation component.
 * @param float angle.
 * @return n/a
 */
void set_rotation_axis(GameObjectReference, float, float, float, float);

/**
 * Set the specified object's rotation.
 * @param GameObjectReference
 * @param Vector rotation axis.
 * @param float angle.
 * @return n/a
 */
void set_rotation_axis(GameObjectReference, Vector, float);
#endif
static int set_rotation_axis(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  Object *object = static_cast<Object*>(lua_touserdata(L, 1));

  int index = 2;
  Vector tv = loadVector(L, index);

  Quaternion rotation;

  rotation.buildFromAxis(tv, lua_tonumber(L, index));

  object->setRotationChanged(true);

  object->setRotation(rotation);

  return 0;
}

#if DOXYGEN_ONLY
/**
 * Add to the specified object's rotation.
 * @param GameObjectReference
 * @param float x rotation component.
 * @param float y rotation component.
 * @param float z rotation component.
 * @param float angle.
 * @return n/a
 */
void add_rotation_axis(GameObjectReference, float, float, float, float);

/**
 * Add to the specified object's rotation.
 * @param GameObjectReference
 * @param Vector rotation axis.
 * @param float angle.
 * @return n/a
 */
void add_rotation_axis(GameObjectReference, Vector, float);
#endif
static int add_rotation_axis(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  Object *object = static_cast<Object*>(lua_touserdata(L, 1));

  int index = 2;
  Quaternion tq;
  Vector tv = loadVector(L, index);
  tv.normalize();
  tq.buildFromAxis(tv, lua_tonumber(L, index));

  Quaternion rotation = object->getRotation();

  rotation = rotation * tq;

  object->setRotation(rotation);

  return 0;
}

#if DOXYGEN_ONLY
/**
 * Fetch the specified game object's timer.
 * @param GameObjectReference
 * @return float
 */
float get_timer(GameObjectReference, float);
#endif
static int get_timer(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  Object *object = static_cast<Object*>(lua_touserdata(L, 1));

  // TODO: Is this logic working properly?
  lua_pushnumber(L, 0);
  return 1;
}

#if DOXYGEN_ONLY
/**
 * Suspend running the specified game object's script until the suspend time has elapsed.
 * @param GameObjectReference
 * @param float - path to wav file to play.
 * @return n/a
 */
void suspend(GameObjectReference, float);
#endif
static int suspend(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  Object *object = static_cast<Object*>(lua_touserdata(L, 1));
  object->setSuspendTime(lua_tonumber(L, 2));
  return 0;
}

#if DOXYGEN_ONLY
/**
 * Specify the model to render for this game object.
 * @param GameObjectReference
 * @string file containing the model to load.
 * @return n/a
 */
void set_model(GameObjectReference, string);
#endif
static int set_model(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  ScriptedObject *object = static_cast<ScriptedObject*>(lua_touserdata(L, 1));

  const char *s = lua_tostring(L, 2);
  string model = string(s);

  object->load(model);
  return 0;
}

#if DOXYGEN_ONLY
/**
 * Set the scale for the specified object.
 * @param GameObjectReference
 * @param x scale factor.
 * @param y scale factor.
 * @param z scale factor.
 * @return n/a
 */
void set_scale(GameObjectReference, float, float, float);

/**
 * Set the scale for the specified object.
 * @param GameObjectReference
 * @param Vector scale factor.
 * @return n/a
 */
void set_scale(GameObjectReference, Vector);
#endif
static int set_scale(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  Object *object = static_cast<Object*>(lua_touserdata(L, 1));

  object->setScale(loadVector(L));
  object->setScaleChanged(true);
  return 0;
}

#if DOXYGEN_ONLY
/**
 * Set lua script for specified game object.
 * @param GameObjectReference
 * @param string lua script file name.
 * @return n/a
 */
void set_script(GameObjectReference, string);
#endif
static int set_script(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  Object *object = static_cast<Object*>(lua_touserdata(L, 1));

  const char *s = lua_tostring(L, 2);
  string script = string(s);

  object->unloadScript();
  object->setScript(script);
  object->loadScript(script, L);

  return 0;
}

#if DOXYGEN_ONLY
/**
 * Enable the specified game object's "always lit" mode which will always render
 * the object's model in full direct light (regardless of the level's light direction).
 * @param GameObjectReference
 * @return n/a
 */
void enable_always_lit(GameObjectReference);
#endif
static int enable_always_lit(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  Object *object = static_cast<Object*>(lua_touserdata(L, 1));
  object->setIsAlwaysLit(true);
  return 0;
}

#if DOXYGEN_ONLY
/**
 * Disable the specified game object's "always lit" mode which will is the default setting
 * and renders object using the level's light direction
 * @param GameObjectReference
 * @return n/a
 */
void disable_always_lit(GameObjectReference);
#endif
static int disable_always_lit(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  Object *object = static_cast<Object*>(lua_touserdata(L, 1));
  object->setIsAlwaysLit(false);
  return 0;
}

#if DOXYGEN_ONLY
/**
 * Add a particle system to the specified object.
 * @param GameObjectReference
 * @param int - number representing the particle system to load.
 * @return n/a
 */
void add_particle_system(GameObjectReference, int);
#endif
static int add_particle_system(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  Object *object = static_cast<Object*>(lua_touserdata(L, 1));

  object->setParticleSystem((int)lua_tonumber(L, 2));

  return 0;
}

#if DOXYGEN_ONLY
/**
 * Set a type identification number for the specified game object.
 * @param GameObjectReference
 * @param int ID for this game object.
 * @return n/a
 */
void set_type_id(GameObjectReference, int);
#endif
static int set_type_id(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  Object *object = static_cast<Object*>(lua_touserdata(L, 1));
  object->setTypeId((int)lua_tonumber(L, 2));
  return 0;
}

#if DOXYGEN_ONLY
/**
 * Fetch the type identification number for the specified game object.
 * @param GameObjectReference
 * @return int - identification number.
 */
void get_type_id(GameObjectReference);
#endif
static int get_type_id(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  Object *object = static_cast<Object*>(lua_touserdata(L, 1));
  lua_pushnumber(L, object->getTypeId());
  return 1;
}

#if DOXYGEN_ONLY
/**
 * Set the rate that the specified game object should scale each game cycle.
 * @param GameObjectReference
 * @param x scale rate.
 * @param y scale rate.
 * @param z scale rate.
 * @return n/a
 */
void set_scale_rate(GameObjectReference, float, float, float);

/**
 * Set the rate that the specified game object should scale each game cycle.
 * @param GameObjectReference
 * @param Vector scale rate.
 * @return n/a
 */
void set_scale_rate(GameObjectReference, Vector);
#endif
static int set_scale_rate(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  Object *object = static_cast<Object*>(lua_touserdata(L, 1));

  Vector scaleRate = object->getScaleRate();

  scaleRate = loadVector(L);

  object->setScaleRate(scaleRate);

  return 0;
}

#if DOXYGEN_ONLY
/**
 * Fetch the scale for the specified game object.
 * @param GameObjectReference
 * @return Vector - game object's scale factor.
 */
Vector get_scale(GameObjectReference);
#endif
static int get_scale(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  Object *object = static_cast<Object*>(lua_touserdata(L, 1));

  Vector scale = object->getScale();

  returnVector(L, scale);
  return 1;
}

#if DOXYGEN_ONLY
/**
 * Fetch the scale rate for the specified game object.
 * @param GameObjectReference
 * @return Vector - game object's scale rate.
 */
Vector get_scale_rate(GameObjectReference);
#endif
static int get_scale_rate(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  Object *object = static_cast<Object*>(lua_touserdata(L, 1));

  Vector scaleRate = object->getScaleRate();

  returnVector(L, scaleRate);
  return 1;
}

#if DOXYGEN_ONLY
/**
 * Enable collision detection between the specified object and other game objects.
 * @param GameObjectReference
 * @return n/a
 */
void enable_collision_detection(GameObjectReference);
#endif
static int enable_collision_detection(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  Object *object = static_cast<Object*>(lua_touserdata(L, 1));
  object->setCollisionDetectionEnabled(true);
  return 0;
}

#if DOXYGEN_ONLY
/**
 * Disable collision detection between the specified object and all other game objects.
 * @param GameObjectReference
 * @return n/a
 */
void disable_collision_detection(GameObjectReference);
#endif
static int disable_collision_detection(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  Object *object = static_cast<Object*>(lua_touserdata(L, 1));
  object->setCollisionDetectionEnabled(false);
  return 0;
}

#if DOXYGEN_ONLY
/**
 * Query the game engine to determine if collision detection has been enabled
 * for the specified game object.
 * @param GameObjectReference
 * @return bool
 */
void get_collision_detection_enabled(GameObjectReference);
#endif
static int get_collision_detection_enabled(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  Object *object = static_cast<Object*>(lua_touserdata(L, 1));
  lua_pushnumber(L, object->getCollisionDetectionEnabled());
  return 1;
}

#if DOXYGEN_ONLY
/**
 * Set the active state of the specified object to true (active).
 * @param GameObjectReference game object reference obtained from the engine
 *        through another API call.
 * @return n/a
 */
void set_active(GameObjectReference);
#endif
static int set_active(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  Object *object = static_cast<Object*>(lua_touserdata(L, 1));
  object->setActive(true);
  return 0;
}

#if DOXYGEN_ONLY
/**
 * Set the active state of the specified object to false (inactive).
 * @param GameObjectReference game object reference obtained from the engine
 *        through another API call.
 * @return n/a
 */
void set_inactive(GameObjectReference);
#endif
static int set_inactive(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  Object *object = static_cast<Object*>(lua_touserdata(L, 1));
  object->setActive(false);
  return 0;
}

#if DOXYGEN_ONLY
/**
 * Obtain the active/inactive state of the specified object.
 * @param GameObjectReference game object reference obtained from the engine
 *        through another API call.
 * @return bool - true = active, false = inactive
 */
bool get_active(GameObjectReference);
#endif
static int get_active(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  Object *object = static_cast<Object*>(lua_touserdata(L, 1));
  lua_pushnumber(L, object->getActive());
  return 1;
}

#if DOXYGEN_ONLY
/**
 * Designate the specified game object to be drawn by the engine each game cycle.
 * @param GameObjectReference
 * @return n/a
 */
void enable_draw(GameObjectReference);
#endif
static int enable_draw(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  Object *object = static_cast<Object*>(lua_touserdata(L, 1));
  object->setDrawEnabled(true);
  return 0;
}

#if DOXYGEN_ONLY
/**
 * Designate the specified game object not to be drawn by the engine each game cycle.
 * @param GameObjectReference
 * @return n/a
 */
void disable_draw(GameObjectReference);
#endif
static int disable_draw(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  Object *object = static_cast<Object*>(lua_touserdata(L, 1));
  object->setDrawEnabled(false);
  return 0;
}

#if DOXYGEN_ONLY
/**
 * Query the engine to determine if the specified game object will be drawn each game cycle.
 * @param GameObjectReference
 * @return bool - whether or not the object will be drawn
 */
void get_draw_enabled(GameObjectReference);
#endif
static int get_draw_enabled(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  Object *object = static_cast<Object*>(lua_touserdata(L, 1));
  lua_pushnumber(L, object->getDrawEnabled());
  return 1;
}

#if DOXYGEN_ONLY
/**
 * Fetch the radius of the specified game object's bounding sphere.
 * @param GameObjectReference
 * @return float - radius
 */
float get_bounding_sphere_radius(GameObjectReference);
#endif
static int get_bounding_sphere_radius(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  Object *object = static_cast<Object*>(lua_touserdata(L, 1));
  lua_pushnumber(L, object->getBoundingSphere().getRadius());
  return 1;
}

#if DOXYGEN_ONLY
/**
 * Determine whether the specified object is in the current camera's view.
 * @param GameObjectReference
 * @return bool - whether or not the object is in the camera's view
 */
float get_in_view(GameObjectReference);
#endif
static int get_in_view(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  Object *object = static_cast<Object*>(lua_touserdata(L, 1));
  lua_pushnumber(L, object->getInView());
  return 1;
}

#if DOXYGEN_ONLY
/**
 * Rotate the specified object such that it appears to be resting on the terrain.
 * Additionally, the specified rotation will be used as the base rotation of the game object.
 * @param GameObjectReference
 * @param x rotation around x axis
 * @param y rotation around y axis
 * @param z rotation around z axis
 * @return n/a
 */
void orient_on_terrain(GameObjectReference, float, float, float);
#endif
static int orient_on_terrain(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  Model *object = static_cast<Model*>(lua_touserdata(L, 1));

  Quaternion rotation;
  rotation.buildFromEuler(lua_tonumber(L,2),lua_tonumber(L,3),lua_tonumber(L,4));

  object->orientOnTerrain(g_KRIG_ENGINE->getCurrentLevel()->getTerrain(), rotation);

  return 0;
}

#if DOXYGEN_ONLY
/**
 * Determine the height of the terrain beneath the specified game object and
 * set its height to match the terrain's height at this location. The offset
 * is used to adjust the object's height along the y axis.
 * @param GameObjectReference
 * @param float - height offset
 * @return n/a
 */
void set_height_from_terrain(GameObjectReference, float);
#endif
static int set_height_from_terrain(lua_State *L) {
  luaL_checktype(L, 1, LUA_TLIGHTUSERDATA);
  Model *object = static_cast<Model*>(lua_touserdata(L, 1));

  float offset = lua_tonumber(L, 2);

  object->setHeightFromTerrain(g_KRIG_ENGINE->getCurrentLevel()->getTerrain(), offset);

  return 0;
}

const luaL_reg krigObjectLib[] = {
  {"get_position", get_position},
  {"set_position", set_position},
  {"get_velocity", get_velocity},
  {"set_velocity", set_velocity},
  {"get_rotation", get_rotation},
  {"set_rotation", set_rotation},
  {"set_rotation_axis", set_rotation_axis},
  {"get_rotation_velocity", get_rotation_velocity},
  {"set_rotation_velocity", set_rotation_velocity},
  {"get_speed", get_speed},
  {"set_speed", set_speed},
  {"get_direction", get_direction},
  {"get_up", get_up},
  {"get_orthogonal", get_orthogonal},
  {"add_position", add_position},
  {"add_rotation", add_rotation},
  {"add_rotation_axis", add_rotation_axis},
  {"add_velocity", add_velocity},
  {"add_speed", add_speed},
  {"add_scale", add_scale},
  {"add_scale_rate", add_scale_rate},
  {"set_interpolation_rotation_start_axis", set_interpolation_rotation_start_axis},
  {"set_interpolation_rotation_end_axis", set_interpolation_rotation_end_axis},
  {"set_interpolation_rotation_start", set_interpolation_rotation_start},
  {"set_interpolation_rotation_end", set_interpolation_rotation_end},
  {"set_interpolation_enable", set_interpolation_enable},
  {"set_interpolation_variable_begin_value", set_interpolation_variable_begin_value},
  {"set_interpolation_variable_end_value", set_interpolation_variable_end_value},
  {"set_interpolation_variable_current_value", set_interpolation_variable_current_value},
  {"set_rotation_velocity_axis", set_rotation_velocity_axis},
  {"get_timer", get_timer},
  {"suspend", suspend},
  {"set_model", set_model},
  {"set_script", set_script},
  {"add_particle_system", add_particle_system},
  {"get_scale", get_scale},
  {"set_scale", set_scale},
  {"get_scale_rate", get_scale_rate},
  {"set_scale_rate", set_scale_rate},
  {"get_type_id", get_type_id},
  {"set_type_id", set_type_id},
  {"get_collision_detection_enabled", get_collision_detection_enabled},
  {"enable_collision_detection", enable_collision_detection},
  {"disable_collision_detection", disable_collision_detection},
  {"get_active", get_active},
  {"set_active", set_active},
  {"set_inactive", set_inactive},
  {"get_draw_enabled", get_draw_enabled},
  {"enable_draw", enable_draw},
  {"disable_draw", disable_draw},
  {"get_bounding_sphere_radius", get_bounding_sphere_radius},
  {"get_in_view", get_in_view},
  {"enable_always_lit", enable_always_lit},
  {"disable_always_lit", disable_always_lit},
  {"orient_on_terrain", orient_on_terrain},
  {"set_height_from_terrain", set_height_from_terrain},
  {NULL, NULL}
};

int luaopen_krigObject (lua_State *L) {
  luaL_openlib(L, "krig.object", krigObjectLib, 0);
  return 1;
}
