//////////////////////////////////////////////////////////////////
// Description : Functions for displaying models and storing    //
//               their transformation data                      //
//////////////////////////////////////////////////////////////////
#include <cstdlib>

#include "Object.h"
#include "api.h"
#include "Snow.h"
#include "StarField.h"

GLint numObjects;
Object* object;

unsigned int Object::textureIds[MAX_TEXTURES];
std::map <std::string, unsigned int> Object::textureHash;

//------------------------------------------------------------------------------
Object::Object() : ObjectNode() {
  scriptName_     = "";
  particleSystem_ = NULL;
  L_              = NULL;

  init();
}

//------------------------------------------------------------------------------
Object::~Object() {
  cleanup();
}

//------------------------------------------------------------------------------
void Object::cleanup() {
  if (L_)
    lua_close(L_);

  if (particleSystem_)
    delete particleSystem_;

  L_ = NULL;
  particleSystem_ = NULL;

  scriptIndex_ = -1;
  gameLevelId_ = -1;
}

//------------------------------------------------------------------------------
void Object::init() {
  cleanup();
  initSettings();
}

//------------------------------------------------------------------------------
void Object::initSettings() {
  velocity_.setVector(0.0f, 0.0f, 0.0f);
  speed_.setVector(0.0f, 0.0f, 0.0f);
  scaleRate_.setVector(0.0f, 0.0f, 0.0f);
  rotationVelocity_.setVector(0.0f, 0.0f, 0.0f);

  setRotationEuler(0.0f, 0.0f, 0.0f);
  position_.setVector(0.0f, 0.0f, 0.0f);
  scale_.setVector(1.0f, 1.0f, 1.0f);

  //rotation_.loadMultIdentity();
  baseDirection_.setVector(0.0f, 0.0f, 1.0f);
  direction_.setVector(0.0f, 0.0f, 1.0f);
  up_.setVector(0.0f, 1.0f, 0.0f);
  orth_.setVector(1.0f, 0.0f, 0.0f);

  // set default orientation and disable interpolation
  rInterpStart_.loadMultIdentity();
  rInterpEnd_.loadMultIdentity();

  valInterpBegin_ = valInterpCurrent_ = valInterpEnd_ = 0.0f;
  isInterpolationEnabled_ = false;

  // setup default collision detection
  boundingSphere_.setSphere(0.0f, 0.0f, 0.0f, 0.0f);
  collisionBox_[0].setVector(0.0f, 0.0f, 0.0f);
  collisionBox_[1].setVector(0.0f, 0.0f, 0.0f);
  isCollisionDetectionEnabled_ = true;

  state_ = NORMAL;
  active_ = true;
  suspendTime_ = 0.0f;
  typeId_ = -1;

  isInView_ = true;
  isDrawEnabled_ = true;

  lastLight_.setVector(0.0f, 0.0f, 0.0f);
  scaleChanged_ = true;
  rotationChanged_ = true;

  isAlwaysLit_ = false;

  enableSphereTest_ = true;
}

//------------------------------------------------------------------------------
void Object::setScript(const std::string &name) {
  scriptName_ = name;

  // If the lua state has not been initialized for this object, attempt to
  // initialize it.
  if (scriptName_ == "" || L_)
    return;

  L_ = lua_open();

  // If the lua state still could not be initialized, then exit the game.
  // ... we can do something smarter with this in the finished product.
  if (!L_) {
    PRINT_ERROR("Could not create Lua state.\n");
    exit(-1);
  }

  // Load the base lua libraries
  luaL_openlibs(L_);

  // Load our library
  luaopen_krigApi(L_);

  luaL_loadfile(L_, name.c_str());
  scriptIndex_ = luaL_ref( L_, LUA_REGISTRYINDEX );
}

//------------------------------------------------------------------------------
void Object::loadScript(const std::string &name, lua_State* luaState) {
  if (scriptIndex_ == -1)
    return;

  lua_rawgeti(L_, LUA_REGISTRYINDEX, scriptIndex_);
  lua_call(L_, 0, 0);

  // Find the update function and call it
  lua_getglobal(L_, SCRIPT_CALLBACK_ON_LOAD);

  // Push a pointer to the current object for use within the lua function
  if (lua_isfunction (L_, -1)) {
    returnObject(L_, this);
    lua_newtable(L_);

    if (lua_istable(luaState, -1)) {
      traverseAndCopyLuaTable(luaState, L_, -2);
    }

    // Call the function with two arguments and no return values
    lua_call(L_, 2, 0);
  }
  else {
    PRINT_DEBUG_LVL(4, "'%s' function not defined.\n", SCRIPT_CALLBACK_ON_LOAD);
    lua_pop(L_, 1);
  }
}

//------------------------------------------------------------------------------
void Object::unloadScript() {
  // Attempt to execute the on_unload function only if the lua state has
  // already been initialized with a script
  if (!L_)
    return;

  // Find the update function and call it
  lua_getglobal(L_, SCRIPT_CALLBACK_ON_UNLOAD);

  // Push a pointer to the current object for use within the lua function
  if (lua_isfunction(L_, -1)) {
    returnObject(L_, this);
    lua_call(L_, 1, 0);
  }
  else {
    PRINT_DEBUG_LVL(4, "'%s' function not defined.\n", SCRIPT_CALLBACK_ON_UNLOAD);
    lua_pop(L_, 1);
  }

  lua_close(L_);
  L_ = NULL;
}

//------------------------------------------------------------------------------
void Object::animateScript(const float &elapsedTime) {
  // Attempt to execute the script only if the lua state has already been
  // initialized with a script
  if (!L_)
    return;

  // If the script is suspended, do not run until time has elapsed
  if (suspendTime_ > 0.0f) {
    suspendTime_ -= elapsedTime;
    if (suspendTime_ > 0.0f)
      return;
    suspendTime_ = 0.0f;
  }

  // Find the update function and call it
  lua_getglobal(L_, SCRIPT_CALLBACK_ON_UPDATE);

  if (lua_isfunction(L_, -1)) {
    // Push a pointer to the current object for use within the lua function
    returnObject(L_, this);

    // Push the time passed since the last iteration of the game loop
    lua_pushnumber(L_, elapsedTime);

    lua_call(L_, 2, 0);
  }
  else {
    PRINT_DEBUG_LVL(4, "'%s' function not defined.\n", SCRIPT_CALLBACK_ON_UPDATE);
    lua_pop(L_, 1);
  }
}

//------------------------------------------------------------------------------
void Object::processCollisions(Object* temp) {
  if (this != temp && active_ && temp->active_ && state_ == NORMAL &&
      temp->state_ == NORMAL && isCollisionDetectionEnabled_ &&
      temp->isCollisionDetectionEnabled_ && this != NULL && temp != NULL) {

    // First perform a sphere-based collision
    float radius1 = boundingSphere_.getRadius();
    float radius2 = boundingSphere_.getRadius();
    float radius_sum = radius1 + radius2;
    radius_sum = radius_sum * radius_sum;

    float distance =
      (position_.x - temp->position_.x) * ( position_.x - temp->position_.x) +
      (position_.y - temp->position_.y) * ( position_.y - temp->position_.y) +
      (position_.z - temp->position_.z) * ( position_.z - temp->position_.z);

    if (!enableSphereTest_ || !temp->getEnableSphereTest() || radius_sum >= distance) {
      Vector boxA[2];
      Vector boxB[2];

      boxA[0].setVector(collisionBox_[0].x + position_.x, collisionBox_[0].y + position_.y, collisionBox_[0].z + position_.z);
      boxA[1].setVector(collisionBox_[1].x + position_.x, collisionBox_[1].y + position_.y, collisionBox_[1].z + position_.z);
      boxB[0].setVector(temp->collisionBox_[0].x + temp->position_.x, temp->collisionBox_[0].y + temp->position_.y, temp->collisionBox_[0].z + temp->position_.z);
      boxB[1].setVector(temp->collisionBox_[1].x + temp->position_.x, temp->collisionBox_[1].y + temp->position_.y, temp->collisionBox_[1].z + temp->position_.z);

      if(((boxA[0].x >= boxB[0].x && boxA[0].x <= boxB[1].x)  ||
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
          (boxB[1].z >= boxA[0].z && boxB[1].z <= boxA[1].z)) ) {

        PRINT_DEBUG_LVL(3, "Collision! object 1=(%d), object 2=(%d).\n", this->typeId_, temp->typeId_);
        handleCollision(temp);
        temp->handleCollision(this);
      }
    }
  }

  if (temp->next != NULL)
    processCollisions(((Object*)temp->next));
}

//------------------------------------------------------------------------------
float Object::calcTriangleCenter(const float &p1, const float &p2, const float &p3) {
  float th1 = (.5) * p1 + (.5 * p2);
  float finalHeight = (.5) * th1 + (.5 * p3);

  return (finalHeight);
}

//------------------------------------------------------------------------------
void Object::showCollisionBox() {
  glDisable(GL_CULL_FACE);
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  glPushMatrix();
  glTranslatef(position_.x, position_.y, position_.z);

  glColor3f(1.0f, 0.0f, 0.0f);

  float radius = boundingSphere_.getRadius();

  float rad = 6.28f / 16.0f;

  glBegin(GL_LINE_LOOP);
  for (int i = 0; i < 16; ++i) {
    glVertex3f((cos(rad * i) * radius), (sin(rad * i) * radius), 0.0f);
  }
  glEnd();

  glBegin(GL_QUADS);

  glVertex3f(collisionBox_[0].x, collisionBox_[1].y, collisionBox_[1].z);
  glVertex3f(collisionBox_[0].x, collisionBox_[0].y, collisionBox_[1].z);
  glVertex3f(collisionBox_[1].x, collisionBox_[0].y, collisionBox_[1].z);
  glVertex3f(collisionBox_[1].x, collisionBox_[1].y, collisionBox_[1].z);

  glVertex3f(collisionBox_[0].x, collisionBox_[1].y, collisionBox_[1].z);
  glVertex3f(collisionBox_[0].x, collisionBox_[1].y, collisionBox_[0].z);
  glVertex3f(collisionBox_[0].x, collisionBox_[0].y, collisionBox_[0].z);
  glVertex3f(collisionBox_[0].x, collisionBox_[0].y, collisionBox_[1].z);

  glVertex3f(collisionBox_[1].x, collisionBox_[1].y, collisionBox_[1].z);
  glVertex3f(collisionBox_[1].x, collisionBox_[0].y, collisionBox_[1].z);
  glVertex3f(collisionBox_[1].x, collisionBox_[0].y, collisionBox_[0].z);
  glVertex3f(collisionBox_[1].x, collisionBox_[1].y, collisionBox_[0].z);

  glVertex3f(collisionBox_[1].x, collisionBox_[1].y, collisionBox_[0].z);
  glVertex3f(collisionBox_[1].x, collisionBox_[0].y, collisionBox_[0].z);
  glVertex3f(collisionBox_[0].x, collisionBox_[0].y, collisionBox_[0].z);
  glVertex3f(collisionBox_[0].x, collisionBox_[1].y, collisionBox_[0].z);

  glVertex3f(collisionBox_[0].x, collisionBox_[1].y, collisionBox_[0].z);
  glVertex3f(collisionBox_[0].x, collisionBox_[1].y, collisionBox_[1].z);
  glVertex3f(collisionBox_[1].x, collisionBox_[1].y, collisionBox_[1].z);
  glVertex3f(collisionBox_[1].x, collisionBox_[1].y, collisionBox_[0].z);

  glVertex3f(collisionBox_[0].x, collisionBox_[0].y, collisionBox_[0].z);
  glVertex3f(collisionBox_[1].x, collisionBox_[0].y, collisionBox_[0].z);
  glVertex3f(collisionBox_[1].x, collisionBox_[0].y, collisionBox_[1].z);
  glVertex3f(collisionBox_[0].x, collisionBox_[0].y, collisionBox_[1].z);

  glEnd();

  glPopMatrix();
  glEnable(GL_CULL_FACE);
  glPolygonMode(GL_FRONT, GL_FILL);

  if (next)
    ((Object*)next)->showCollisionBox();
}

//------------------------------------------------------------------------------
void Object::showControlTriangle() {
  glDisable(GL_CULL_FACE);
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  glPushMatrix();
  glTranslatef(position_.x, position_.y, position_.z);
  glColor3f(1.0f, 0.0f, 0.0f);
  glBegin(GL_TRIANGLES);

  glVertex3f(controlPoints_[0].x, controlPoints_[0].y, controlPoints_[0].z);
  glVertex3f(controlPoints_[1].x, controlPoints_[1].y, controlPoints_[1].z);
  glVertex3f(controlPoints_[2].x, controlPoints_[2].y, controlPoints_[2].z);
  glEnd();

  glPopMatrix();
  glEnable(GL_CULL_FACE);
  glPolygonMode(GL_FRONT, GL_FILL);

  if (next)
    ((Object*)next)->showControlTriangle();
}

//------------------------------------------------------------------------------
void Object::setPosition(const GLfloat &x, const GLfloat &y, const GLfloat &z)
{ position_.setVector(x, y, z); }

//------------------------------------------------------------------------------
void Object::setPosition(const Vector &t)
{ position_ = t; }

//------------------------------------------------------------------------------
void Object::setRotationAxis(const GLfloat &vx, const GLfloat &vy, const GLfloat &vz, const GLfloat &vw)
{ rotation_.buildFromAxis(Vector(vx, vy, vz), vw); }

//------------------------------------------------------------------------------
void Object::setRotationAxis(const Vector &v, const GLfloat &a)
{ rotation_.buildFromAxis(v, a); }

//------------------------------------------------------------------------------
void Object::setRotationEuler(const GLfloat &x, const GLfloat &y, const GLfloat &z)
{ rotation_.buildFromEuler(x, y, z); }

//------------------------------------------------------------------------------
void Object::setRotationEuler(const Vector &v)
{ rotation_.buildFromEuler(v); }

//------------------------------------------------------------------------------
void Object::setRotationQuaternion(const Quaternion &q)
{ rotation_ = q; }

//------------------------------------------------------------------------------
void Object::setRotationVelocity(
    const GLfloat &xAngle, const GLfloat &yAngle, const GLfloat &zAngle
) { rotationVelocity_.setVector(xAngle, yAngle, zAngle); }

//------------------------------------------------------------------------------
void Object::setRotationVelocity(const Vector &v)
{ rotationVelocity_ = v; }

//------------------------------------------------------------------------------
void Object::setVelocity(const GLfloat &x, const GLfloat &y, const GLfloat &z)
{ velocity_.setVector(x, y, z); }

//------------------------------------------------------------------------------
void Object::setVelocity(const Vector &v)
{ velocity_ = v; }

//------------------------------------------------------------------------------
void Object::setSpeed(const GLfloat &x, const GLfloat &y, const GLfloat &z)
{ speed_.setVector(x, y, z); }

//------------------------------------------------------------------------------
void Object::setSpeed(const Vector &v)
{ speed_ = v; }

//------------------------------------------------------------------------------
void Object::setParticleSystem(const int &particleSystemNumber) {
  if (particleSystem_) {
    // We won't allow updating a particle system for now
    return;
  }

  PRINT_DEBUG_LVL(1, "Attempting to load particle system %d\n", particleSystemNumber);

  switch (particleSystemNumber) {
    case 1:
      particleSystem_ = new StarField(this);
      break;
    case 2:
      particleSystem_ = new Snow(this);
      break;
    default:
      PRINT_DEBUG_LVL(1, "Specified particle system was not defined!\n");
      break;
  }
}

//------------------------------------------------------------------------------
void Object::setScale(const GLfloat &x, const GLfloat &y, const GLfloat &z)
{ scale_.setVector(x, y, z); }

//------------------------------------------------------------------------------
void Object::setScale(const Vector &v)
{ scale_ = v; }

//------------------------------------------------------------------------------
void Object::setState(const unsigned char &tState)
{ state_ = tState; }

//------------------------------------------------------------------------------
void Object::setScaleRate(const GLfloat &x, const GLfloat &y, const GLfloat &z)
{ scaleRate_.setVector(x, y, z); }

//------------------------------------------------------------------------------
void Object::setScaleRate(const Vector &v)
{ scaleRate_ = v; }

//------------------------------------------------------------------------------
void Object::traverseAndCopyLuaTable(lua_State* srcState, lua_State* destState, const int &index) {
  lua_pushnil(srcState);

  while (lua_next(srcState, index) != 0) {
    switch(lua_type(srcState, -1)) {
      case LUA_TTABLE:
        copyLuaTableKey(srcState, destState);
        lua_newtable(destState);
        traverseAndCopyLuaTable(srcState, destState, -2);
        lua_rawset(destState, -3);
        break;
      case LUA_TNUMBER:
        copyLuaTableKey(srcState, destState);
        lua_pushnumber(destState, lua_tonumber(srcState, -1));
        lua_rawset(destState, -3);
        break;
      case LUA_TSTRING:
        copyLuaTableKey(srcState, destState);
        lua_pushstring(destState, lua_tostring(srcState, -1));
        lua_rawset(destState, -3);
        break;
      default:
        PRINT_DEBUG_LVL(
          1,
          "Ignoring unsupported type (%s) found on Lua stack while traversing table.\n",
          lua_typename(srcState, lua_type(srcState, -1))
        );
    }

    lua_pop(srcState, 1);
  }
}

void Object::copyLuaTableKey(lua_State* srcState, lua_State* destState) {
  switch (lua_type(srcState, -2)) {
    case LUA_TNUMBER:
      lua_pushnumber(destState, lua_tonumber(srcState, -2));
      break;
    case LUA_TSTRING:
      lua_pushstring(destState, lua_tostring(srcState, -2));
      break;
  }
}

void Object::buildLuaObjectTable(lua_State *L) {
  lua_pushstring(L, "position");
  returnVector(L, getPosition());
  lua_rawset(L, -3);

  lua_pushstring(L, "velocity");
  returnVector(L, getVelocity());
  lua_rawset(L, -3);

  lua_pushstring(L, "rotation");
  returnQuaternion(L, getRotation());
  lua_rawset(L, -3);

  lua_pushstring(L, "rotation_velocity");
  returnVector(L, getRotationVelocity());
  lua_rawset(L, -3);

  lua_pushstring(L, "speed");
  returnVector(L, getSpeed());
  lua_rawset(L, -3);

  lua_pushstring(L, "scale");
  returnVector(L, getScale());
  lua_rawset(L, -3);

  lua_pushstring(L, "scale_rate");
  returnVector(L, getScaleRate());
  lua_rawset(L, -3);

  lua_pushstring(L, "up");
  returnVector(L, getUp());
  lua_rawset(L, -3);

  lua_pushstring(L, "direction");
  returnVector(L, getDirection());
  lua_rawset(L, -3);

  lua_pushstring(L, "orthogonal");
  returnVector(L, getOrth());
  lua_rawset(L, -3);

  lua_pushstring(L, "type_id");
  lua_pushnumber(L, getTypeId());
  lua_rawset(L, -3);

  lua_pushstring(L, "bounding_sphere_radius");
  lua_pushnumber(L, getBoundingSphere().getRadius());
  lua_rawset(L, -3);

  lua_pushstring(L, "in_view");
  lua_pushboolean(L, getInView());
  lua_rawset(L, -3);

  lua_pushstring(L, "collision_detection_enabled");
  lua_pushboolean(L, getCollisionDetectionEnabled());
  lua_rawset(L, -3);

  lua_pushstring(L, "active");
  lua_pushboolean(L, getActive());
  lua_rawset(L, -3);

  lua_pushstring(L, "draw_enabled");
  lua_pushboolean(L, getDrawEnabled());
  lua_rawset(L, -3);

  lua_pushstring(L, "always_lit");
  lua_pushboolean(L, getIsAlwaysLit());
  lua_rawset(L, -3);

  lua_pushstring(L, "interpolation_enabled");
  lua_pushboolean(L, isInterpolationEnabled());
  lua_rawset(L, -3);
}

void Object::transferLuaObjectTable(lua_State *L) {
  Vector tv;
  Quaternion tq;
  int index = -2;

  lua_pushstring(L, "scale");
  lua_gettable(L, 1);
  if (lua_istable(L, -1)) {
    tv = loadVector(L, index);
    setScale(tv);
  }
  lua_pop(L, 1);

  index = -2;
  lua_pushstring(L, "scale_rate");
  lua_gettable(L, 1);
  if (lua_istable(L, -1)) {
    tv = loadVector(L, index);
    setScaleRate(tv);
  }
  lua_pop(L, 1);

  index = -2;
  lua_pushstring(L, "velocity");
  lua_gettable(L, 1);
  if (lua_istable(L, -1)) {
    tv = loadVector(L, index);
    setVelocity(tv);
  }
  lua_pop(L, 1);

  index = -2;
  lua_pushstring(L, "rotation_velocity");
  lua_gettable(L, 1);
  if (lua_istable(L, -1)) {
    tv = loadVector(L, index);
    setRotationVelocity(tv);
  }
  lua_pop(L, 1);

  index = -2;
  lua_pushstring(L, "rotation");
  lua_gettable(L, 1);
  if (lua_istable(L, -1)) {
    tq = loadQuaternion(L, index);
    setRotation(tq);
  }
  lua_pop(L, 1);

  index = -2;
  lua_pushstring(L, "position");
  lua_gettable(L, 1);
  if (lua_istable(L, -1)) {
    tv = loadVector(L, index);
    setPosition(tv);
  }
  lua_pop(L, 1);

  index = -2;
  lua_pushstring(L, "speed");
  lua_gettable(L, 1);
  if (lua_istable(L, -1)) {
    tv = loadVector(L, index);
    setSpeed(tv);
  }
  lua_pop(L, 1);

  index = -1;
  lua_pushstring(L, "type_id");
  lua_gettable(L, 1);
  if (lua_isnumber(L, index)) {
    setTypeId((int)lua_tonumber(L, -1));
  }
  lua_pop(L, 1);

  index = -1;
  lua_pushstring(L, "collision_detection_enabled");
  lua_gettable(L, 1);
  if (lua_isboolean(L, index)) {
    setCollisionDetectionEnabled(lua_toboolean(L, -1));
  }
  lua_pop(L, 1);

  index = -1;
  lua_pushstring(L, "active");
  lua_gettable(L, 1);
  if (lua_isboolean(L, index)) {
    setActive(lua_toboolean(L, -1));
  }
  lua_pop(L, 1);

  index = -1;
  lua_pushstring(L, "draw_enabled");
  lua_gettable(L, 1);
  if (lua_isboolean(L, index)) {
    setDrawEnabled(lua_toboolean(L, -1));
  }
  lua_pop(L, 1);

  index = -1;
  lua_pushstring(L, "always_lit");
  lua_gettable(L, 1);
  if (lua_isboolean(L, index)) {
    setIsAlwaysLit(lua_toboolean(L, -1));
  }
  lua_pop(L, 1);

  index = -1;
  lua_pushstring(L, "interpolation_enabled");
  lua_gettable(L, 1);
  if (lua_isboolean(L, index)) {
    setIsInterpolationEnabled(lua_toboolean(L, -1));
  }
  lua_pop(L, 1);
}

//-----------------------------------------------------------------------------
void Object::animate(const float &timeElapsed, Object* camera) {
  // exectue the current object's update function
  animateScript(timeElapsed);

  // calculate new position using speed
  if (speed_.x != 0.0f) {
    direction_.scale(speed_.x * timeElapsed);
    position_.x += direction_.x;
    position_.y += direction_.y;
    position_.z += direction_.z;
    direction_.normalize();
  }

  if (speed_.y != 0.0f) {
    up_.scale(speed_.y * timeElapsed);
    position_.x += up_.x;
    position_.y += up_.y;
    position_.z += up_.z;
    direction_.normalize();
  }

  if (speed_.z != 0.0f) {
    Vector rotationAxis;

    rotationAxis.crossProduct(up_, direction_);
    rotationAxis.normalize();

    rotationAxis.scale(speed_.z * timeElapsed);
    position_.x += rotationAxis.x;
    position_.y += rotationAxis.y;
    position_.z += rotationAxis.z;
  }

  // update position using velocity
  if (velocity_.x != 0.0f)
    position_.x += velocity_.x * timeElapsed;

  if (velocity_.y != 0.0f)
    position_.y += velocity_.y * timeElapsed;

  if (velocity_.z != 0.0f)
    position_.z += velocity_.z * timeElapsed;

  // update scale
  if (scaleRate_.x != 0.0f) {
    scale_.x += scaleRate_.x * timeElapsed;
    scaleChanged_ = true;
  }

  if (scaleRate_.y != 0.0f) {
    scale_.y += scaleRate_.y * timeElapsed;
    scaleChanged_ = true;
  }

  if (scaleRate_.z != 0.0f) {
    scale_.z += scaleRate_.z * timeElapsed;
    scaleChanged_ = true;
  }

  if (!isInterpolationEnabled_) {
    if (rotationVelocity_.x != 0.0f ||
        rotationVelocity_.y != 0.0f ||
        rotationVelocity_.z != 0.0f) {
      rotationChanged_ = true;

      Vector tempV;
      Quaternion tempQ;

      tempV.x = rotationVelocity_.x * timeElapsed;
      tempV.y = rotationVelocity_.y * timeElapsed;
      tempV.z = rotationVelocity_.z * timeElapsed;

      tempQ.buildFromEuler(tempV);
      rotation_ = rotation_ * tempQ;
    }
  }
  else {
    rotationChanged_ = true;

    float endVal = valInterpEnd_ - valInterpBegin_;
    float curVal = valInterpCurrent_ - valInterpBegin_;

    float t = 0.0f;

    if (endVal > 0) {
      if (curVal > endVal)
        t = 1.0f;
      else if (curVal < 0.0f)
        t = 0.0f;
      else
        t = curVal / endVal;
    }
    else if (endVal < 0) {
      if (curVal < endVal)
        t = 1.0f;
      else if (curVal > 0.0f)
        t = 0.0f;
      else
        t = curVal / endVal;
    }

    rotation_.slerp(rInterpStart_, t, rInterpEnd_);
  }
}
