//////////////////////////////////////////////////////////////////
// Description : Class for handling matrix math.                //
//////////////////////////////////////////////////////////////////
#include <math.h>
#include <stdio.h>
#include "matrix.h"

#ifndef _VECTOR_H_
#define _VECTOR_H_

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
