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

