//////////////////////////////////////////////////////////////////
// Authors     : Steve Robbibaro     sar2                       //
//             : Andrew Yochum       amyst14                    //
//                                                              //
// Filename    : matrix.h                                       //
//                                                              //
// Description : Class for handling matrix math.                //
//                                                              //
// Date        : 12-1-03                                        //
//////////////////////////////////////////////////////////////////
#include <math.h>
#include <stdio.h>

#ifndef _MATRIX_H_
#define _MATRIX_H_

    struct Matrix
    {
        float data[16];       // 4 x 4
        
        Matrix();
        Matrix( const Matrix& );
        Matrix ( float, float, float, float,
                 float, float, float, float,
                 float, float, float, float,
                 float, float, float, float );
                 
        void loadIdentity(void);
        void loadZero(void);
        void operator =( Matrix );
        Matrix operator *( Matrix );
        void transpose(Matrix&);
        void setRotation( float, float, float, float );
        void setRotation2(float, float, float);
        void setTranslation( float, float, float );
        void setScale( float, float, float );
        void setShadow( float lightPos[4], float plane[4] );
        void transformVertex( float* , float*);
        void display( void );
        void CreateFromAxisAngle(float x, float y, float z, float angle);

        void CreateFromAxisAngles(float x, float y, float z, float angle);


        void fix();
    };
    
    struct Vector
    {
        float x, y, z;
        
        Vector();
        Vector( float, float, float );
        void setVector( float, float, float );
        void normalize();
        float dotProduct( Vector & );
        void crossProduct( Vector &, Vector & );
        float getLength(void);
        float getSum(void);

        void operator = ( Vector );
        Vector operator * ( float );
        Vector operator +( Vector );
        void operator += (Vector);
        void operator -= (Vector);

        float getScaler(Vector);


        float getDistance( Vector );
        void rotateVector( Matrix &, Vector & );
        void scale( float );
        void calcNorm( Vector &, Vector &, Vector & );
        void transformVector( Matrix &, Vector & );
        void average( Vector &, Vector & );
        bool intersectBox( Vector, Vector[], float, Vector &); 
        bool intersectBox( Vector, Vector[], float);         
    };
    
#endif
