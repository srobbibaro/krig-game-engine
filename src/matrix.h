//////////////////////////////////////////////////////////////////
// Description : Class for handling matrix math.                //
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
    
#endif
