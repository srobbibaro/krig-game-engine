//////////////////////////////////////////////////////////////////
// Authors     : Steve Robbibaro     sar2                       //
//             : Andrew Yochum       amyst14                    //
//                                                              //
// Filename    : matrix.cpp                                     //
//                                                              //
// Description : Functions to handle matrix multiplication for  //
//               object transformations.                        //
//                                                              //
// Date        : 12-1-03                                        //
//////////////////////////////////////////////////////////////////
#include "matrix.h"

//------------------------------------------------------------------------------
Matrix::Matrix()
{
    loadIdentity();
}

//------------------------------------------------------------------------------
Matrix::Matrix( const Matrix &t )
{
    for ( int i = 0; i < 16; i++ )
        data[i] = t.data[i];
}

//------------------------------------------------------------------------------
Matrix::Matrix ( float c11, float c21, float c31, float c41,
         float c12, float c22, float c32, float c42,
         float c13, float c23, float c33, float c43,
         float c14, float c24, float c34, float c44 )
{
    data[0] = c11; data[1] = c12; data[2] = c13; data[3] = c14;
    data[4] = c21; data[5] = c22; data[6] = c23; data[7] = c24;
    data[8] = c31; data[9] = c32; data[10] = c33; data[11] = c34;    
    data[12] = c41; data[13] = c42; data[14] = c43; data[15] = c44;

}

//------------------------------------------------------------------------------
void Matrix::loadIdentity(void)
{
    for ( int i = 0; i < 16; i++ )
        data[i] = 0;
    
    data[0] = 1;
    data[5] = 1;
    data[10] = 1;
    data[15] = 1;
}

//------------------------------------------------------------------------------
void Matrix::loadZero(void)
{
    for ( int i = 0; i < 16; i++ )
        data[i] = 0;
}

//------------------------------------------------------------------------------
void Matrix::operator =( Matrix t )
{
    for ( int i = 0; i < 16; i++ )
        data[i] = t.data[i];
}

//------------------------------------------------------------------------------
Matrix Matrix::operator *( Matrix p )
{
    Matrix t; int c, r;
    
    for ( int i = 0; i < 16; i++ )
    {
        t.data[i] = 0.0f;
        
        c = (int ) i / 4; r = i % 4;
        for ( int j = 0; j < 4; j++ )
            t.data[i] += ( p.data[(c*4)+j] * data[ (j*4) + r ] );
    }
        
    return (t);
}


//------------------------------------------------------------------------------
void Matrix::setRotation( float angle, float x, float y, float z )
{ 
    float t = 1 - cos(angle);
    float s = sin(angle);
    float c = cos(angle);
    
    loadIdentity();

    data[0] = ( t *  x * x ) + c;    
    data[4] = ( t * x* y ) + ( s * z);     
    data[8] = ( t * x * z ) - ( s * y );

    data[1] = (t*x*y) - (s*z);    
    data[5] = (t*y*y) + c;     
    data[9] = (t*y*z) + (s*x);
    
    data[2] = (t*x*z) + (s*y);
    data[6] = (t*y*z) - (s*x);     
    data[10] = (t*z*z) + c;
}
/*
//------------------------------------------------------------------------------
void Matrix::setRotation2() 
{   
    data[ 0] = 1.0f - 2.0f * ( r[1] * r[1] + r[2] * r[2] );
	data[ 1] = 2.0f * (r[0] * r[1] + r[2] * r[3]);
	data[ 2] = 2.0f * (r[0] * r[2] - r[1] * r[3]);
	data[ 3] = 0.0f;
	
	// Second row
	data[ 4] = 2.0f * ( r[0] * r[1] - r[2] * r[3] );
	data[ 5] = 1.0f - 2.0f * ( r[0] * r[0] + r[2] * r[2] );
	data[ 6] = 2.0f * (r[2] * r[1] + r[0] * r[3] );
	data[ 7] = 0.0f;

	// Third row
	data[ 8] = 2.0f * ( r[0] * r[2] + r[1] * r[3] );
	data[ 9] = 2.0f * ( r[1] * r[2] - r[0] * r[3] );
	data[10] = 1.0f - 2.0f * ( r[0] * r[0] + r[1] * r[1] );
	data[11] = 0.0f;

	// Fourth row
	data[12] = 0;
	data[13] = 0;
	data[14] = 0;
	data[15] = 1.0f;
}
*/

//------------------------------------------------------------------------------
void Matrix::setTranslation( float x, float y, float z )
{
    loadIdentity();
    
    data[12] = x;
    data[13] = y;
    data[14] = z;
}

//------------------------------------------------------------------------------
void Matrix::setScale( float x, float y, float z )
{
    loadIdentity();
    
    data[0] = x;
    data[5] = y;
    data[10] = z;
}

//------------------------------------------------------------------------------
void Matrix::transpose(Matrix &m)
{ 
    for ( int j = 0; j < 3; j++ ) {
        for (int i = 0; i < (4-j); i++) {
            m.data[(i*4)+j]=data[15-(j*4)-i];
            m.data[15-(j*4)-i]=data[(i*4)+j];
        }
    }
}


//------------------------------------------------------------------------------
Vector::Vector()
{
    x = y = z = 0.0f;
}

//------------------------------------------------------------------------------
Vector::Vector( float tx, float ty, float tz )
{
    x = tx; y = ty; z = tz;
}

//------------------------------------------------------------------------------
void Vector::setVector( float tx, float ty, float tz )
{
    x = tx; y = ty; z = tz;
}

//------------------------------------------------------------------------------
void Vector::normalize()
{
    float len = sqrtf( ( x * x ) + ( y * y ) + ( z * z ) );
    
    /*
    if ( len != 0.0f )
    {
        x /= len;
        y /= len;
        z /= len;
    }
    
    */
}

//------------------------------------------------------------------------------
float Vector::dotProduct( Vector &rhs )
{
   return( ( ( x * rhs.x ) + ( y * rhs.y ) + ( z * rhs.z ) ) );
}

//------------------------------------------------------------------------------
void Vector::operator = ( Vector rhs )
{
    x = rhs.x; y = rhs.y; z = rhs.z;
}

//------------------------------------------------------------------------------
void Vector::rotateVector( Matrix &M, Vector &V )
{
    x = ( M.data[0] * V.x ) + ( M.data[4] * V.y ) + ( M.data[8] * V.z );
    y = ( M.data[1] * V.x ) + ( M.data[5] * V.y ) + ( M.data[9] * V.z );
    z = ( M.data[2] * V.x ) + ( M.data[6] * V.y ) + ( M.data[10] * V.z );
}

//------------------------------------------------------------------------------
void Vector::crossProduct ( Vector &d1, Vector &d2 )
{
    x = d1.y * d2.z   -   d1.z * d2.y;
    y = d1.z * d2.x   -   d1.x * d2.z;
    z = d1.x * d2.y   -   d1.y * d2.x;
}

//------------------------------------------------------------------------------
void Vector::calcNorm( Vector &p1, Vector &p2, Vector &p3 )
{
    // calculate the surface normal of the triangle formed by p1, p2 and p3
    
    Vector t1;
    Vector t2;
    
    t1.setVector( p2.x - p1.x, p2.y - p1.y, p2.z - p1.z );
    t2.setVector( p3.x - p1.x, p3.y - p1.y, p3.z - p1.z );
 
    crossProduct( t1, t2 );
    normalize();
}

//------------------------------------------------------------------------------
void Vector::transformVector( Matrix &m, Vector &v )
{
    float h;
    
    x = ( v.x * m.data[0] ) + ( v.y * m.data[4] ) + ( v.z * m.data[8] ) + m.data[12];
    y = ( v.x * m.data[1] ) + ( v.y * m.data[5] ) + ( v.z * m.data[9] ) + m.data[13];
    z = ( v.x * m.data[2] ) + ( v.y * m.data[6] ) + ( v.z * m.data[10] ) + m.data[14];
    h = ( v.x * m.data[3] ) + ( v.y * m.data[7] ) + ( v.z * m.data[11] ) + m.data[15];
        
   //x /= h;
   // y /= h;
   // z /= h;
}

//------------------------------------------------------------------------------
void Vector::average( Vector &a, Vector &b )
{
    x = a.x + b.x;
    y = a.y + b.y;
    z = a.z + b.z;
    
    normalize();
}

//------------------------------------------------------------------------------
void Vector::scale( float m )
{
    x *= m; y *= m; z *= m;
}

//------------------------------------------------------------------------------
float Vector::getDistance( Vector t )
{
    float dx = t.x - x;
    float dy = t.y - y;
    float dz = t.z - z;

    return sqrtf( dx*dx + dy*dy + dz*dz );
}

//------------------------------------------------------------------------------
float Vector::getSum(void)
{
    return (x + y + z);
}

//------------------------------------------------------------------------------
float Vector::getScaler(Vector vector)
{
    Vector t;
    t.x = x * vector.x;
    t.y = y * vector.y;
    t.z = z * vector.z;
    
    return (t.x + t.y + t.z);
}

//------------------------------------------------------------------------------
bool Vector::intersectBox( 
    Vector rayPosition, Vector collisionBox[], 
    float extend, Vector &tv 
 )
{
    bool hit = false;
    float s = 0;
           
    // test planes with fixed x value //
    if ( x > 0 ) {
        if ( rayPosition.x <= collisionBox[1].x+extend )
            s = (collisionBox[1].x+extend-rayPosition.x) / x;           
    }
    else if ( x < 0 ) {
        if ( rayPosition.x >= collisionBox[0].x-extend )
            s = (collisionBox[0].x-extend-rayPosition.x) / x;
    }
                                 
            
    if ( s != 0 ) {
        tv.x = rayPosition.x +x * s;
        tv.y = rayPosition.y +y * s;
        tv.z = rayPosition.z +z * s;
                
        if ( tv.y <= collisionBox[1].y+extend && tv.y >= collisionBox[0].y-extend && 
             tv.z <= collisionBox[1].z+extend && tv.z >= collisionBox[0].z-extend) {
                hit = true;
                }
            }
    //////////////////////////////////////
            
            
    // if no hit on x planes test planes with fixed y value //
    if ( !hit ) {
        s = 0;
                
        if ( y > 0 ) {
            if ( rayPosition.y <= collisionBox[1].y+extend )
                s = (collisionBox[1].y+extend-rayPosition.y) / y;
              
        }
        else if ( y < 0 ) {
            if ( rayPosition.y >= collisionBox[0].y-extend )
                s = (collisionBox[0].y-extend-rayPosition.y) / y;           
        }
            
        if ( s != 0 ) {
            if ( tv.x <= collisionBox[1].x+extend && tv.x >= collisionBox[0].x-extend && 
                 tv.z <= collisionBox[1].z+extend && tv.z >= collisionBox[0].z-extend) {
                 hit = true;
            }
        }
    }
    ////////////////////////////////////////////////////////////
            
    // if no hit on x or y planes test planes with fixed z value //
    if ( !hit ) {
        s = 0;
                
        if ( z > 0 ) {
            if ( rayPosition.z <= collisionBox[1].z+extend )
                s = (collisionBox[1].z+extend-rayPosition.z) / z;   
        }
        else if ( z < 0 ) {
            if ( rayPosition.z >= collisionBox[0].z-extend )
                s = (collisionBox[0].z-extend-rayPosition.z) / z;  
        }
            
        if ( s != 0 ) {
            tv.x = rayPosition.x +x * s;
            tv.y = rayPosition.y +y * s;
            tv.z = rayPosition.z +z * s;          
                    
            if ( tv.x <= collisionBox[1].x+extend && tv.x >= collisionBox[0].x-extend && 
                 tv.y <= collisionBox[1].y+extend && tv.y >= collisionBox[0].y-extend) {
                    hit = true;
            }  
        }
    }
    ///////////////////////////////////////////////////////////////////
            
    return hit;
}

//------------------------------------------------------------------------------
bool Vector::intersectBox( Vector rayPosition, Vector collisionBox[], float extend )
{
    Vector tv;
    bool hit = false;
    float s = 0;
           
    // test planes with fixed x value //
    if ( x > 0 ) {
        if ( rayPosition.x <= collisionBox[1].x+extend )
            s = (collisionBox[1].x+extend-rayPosition.x) / x;           
    }
    else if ( x < 0 ) {
        if ( rayPosition.x >= collisionBox[0].x-extend )
            s = (collisionBox[0].x-extend-rayPosition.x) / x;
    }                             
            
    if ( s != 0 ) {
        tv.x = rayPosition.x +x * s;
        tv.y = rayPosition.y +y * s;
        tv.z = rayPosition.z +z * s;
                
        if ( tv.y <= collisionBox[1].y+extend && tv.y >= collisionBox[0].y-extend && 
             tv.z <= collisionBox[1].z+extend && tv.z >= collisionBox[0].z-extend) {
                hit = true;
                }
            }
    //////////////////////////////////////
            
            
    // if no hit on x planes test planes with fixed y value //
    if ( !hit ) {
        s = 0;
                
        if ( y > 0 ) {
            if ( rayPosition.y <= collisionBox[1].y+extend )
                s = (collisionBox[1].y+extend-rayPosition.y) / y;
              
        }
        else if ( y < 0 ) {
            if ( rayPosition.y >= collisionBox[0].y-extend )
                s = (collisionBox[0].y-extend-rayPosition.y) / y;           
        }
            
        if ( s != 0 ) {
            if ( tv.x <= collisionBox[1].x+extend && tv.x >= collisionBox[0].x-extend && 
                 tv.z <= collisionBox[1].z+extend && tv.z >= collisionBox[0].z-extend) {
                 hit = true;
            }
        }
    }
    ////////////////////////////////////////////////////////////
            
    // if no hit on x or y planes test planes with fixed z value //
    if ( !hit ) {
        s = 0;
                
        if ( z > 0 ) {
            if ( rayPosition.z <= collisionBox[1].z+extend )
                s = (collisionBox[1].z+extend-rayPosition.z) / z;   
        }
        else if ( z < 0 ) {
            if ( rayPosition.z >= collisionBox[0].z-extend )
                s = (collisionBox[0].z-extend-rayPosition.z) / z;  
        }
            
        if ( s != 0 ) {
            tv.x = rayPosition.x +x * s;
            tv.y = rayPosition.y +y * s;
            tv.z = rayPosition.z +z * s;          
                    
            if ( tv.x <= collisionBox[1].x+extend && tv.x >= collisionBox[0].x-extend && 
                 tv.y <= collisionBox[1].y+extend && tv.y >= collisionBox[0].y-extend) {
                    hit = true;
            }  
        }
    }
    ///////////////////////////////////////////////////////////////////
            
    return hit;
}

//------------------------------------------------------------------------------
Vector Vector::operator +( Vector a )
{
    Vector t;
    t.setVector(x+a.x, y+a.y, z+a.z);
    return t;
}

//------------------------------------------------------------------------------
Vector Vector::operator * ( float a )
{
    return Vector( a*x, a*y, a*z );   
}

//------------------------------------------------------------------------------
void Vector::operator += ( Vector a )
{
    x+=(a.x); y+=(a.y); z+=(a.z);
}

//------------------------------------------------------------------------------
void Vector::operator -= ( Vector a )
{
    x-=(a.x); y-=(a.y); z-=(a.z);
}

//------------------------------------------------------------------------------
float Vector::getLength(void)
{
    return (sqrtf( ( x * x ) + ( y * y ) + ( z * z ) ));
}

//------------------------------------------------------------------------------
void Matrix::transformVertex( float*v , float *newVertex)
{
    float h, x, y, z;
    
    x = ( v[0] * data[0] ) + ( v[1] * data[4] ) + ( v[2] * data[8] ) + data[12];
    y = ( v[0] * data[1] ) + ( v[1] * data[5] ) + ( v[2] * data[9] ) + data[13];
    z = ( v[0] * data[2] ) + ( v[1] * data[6] ) + ( v[2] * data[10] ) + data[14];
    h = ( v[0] * data[3] ) + ( v[1] * data[7] ) + ( v[2] * data[11] ) + data[15];
        
    newVertex[0] = x; newVertex[1] = y; newVertex[2] = z;
    
    newVertex[0] /= h;
    newVertex[1] /= h;
    newVertex[2] /= h;
}

//------------------------------------------------------------------------------
void Matrix::setShadow( float lightPos[4], float plane[4] )
{
    float temp = plane[0] * lightPos[0] + plane[1] * lightPos[1] + plane[1] * lightPos[2] + plane[3] * lightPos[3];
    
    data[0] = temp - lightPos[0] * plane[0];
    data[4] = 0.0f - lightPos[0] * plane[1];
    data[8] = 0.0f - lightPos[0] * plane[2];
    data[12] = 0.0f - lightPos[0] * plane[3];
    
    data[1] = 0.0f - lightPos[1] * plane[0];
    data[5] = temp - lightPos[1] * plane[1];
    data[9] = 0.0f - lightPos[1] * plane[2];
    data[13] = 0.0f - lightPos[1] * plane[3];

    data[2] = 0.0f - lightPos[2] * plane[0];
    data[6] = 0.0f - lightPos[2] * plane[1];
    data[10] = temp - lightPos[2] * plane[2];
    data[14] = 0.0f - lightPos[2] * plane[3];
    
    data[3] = 0.0f - lightPos[3] * plane[0];
    data[7] = 0.0f - lightPos[3] * plane[1];
    data[11] = 0.0f - lightPos[3] * plane[2];
    data[15] = temp - lightPos[3] * plane[3];
}

//------------------------------------------------------------------------------
void Matrix::display( void )
{
    printf("\n");
    for ( int i = 0; i < 16; i++ )
    {
        printf("%.3f\t",data[i] );
        if ( (i+1) % 4 == 0 )
            printf( "\n" );
    }
}

//------------------------------------------------------------------------------
void Matrix::setRotation2( float x, float y, float z )
{
    // simplify math for later //
    float cosX = cos(x/2);
    float cosY = cos(y/2);
    float cosZ = cos(z/2);
    
    float sinX = sin(x/2);
    float sinY = sin(y/2);
    float sinZ = sin(z/2);
    
    float cosFinal = cosY * cosZ;
    float sinFinal = sinY * sinZ;
    
    // build the quaternion //
    float quatW = cosX * cosFinal + sinX * sinFinal;
	float quatX = sinX * cosFinal - cosX * sinFinal;
	float quatY = cosX * sinY * cosZ + sinX * cosY * sinZ;
	float quatZ = cosX * cosY * sinZ - sinX * sinY * cosZ;
    
    float wx, wy, wz, xx, yy, yz, xy, xz, zz, x2, y2, z2;

    // calculate coefficients
    x2 = quatX + quatX; y2 = quatY + quatY;
    z2 = quatZ + quatZ;
    xx = quatX * x2; xy = quatX * y2; xz = quatX * z2;
    yy = quatY * y2; yz = quatY * z2; zz = quatZ * z2;
    wx = quatW * x2; wy = quatW * y2; wz = quatW * z2;

    // build the matrix //
    data[0] = 1.0 - (yy + zz); data[4] = xy - wz;
    data[8] = xz + wy; data[12] = 0.0;

    data[1] = xy + wz; data[5] = 1.0 - (xx + zz);
    data[9] = yz - wx; data[13] = 0.0;


    data[2] = xz - wy; data[6] = yz + wx;
    data[10] = 1.0 - (xx + yy); data[14] = 0.0;


    data[3] = 0; data[7] = 0;
    data[11] = 0; data[15] = 1;
}

