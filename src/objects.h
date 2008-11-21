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
#include "camera.h"

#ifndef _OBJECTS_H_
#define _OBJECTS_H_
    
    class Object : public ObjectNode, public ScriptedObject
    {
       public:
       //protected:
            Vector scaleFactor;         // x,y,z scale values
            Vector scaleRate;           // x,y,z rate of scale 
            
            // collision detection //
            Vector collisionBox[2]; // 0 = min points, 1 = max points
            Vector controlPoints[3];// used for orienting objects on surfaces
            
            // animation //
            float animStartTime;    // used mostly for special effects like invul
            float duration;         // duration of animation
            
            float elapsedTime;

            // state attributes //   
            int life;               // hit points remaining
            int state;              // objects current state
            bool active;            // is object active?
            bool isEnemy;           // 0 not enemy, 1 = enemy
            bool isVisible;         // is the object visible
            bool collision;         // was there a collision?
            bool testActiveZoneEnable;        
            
            // shot information //
            float shotDelay;        // time between shot regen
            float shotTime;         // time of last shot    
             
            int numShots;           // number of shots remaining
            int numMissiles;        // number of missiles remaining
            
            // pointer to sound class //
            Sound *s;               // pointer to sound library
            
                                 
        //public:
            Object();
            virtual ~Object();
            
            void setScaleFactor( GLfloat, GLfloat, GLfloat );
            void setScaleFactor( const Vector &v );
            void setScaleRate( GLfloat, GLfloat, GLfloat );
            void setScaleRate( const Vector &v );
            
            void setState( unsigned char );    
            
            void drawObjects( Camera* );
            void drawObjectOutlines( Camera* );
            void drawShadows( Vector* );
            
            void updateObjects( Vector*  );
            void processCollisions( Object* );
            void prepareObjects();
            void animateObjects( float, Camera* );

            Object* getRoot();
            
            bool checkActiveEnemy( void );
            void setShot (Object*, float );
            
            
            // virtual functions ///////////////////////
            virtual void draw( Camera* ) = 0;
            virtual void drawOutline( Camera* ) = 0;
            virtual void drawShadow ( Vector* ) {}
            virtual void handleCollision( Object* ) = 0;
            virtual void update( Vector* ) = 0;
            virtual void prepare( void ) = 0;
            virtual void animate( float, Camera* ) = 0;
            virtual void handleDeath() = 0;
            virtual void fireShot() { }
            ////////////////////////////////////////////            

            
            void showCollisionBox(); 
            void showControlTriangle();
            float calcTriangleCenter( void );
            bool testActiveZone( Camera* );
            void setSoundClass( Sound * );
            void decrementShots( void );
            
            int processExtendedCommand( const ScriptCommand & );
    };
    
#endif
