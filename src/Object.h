//////////////////////////////////////////////////////////////////
// Description : Class for managing model data.                 //
//////////////////////////////////////////////////////////////////

#include "ObjectNode.h"
#include "ScriptedObject.h"
#include "ParticleSystem.h"
#include "Sphere.h"

#include "Camera.h"

extern "C" {
    #include "lua5.1/lua.h"
    #include "lua5.1/lualib.h"
    #include "lua5.1/lauxlib.h"
}

#ifndef _OBJECT_H_
#define _OBJECT_H_

class ParticleSystem;
class Terrain;

class Object : public ObjectNode
{
    protected:
        // orientation //
        Vector position_;            // x,y,z position of object
        Quaternion rotation_;
        Vector scale_;               // x,y,z scale values

        Vector baseDirection_;       // base direction of object
        Vector direction_;           // direction facing
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
        unsigned char state_;    // objects current state
        bool active_;            // is object active?
        bool isDrawEnabled_;

        bool isInView_;          // is the object within the camera's view

        bool isCollisionDetectionEnabled_;

        // used for interpolation between 2 orientations //
        Quaternion rInterpStart_;
        Quaternion rInterpEnd_;

        float valInterpBegin_, valInterpCurrent_, valInterpEnd_;
        bool isInterpolationEnabled_;

        // Necessary for the Lua implementation
        lua_State* L_;
        string scriptName_;
        int scriptIndex_;

        float suspendTime_;

        ParticleSystem *particleSystem_;

        int typeId_;


        Vector lastLight_;
        bool scaleChanged_;
        bool rotationChanged_;

        bool isAlwaysLit_;

        bool enableSphereTest_;

    public:
        Object();
        virtual ~Object();


        void processCollisions( Object* );
        Object* getRoot();

        // virtual functions ///////////////////////
        virtual void draw( Object* ) = 0; // Camera*
        virtual void drawOutline( Object* ) = 0; // Camera*
        virtual void drawShadow ( Vector* ) {}
        virtual void handleCollision( Object* ) = 0;
        virtual void update( Vector* ) = 0;
        virtual void animate( float, Object* ) = 0; // Camera*
        ////////////////////////////////////////////

        void showCollisionBox();
        void showControlTriangle();

        float calcTriangleCenter( float, float, float );

        void init(void);
        void initSettings(void);
        void cleanup(void);

        void loadScript(string file, float args[], int n);
        void animateScript( float elapsedTime );
        void unloadScript();

        // virtual functions
        virtual void printTypeName(void) = 0;

        void setPosition( const GLfloat&, const GLfloat&, const GLfloat& );
        void setPosition( const Vector& );
        void setRotationAxis( const GLfloat &vx, const GLfloat &vy, const GLfloat &vz, const GLfloat &vw );
        void setRotationAxis( const Vector &v, GLfloat a );
        void setRotationEuler( const GLfloat&, const GLfloat&, const GLfloat& );
        void setRotationEuler( const Vector &v );
        void setRotationQuaternion( const Quaternion &q );
        void setVelocity( GLfloat, GLfloat, GLfloat );
        void setVelocity( const Vector &v );
        void setRotationVelocity( GLfloat xAngle, GLfloat yAngle, GLfloat zAngle );
        void setRotationVelocity( const Vector &v );

        void setScale(GLfloat, GLfloat, GLfloat);
        void setScale(const Vector &v);
        void setScaleRate(GLfloat, GLfloat, GLfloat);
        void setScaleRate(const Vector &v);

        void setDrawEnabled(bool isDrawEnabled) { isDrawEnabled_ = isDrawEnabled; }
        bool getDrawEnabled() { return isDrawEnabled_; }

        bool getInView() { return isInView_; }
        void setParticleSystem(int particleSystemNumber);

        float getScriptValue(const char* s)
        {
            float value = 0.0f;

            if (L_ != NULL) {
                lua_getglobal(L_, s);
                value = (float)lua_tonumber(L_, -1);
                lua_pop(L_, 1);
            }

            return value;
        }

        void setScriptValue(const char* s, float value)
        {
            if (L_ != NULL) {
                lua_pushnumber(L_, value);
                lua_setglobal(L_, s);
            }
        }

        void setState(unsigned char);
        void setScript( string name );

        void setSpeed( GLfloat, GLfloat, GLfloat );
        void setSpeed( const Vector &v );

        void setCollisionDetectionEnabled(bool isCollisionDetectionEnabled)
        {
            isCollisionDetectionEnabled_ = isCollisionDetectionEnabled;
        }

        bool getCollisionDetectionEnabled() { return isCollisionDetectionEnabled_; }

        void setActive( bool new_active ) { active_ = new_active; }
        bool getActive() { return active_; }

        Vector getPosition() { return position_; }
        Vector getVelocity() { return velocity_; }
        Vector getRotationVelocity() { return rotationVelocity_; }
        Vector getScaleRate() { return scaleRate_; }
        Vector getSpeed() { return speed_; }

        bool isRotationChanged() { return rotationChanged_; }
        void setRotationChanged( bool rotationChanged_l) { rotationChanged_ = rotationChanged_l; }
        Quaternion getRotation() { return rotation_; }
        void setRotation(Quaternion rotation) { rotation_ = rotation; rotationChanged_ = true;}
        Vector getDirection() { return direction_; }
        Vector getUp() { return up_; }
        Vector getScale() { return scale_; }

        Quaternion getRInterpStart() { return rInterpStart_; }
        Quaternion getRInterpEnd() { return rInterpEnd_; }
        void setRInterpStart(Quaternion rInterpStart_l) { rInterpStart_ = rInterpStart_l;}
        void setRInterpEnd(Quaternion rInterpEnd_l) { rInterpEnd_= rInterpEnd_l;}

        bool isInterpolationEnabled() { return isInterpolationEnabled_; }
        bool setIsInterpolationEnabled(bool isInterpolationEnabled_l) { isInterpolationEnabled_ = isInterpolationEnabled_l; }

        float getValInterpBegin() { return valInterpBegin_; }
        float getValInterpCurrent() { return valInterpCurrent_; }
        float getValInterpEnd() { return valInterpEnd_; }

        void setValInterpBegin(float valInterpBegin) { valInterpBegin_ = valInterpBegin; }
        void setValInterpCurrent(float valInterpCurrent) { valInterpCurrent_ = valInterpCurrent; }
        void setValInterpEnd(float valInterpEnd) { valInterpEnd_ = valInterpEnd; }

        void setSuspendTime( float time ) { suspendTime_ = time; }

        void setState( int state_l ) {state_ = state_l; }
        int  getState() { return state_; }

        void setScaleChanged( bool scaleChanged_l ) { scaleChanged_ = scaleChanged_l; }

        void setIsAlwaysLit( bool isAlwaysLit ) { isAlwaysLit_ = isAlwaysLit; }

        void setTypeId (int type_id) { typeId_ = type_id; }
        int getTypeId() { return typeId_; }

        Sphere getBoundingSphere() { return boundingSphere_; }

        string getScriptName() { return scriptName_; }

        Vector getOrth() { return orth_; }

        bool getEnableSphereTest() { return enableSphereTest_; }

        virtual void orientOnTerrain(Terrain *temp, Quaternion baseRotation)
        {

        }

        virtual void setHeightFromTerrain(Terrain *temp, float offset){}

};

#endif
