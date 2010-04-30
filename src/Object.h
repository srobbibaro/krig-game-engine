// objects.h
// 11/12/2003

//////////////////////////////////////////////////////////////////
// Authors     : Steve Robbibaro     sar2                       //
//             : Andrew Yochum       amyst14                    //
//                                                              //
// Filename    : objects.h                                      //
//                                                              //
// Description : Class for managing model data.                 //
//                                                              //
// Date        : 05-8-04                                        //
//////////////////////////////////////////////////////////////////

#include "ObjectNode.h"
#include "ScriptedObject.h"
#include "ParticleSystem.h"
#include "Sphere.h"

#include "Camera.h"

extern "C" {
    #include "lua.h"
    #include "lualib.h"
    #include "lauxlib.h"
}

#ifndef _OBJECT_H_
#define _OBJECT_H_

class ParticleSystem;

class Object : public ObjectNode
{
    protected:
        // orientation //
        Vector position;            // x,y,z position of object
        Quaternion rotation;
        Vector scale;               // x,y,z scale values

        Vector baseDirection;       // base direction of object
        Vector direction;           // direction facing
        Vector up;
        Vector orth;

        // rate of change //
        Vector velocity;
        Vector speed;
        Vector rotationVelocity;
        Vector scaleRate;

        // collision detection //
        Vector collisionBox[2];    // 0 = min points, 1 = max points
        Vector controlPoints[3];   // used for orienting objects on surfaces
        Sphere boundingSphere;

        // state attributes //
        unsigned char state;    // objects current state
        bool active;            // is object active?
        bool isDrawEnabled_;

        bool isInView;          // is the object within the camera's view

        bool isCollisionDetectionEnabled_;

        // used for interpolation between 2 orientations //
        Quaternion rInterpStart;
        Quaternion rInterpEnd;

        float valInterpBegin_, valInterpCurrent_, valInterpEnd_;
        bool isInterpolationEnabled_;

        // Necessary for the Lua implementation
        lua_State* L;
        string scriptName;
        int scriptIndex;

        float suspendTime;

        ParticleSystem *particleSystem;

        int typeId;


        Vector lastLight;
        bool scaleChanged;
        bool rotationChanged;

        bool isAlwaysLit_;

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

        float calcTriangleCenter( void );

        void init(void);
        void initSettings(void);
        void cleanup(void);

        void loadScript(string file);
        void animateScript( float elapsedTime );
        void unloadScript();

        // virtual functions
        virtual void printTypeName(void) = 0;

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

        void setScale(GLfloat, GLfloat, GLfloat);
        void setScale(const Vector &v);
        void setScaleRate(GLfloat, GLfloat, GLfloat);
        void setScaleRate(const Vector &v);

        void setDrawEnabled(bool isDrawEnabled) { isDrawEnabled_ = isDrawEnabled; }
        bool getDrawEnabled() { return isDrawEnabled_; }

        bool getInView() { return isInView; }
        void setParticleSystem(int particleSystemNumber);

        float getScriptValue(const char* s)
        {
            float value = 0.0f;

            if (L != NULL) {
                lua_getglobal(L, s);
                value = (float)lua_tonumber(L, -1);
                lua_pop(L, 1);
            }

            return value;
        }

        void setScriptValue(const char* s, float value)
        {
            if (L != NULL) {
                lua_pushnumber(L, value);
                lua_setglobal(L, s);
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

        void setActive( bool new_active ) { active = new_active; }
        bool getActive() { return active; }

        Vector getPosition() { return position; }
        Vector getVelocity() { return velocity; }
        Vector getRotationVelocity() { return rotationVelocity; }
        Vector getScaleRate() { return scaleRate; }
        Vector getSpeed() { return speed; }

        bool isRotationChanged() { return rotationChanged; }
        void setRotationChanged( bool rotationChanged_l) { rotationChanged = rotationChanged_l; }
        Quaternion getRotation() { return rotation; }
        void setRotation(Quaternion rotation_l) { rotation = rotation_l; rotationChanged = true;}
        Vector getDirection() { return direction; }
        Vector getUp() { return up; }
        Vector getScale() { return scale; }

        Quaternion getRInterpStart() { return rInterpStart; }
        Quaternion getRInterpEnd() { return rInterpEnd; }
        void setRInterpStart(Quaternion rInterpStart_l) { rInterpStart = rInterpStart_l;}
        void setRInterpEnd(Quaternion rInterpEnd_l) { rInterpEnd= rInterpEnd_l;}

        bool isInterpolationEnabled() { return isInterpolationEnabled_; }
        bool setIsInterpolationEnabled(bool isInterpolationEnabled_l) { isInterpolationEnabled_ = isInterpolationEnabled_l; }

        float getValInterpBegin() { return valInterpBegin_; }
        float getValInterpCurrent() { return valInterpCurrent_; }
        float getValInterpEnd() { return valInterpEnd_; }

        void setValInterpBegin(float valInterpBegin) { valInterpBegin_ = valInterpBegin; }
        void setValInterpCurrent(float valInterpCurrent) { valInterpCurrent_ = valInterpCurrent; }
        void setValInterpEnd(float valInterpEnd) { valInterpEnd_ = valInterpEnd; }

        void setSuspendTime( float time ) { suspendTime = time; }

        void setState( int state_l ) {state = state_l; }
        int  getState() { return state; }

        void setScaleChanged( bool scaleChanged_l ) { scaleChanged = scaleChanged_l; }

        void setIsAlwaysLit( bool isAlwaysLit ) { isAlwaysLit_ = isAlwaysLit; }

        void setTypeId (int type_id) { typeId = type_id; }
        int getTypeId() { return typeId; }

        Sphere getBoundingSphere() { return boundingSphere; }

        string getScriptName() { return scriptName; }

        Vector getOrth() { return orth; }

};

#endif
