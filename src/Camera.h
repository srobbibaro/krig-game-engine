//////////////////////////////////////////////////////////////////
// Authors     : Steve Robbibaro     sar2                       //
//             : Andrew Yochum       amyst14                    //
//                                                              //
// Filename    : camera.h                                       //
//                                                              //
// Description : Calls for managing camera position.            //
//                                                              //
// Date        : 12-1-03                                        //
//////////////////////////////////////////////////////////////////
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "Object.h"
#include "Frustum.h"

class Camera : public Object
{
    public:
    //private:
     
        Matrix rotationMatrix;     // send to rendering library
        Frustum frustum;
        Matrix projectionMatrix;
        Matrix modelViewMatrix;
	
	   Matrix final;
	   
	   int id;
	   
   // public:	
        Camera();
        virtual ~Camera();
        void update(float);
        void setCamera( const Vector&, const Vector&, const Quaternion&, const Vector& );
        void prepareGLView( void );
        
        // accessor functions
        void getRotationMatrix( Matrix &m ) const;
        
        void printTypeName(void) {
            cout << "camera";
        }
        
        void draw(Object*);
        
        void getFrustum(Frustum &f);
        
        void drawOutline( Object* ) {}; // Camera*
        void drawShadow ( Vector* ) {}
        void handleCollision( Object* ) {};
        void update( Vector* ) {};
        void prepare( void ) {};
        void animate( float, Object* ) {}; // Camera*
        void handleDeath() {};
};

#endif
