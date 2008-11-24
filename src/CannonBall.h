#include "Model.h"

#ifndef _CANNON_BALL_H_
#define _CANNON_BALL_H_

class CannonBall : public Model
{
    private:
    public:
        void handleCollision( Object* );
        void prepare() {}
        
        CannonBall( string, float* );
        void processAI( Camera*, float );
        void animate( float, Camera* );
     
        
        void printTypeName(void) 
         {
            cout << "CannonBall";
         }
};


#endif
