/**
 * @file Object.h
 * @brief Base game object from which all concrete types are defined.
 *
 * Base game object.
 */
#ifndef _OBJECT_H_
#define _OBJECT_H_

#include <GL/glut.h>
#include <string>
#include <map>

#include "ObjectNode.h"
#include "Sphere.h"
#include "Quaternion.h"
#include "ParticleSystem.h"
#include "constants.h"

extern "C" {
  #include "lua5.1/lua.h"
  #include "lua5.1/lualib.h"
  #include "lua5.1/lauxlib.h"
}

// States
#define DEAD          0
#define NORMAL        1

#define MAX_TEXTURES  32

class Terrain;
class Camera;

class Object : public ObjectNode {
  public:
    Object();
    virtual ~Object();

    void processCollisions(Object*);

    // virtual functions ///////////////////////
    virtual void draw(Object*) = 0;
    virtual void drawOutline(Object*) = 0;
    virtual void handleCollision(Object*) = 0;
    virtual void update(Vector*) = 0;
    virtual void printTypeName() = 0;
    virtual void drawShadow(Vector*) {}
    virtual void animate(const float&, Object*);
    virtual void orientOnTerrain(Terrain *temp, const Quaternion &baseRotation) {}
    virtual void setHeightFromTerrain(Terrain *temp, const float &offset){}
    virtual void buildLuaObjectTable(lua_State *L);
    virtual void transferLuaObjectTable(lua_State *L);
    ////////////////////////////////////////////

    void showCollisionBox();
    void showControlTriangle();

    float calcTriangleCenter(const float&, const float&, const float&);

    void init();
    void initSettings();
    void cleanup();

    void loadScript(const std::string&, lua_State* luaState);
    void animateScript(const float&);
    void unloadScript();

    void setPosition(const GLfloat&, const GLfloat&, const GLfloat&);
    void setPosition(const Vector&);
    void setRotationAxis(const GLfloat&, const GLfloat&, const GLfloat&, const GLfloat&);
    void setRotationAxis(const Vector&, const GLfloat&);
    void setRotationEuler(const GLfloat&, const GLfloat&, const GLfloat&);
    void setRotationEuler(const Vector&);
    void setRotationQuaternion(const Quaternion&);
    void setVelocity(const GLfloat&, const GLfloat&, const GLfloat&);
    void setVelocity(const Vector&);
    void setRotationVelocity(const GLfloat&, const GLfloat&, const GLfloat&);
    void setRotationVelocity(const Vector&);

    void setScale(const GLfloat&, const GLfloat&, const GLfloat&);
    void setScale(const Vector&);
    void setScaleRate(const GLfloat&, const GLfloat&, const GLfloat&);
    void setScaleRate(const Vector&);

    void setDrawEnabled(const bool &isDrawEnabled) { isDrawEnabled_ = isDrawEnabled; }
    bool getDrawEnabled()                          { return isDrawEnabled_; }

    bool getInView() { return isInView_; }
    void setParticleSystem(const int&);

    float getScriptValue(const char* s) {
      float value = 0.0f;

      if (L_ != NULL) {
        lua_getglobal(L_, s);
        value = (float)lua_tonumber(L_, -1);
        lua_pop(L_, 1);
      }

      return value;
    }

    void setScriptValue(const char* s, const float &value) {
      if (L_ != NULL) {
        lua_pushnumber(L_, value);
        lua_setglobal(L_, s);
      }
    }

    void setState(const unsigned char&);
    void setScript(const std::string&);

    void setSpeed(const GLfloat&, const GLfloat&, const GLfloat&);
    void setSpeed(const Vector&);

    void setCollisionDetectionEnabled(const bool &isCollisionDetectionEnabled) {
      isCollisionDetectionEnabled_ = isCollisionDetectionEnabled;
    }

    bool getCollisionDetectionEnabled() { return isCollisionDetectionEnabled_; }

    void setActive(const bool &new_active) { active_ = new_active; }
    bool getActive()                       { return active_; }

    Vector getPosition()         { return position_; }
    Vector getVelocity()         { return velocity_; }
    Vector getRotationVelocity() { return rotationVelocity_; }
    Vector getScaleRate()        { return scaleRate_; }
    Vector getSpeed()            { return speed_; }

    bool isRotationChanged()                               { return rotationChanged_; }
    void setRotationChanged(const bool &rotationChanged_l) { rotationChanged_ = rotationChanged_l; }
    Quaternion getRotation()                               { return rotation_; }
    void setRotation(const Quaternion &rotation)           { rotation_ = rotation; rotationChanged_ = true;}
    Vector getDirection()                                  { return direction_; }
    Vector getUp()                                         { return up_; }
    Vector getScale()                                      { return scale_; }

    Quaternion getRInterpStart()                           { return rInterpStart_; }
    Quaternion getRInterpEnd()                             { return rInterpEnd_; }
    void setRInterpStart(const Quaternion &rInterpStart_l) { rInterpStart_ = rInterpStart_l;}
    void setRInterpEnd(const Quaternion &rInterpEnd_l)     { rInterpEnd_= rInterpEnd_l;}

    bool isInterpolationEnabled() { return isInterpolationEnabled_; }
    void setIsInterpolationEnabled(const bool &isInterpolationEnabled_l) {
      isInterpolationEnabled_ = isInterpolationEnabled_l;
    }

    float getValInterpBegin()   { return valInterpBegin_; }
    float getValInterpCurrent() { return valInterpCurrent_; }
    float getValInterpEnd()     { return valInterpEnd_; }

    void setValInterpBegin(const float &valInterpBegin)     { valInterpBegin_ = valInterpBegin; }
    void setValInterpCurrent(const float &valInterpCurrent) { valInterpCurrent_ = valInterpCurrent; }
    void setValInterpEnd(const float &valInterpEnd)         { valInterpEnd_ = valInterpEnd; }

    void setSuspendTime(const float &time) { suspendTime_ = time; }

    void setState(const int &state_l) {state_ = state_l; }
    int  getState()                   { return state_; }

    void setScaleChanged(const bool &scaleChanged_l) { scaleChanged_ = scaleChanged_l; }

    void setIsAlwaysLit(const bool &isAlwaysLit) { isAlwaysLit_ = isAlwaysLit; }
    bool getIsAlwaysLit() { return isAlwaysLit_; }

    void setTypeId (const int &type_id) { typeId_ = type_id; }
    int getTypeId()                     { return typeId_; }

    Sphere getBoundingSphere() { return boundingSphere_; }

    std::string getScriptName() { return scriptName_; }

    Vector getOrth() { return orth_; }

    bool getEnableSphereTest() { return enableSphereTest_; }

    bool isDrawable() {
      return getActive() &&
             getInView() &&
             getDrawEnabled() &&
             getState() != DEAD;
    }

    void setGameLevelId(const unsigned int& gameLevelId) { gameLevelId_ = gameLevelId; }
    unsigned int getGameLevelId()                        { return gameLevelId_; }

    static unsigned int textureIds[MAX_TEXTURES];
    static std::map <std::string, unsigned int> textureHash;

  protected:
    // orientation //
    Vector position_;      // x,y,z position of object
    Quaternion rotation_;
    Vector scale_;         // x,y,z scale values

    Vector baseDirection_; // base direction of object
    Vector direction_;     // direction facing
    Vector up_;
    Vector orth_;

    // rate of change //
    Vector velocity_;
    Vector speed_;
    Vector rotationVelocity_;
    Vector scaleRate_;

    // collision detection //
    Vector collisionBox_[2];    // 0 = min points, 1 = max points
    Vector controlPoints_[3];   // used for orienting objects on surfaces
    Sphere boundingSphere_;

    // state attributes //
    unsigned char state_; // objects current state
    bool active_;         // is object active?
    bool isDrawEnabled_;

    bool isInView_;       // is the object within the camera's view

    bool isCollisionDetectionEnabled_;

    // used for interpolation between 2 orientations //
    Quaternion rInterpStart_;
    Quaternion rInterpEnd_;

    float valInterpBegin_, valInterpCurrent_, valInterpEnd_;
    bool isInterpolationEnabled_;

    // Necessary for the Lua implementation
    lua_State* L_;
    std::string scriptName_;
    int scriptIndex_;

    float suspendTime_;

    ParticleSystem *particleSystem_;

    int typeId_;
    unsigned int gameLevelId_;

    Vector lastLight_;
    bool scaleChanged_;
    bool rotationChanged_;

    bool isAlwaysLit_;

    bool enableSphereTest_;

  private:
    void traverseAndCopyLuaTable(lua_State*, lua_State*, const int&);
    void copyLuaTableKey(lua_State*, lua_State*);
};

#endif
