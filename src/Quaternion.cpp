#include "Quaternion.h"

//------------------------------------------------------------------------------
Quaternion::Quaternion( void )
{
    x = y = z = w = 0.0f;
}

//------------------------------------------------------------------------------
Quaternion::Quaternion( Vector &v )
{
    buildFromEuler(v);
}

//------------------------------------------------------------------------------
Quaternion::Quaternion( float x, float y, float z )
{
    buildFromEuler( x, y, z );
}

//------------------------------------------------------------------------------
Quaternion::Quaternion( const Vector &axis, float angle )
{
    buildFromAxis( axis, angle );
}

//------------------------------------------------------------------------------
Quaternion::Quaternion( float qX, float qY, float qZ, float qW )
{
    x = qX;
    y = qY;
    z = qZ;
    w = qW;
}

//------------------------------------------------------------------------------
Quaternion::Quaternion( const Quaternion &q )
{
    x = q.x;
    y = q.y;
    z = q.z;
    w = q.w;
}

//------------------------------------------------------------------------------
void Quaternion::buildFromEuler( const Vector &v )
{
    // calculate trig identities ONCE
    float cosX = cos(v.x/2.0f);
	float cosY = cos(v.y/2.0f);
	float cosZ = cos(v.z/2.0f);

    float sinX = sin(v.x/2.0f);
	float sinY = sin(v.y/2.0f);
	float sinZ = sin(v.z/2.0f);

	float cosProduct = cosY * cosZ;
	float sinProduct = sinY * sinZ;

    // calculate the quaternion
	w = cosX * cosProduct + sinX * sinProduct;
	x = sinX * cosProduct - cosX * sinProduct;
	y = cosX * sinY * cosZ + sinX * cosY * sinZ;
	z = cosX * cosY * sinZ - sinX * sinY * cosZ;
}

//------------------------------------------------------------------------------
void Quaternion::buildFromEuler( float angleX, float angleY, float angleZ )
{
    // calculate trig identities ONCE
    float cosX = cos(angleX/2.0f);
	float cosY = cos(angleY/2.0f);
	float cosZ = cos(angleZ/2.0f);

    float sinX = sin(angleX/2.0f);
	float sinY = sin(angleY/2.0f);
	float sinZ = sin(angleZ/2.0f);

	float cosProduct = cosY * cosZ;
	float sinProduct = sinY * sinZ;

    // calculate the quaternion
	w = cosX * cosProduct + sinX * sinProduct;
	x = sinX * cosProduct - cosX * sinProduct;
	y = cosX * sinY * cosZ + sinX * cosY * sinZ;
	z = cosX * cosY * sinZ - sinX * sinY * cosZ;
}

//------------------------------------------------------------------------------
void Quaternion::buildFromAxis( const Vector &v, float angle )
{
    // this function currently assumes a valid unit vector is passed in Vector v
	float scale	= sin( angle / 2.0f );

    w = cos( angle / 2.0f);
	x = v.x * scale;
	y = v.y * scale;
	z = v.z * scale;

	//normalize();
}

//------------------------------------------------------------------------------
void Quaternion::normalize()
{
    float len = sqrtf( ( x * x ) + ( y * y ) + ( z * z )  + ( w * w ) );

    if ( len != 0.0f ) {
        x /= len;
        y /= len;
        z /= len;
        w /= len;
    }
}

//------------------------------------------------------------------------------
void Quaternion::loadMultIdentity( void )
{
    x = 0.0f;
    y = 0.0f;
    z = 0.0f;
    w = 1.0f;
}

//------------------------------------------------------------------------------
void Quaternion::loadAddIdentity( void )
{
    x = 0.0f;
    y = 0.0f;
    z = 0.0f;
    w = 0.0f;
}

//------------------------------------------------------------------------------
void Quaternion::getEulerAngles( Vector &v )
{
    Matrix m;
    buildRotationMatrix(m);

    const float epsilon = 0.0000001;

    if(m.data[2] < 1 - epsilon && m.data[2] > -1 + epsilon) {
      v.y = -asin(m.data[2]);
      float c = cos(v.y);
      v.x = atan2(m.data[6] / c, m.data[10] / c);
      v.z = atan2(m.data[1] / c, m.data[0] / c);
   }
   else {
      v.z = 0;
      v.y = -atan2(m.data[2], 0);
      v.x = atan2(-m.data[9], m.data[5]);
   }
}

//------------------------------------------------------------------------------
void Quaternion::buildRotationMatrix( Matrix &m )
{
    // precalculate coefficients
    float addX = x + x;
    float addY = y + y;
    float addZ = z + z;
    float multXX = x * addX;

    float multXY = x * addY;
    float multXZ = x * addZ;
    float multYY = y * addY;
    float multYZ = y * addZ;
    float multZZ = z * addZ;
    float multWX = w * addX;
    float multWY = w * addY;
    float multWZ = w * addZ;

    // column 1
    m.data[0]  = 1.0f - (multYY + multZZ);
    m.data[4]  = multXY - multWZ;
    m.data[8]  = multXZ + multWY;
    m.data[12] = 0.0f;

    // column 2
    m.data[1]  = multXY + multWZ;
    m.data[5]  = 1.0 - (multXX + multZZ);
    m.data[9]  = multYZ - multWX;
    m.data[13] = 0.0f;

    // column 3
    m.data[2]  = multXZ - multWY;
    m.data[6]  = multYZ + multWX;
    m.data[10] = 1.0f - (multXX + multYY);
    m.data[14] = 0.0f;

    // column 4
    m.data[3]  = 0.0f;
    m.data[7]  = 0.0f;
    m.data[11] = 0.0f;
    m.data[15] = 1.0f;
}

//------------------------------------------------------------------------------
void Quaternion::rotate( float angle )
{
    // work in progress

    // dQ/ dt + 0.5 * quat(angle) * Q;
}

//------------------------------------------------------------------------------
void Quaternion::slerp( const Quaternion &sQ, float t, const Quaternion &eQ )
{
    // first make sure t is within tolerance
    if ( t >= 0.0f || t <+ 1.0f ) {
        float tx, ty, tz, tw;

        float dotQ = ( sQ.x * eQ.x ) +
                     ( sQ.y * eQ.y ) +
                     ( sQ.z * eQ.z ) +
                     ( sQ.w * eQ.w );

        // intermediate values used to calculate final quaternion
        float omega, sin1, scale1, scale2;

        // setup the temporary quaternion, if the dot product is negative
        // setup the quaternion to negative
        if ( dotQ < 0.0f ) {
            dotQ = -dotQ;
            tx = -eQ.x;
            ty = -eQ.y;
            tz = -eQ.z;
            tw = -eQ.w;
        }
        else {
            tx = eQ.x;
            ty = eQ.y;
            tz = eQ.z;
            tw = eQ.w;
        }

        // calculate values needed for final calculations
        if ( ( 1.0f - dotQ ) > 0.05f ) {  // 0.05 tolerance
            omega  = acos( dotQ );
            sin1   = sin ( omega );
            scale1 = sin ( ( 1.0f - t ) * omega ) / sin1;
            scale2 = sin ( t * omega ) / sin1;
        }
        else {
            // within tolerance, do a standard linear interpolation
            scale1 = 1.0f - t;
            scale2 = t;
        }

	   // calculate result quaternion
	   x = scale1 * sQ.x + scale2 * tx;
	   y = scale1 * sQ.y + scale2 * ty;
	   z = scale1 * sQ.z + scale2 * tz;
	   w = scale1 * sQ.w + scale2 * tw;
	}
}

//------------------------------------------------------------------------------
Quaternion Quaternion::operator+( const Quaternion &q )
{
    float qx = q.x + x;
    float qy = q.y + y;
    float qz = q.z + z;
    float qw = q.w + w;

    return (Quaternion( qx, qy, qz, qw ) );
}

//------------------------------------------------------------------------------
Quaternion Quaternion::operator*( const Quaternion &q )
{
	float qx = q.w * x + q.x * w + q.y * z - q.z * y;
	float qy = q.w * y + q.y * w + q.z * x - q.x * z;
	float qz = q.w * z + q.z * w + q.x * y - q.y * x;
	float qw = q.w * w - q.x * x - q.y * y - q.z * z;

	return( Quaternion( qx, qy, qz, qw) );
}

//------------------------------------------------------------------------------
Quaternion& Quaternion::operator=( const Quaternion &q )
{
	x = q.x;
	y = q.y;
	z = q.z;
	w = q.w;

	return( *this );
}
