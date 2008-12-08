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
#include "Sound.h"
#include "ScriptedObject.h"
#include "ParticleSystem.h"

#include "Camera.h"

extern "C" {
    #include "lua/lua.h"
    #include "lua/lualib.h"
    #include "lua/lauxlib.h"
}

#ifndef _OBJECT_H_
#define _OBJECT_H_

class ParticleSystem;
    
class Object : public ObjectNode
{ 
    public:
        Object *temp;
    //protected:
        // orientation //
        Vector position;            // x,y,z position of object
        Quaternion rotation;
        Vector baseDirection;       // base direction of object
        Vector direction;           // direction facing
        Vector up;
        
        Vector scale;              // x,y,z scale values
        
        // rate of change //
        Vector velocity;
        Vector rotationVelocity;
        float speed;                // speed of object, used in scripting  
        int speedDir; 
        Vector scaleRate;          // x,y,z rate of scale 
            
        // collision detection //
        Vector collisionBox[2];    // 0 = min points, 1 = max points
        Vector controlPoints[3];   // used for orienting objects on surfaces
            
        // state attributes //   
        unsigned char state;    // objects current state
        bool active;            // is object active?
        bool isVisible;         // is the object visible
        bool collision;        // was there a collision?
                                   
        // pointer to sound class //
        Sound *s;           // pointer to sound library
                
        // used for interpolation between 2 orientations //
        Quaternion rInterpStart;
        Quaternion rInterpEnd;
        
        int tieMemVarIndex;
        float valInterpStart;
        float valInterpEnd;
        float* valInterpPtr;
        bool interp;
                 
        // pointer to camera and player //
        Object* camera;
        Object* player;
          
        // Necessary for the Lua implementation
        lua_State* L;
        string scriptName;
        
        bool suspend;
        float suspendTime;
        
        ParticleSystem *particleSystem;
        
    //public:
        Object();
        virtual ~Object();
            
        void setState(unsigned char);    
            
        void drawObjects( Object* ); // Camera*
        void drawObjectOutlines( Object* ); // Camera*
        void drawShadows( Vector* );
            
        void updateObjects( Vector*  );
        void processCollisions( Object* );
        void prepareObjects();
        void animateObjects( float, Object* ); // Camera*

        Object* getRoot();
            
        // virtual functions ///////////////////////
        virtual void draw( Object* ) = 0; // Camera*
        virtual void drawOutline( Object* ) = 0; // Camera*
        virtual void drawShadow ( Vector* ) {}
        virtual void handleCollision( Object* ) = 0;
        virtual void update( Vector* ) = 0;
        virtual void prepare( void ) = 0;
        virtual void animate( float, Object* ) = 0; // Camera*
        virtual void handleDeath() = 0;
        ////////////////////////////////////////////            

            
        void showCollisionBox(); 
        void showControlTriangle();
        float calcTriangleCenter( void );
        bool testActiveZone( Object* ); // Camera*
        void setSoundClass( Sound * );
             
        void init(void);
        
        void loadScript(string file);
        void unloadScript();
        
        void setInterpolationVariable(int index);
    
        void animateScript( float elapsedTime );
        
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
        void setTimer( float* );
        void setPlayerPtr( Object* );
        void setCameraPtr( Object* );
        void setLuaState(lua_State*);
        void setScale(GLfloat, GLfloat, GLfloat);
        void setScale(const Vector &v);
        void setScaleRate(GLfloat, GLfloat, GLfloat);
        void setScaleRate(const Vector &v);
        
        void setParticleSystem(int particleSystemNumber);
};
    
#endif
