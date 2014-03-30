#ifndef _QUATERNION_H_
#define _QUATERNION_H_

#include "matrix.h"
#include "Vector.h"

class Quaternion
{
    private:
        float x, y, z, w;
        
    public:
        Quaternion( void );
        Quaternion( Vector &v);
        Quaternion( float x, float y, float z );
        Quaternion( const Vector &axis, float angle );
        Quaternion( float qX, float qY, float qZ, float qW );
        Quaternion( const Quaternion &t );
        ~Quaternion( void ) {}
        
        void normalize( void );  
        void buildFromEuler( const Vector &v );
        void buildFromEuler( float angleX, float angleY, float angleZ );
        void buildFromAxis ( const Vector &v, float angle );
        void buildFromAxis ( float x, float y, float z );
        void loadMultIdentity( void );
        void loadAddIdentity ( void );
        void getEulerAngles( Vector &v );
        
        void buildRotationMatrix( Matrix &m );
        void rotate( float angle );
        void slerp ( const Quaternion &sQ, float t, const Quaternion &eQ );
        
        // accessor functions 
        float getX( void ) const { return( x ); }
        float getY( void ) const { return( y ); }
        float getZ( void ) const { return( z ); }
        float getW( void ) const { return( w ); }
        
        // overloaded operators
        Quaternion operator+ ( const Quaternion &q );
        Quaternion operator* ( const Quaternion &q );
        Quaternion&	operator=( const Quaternion &q );						
};

#endif
