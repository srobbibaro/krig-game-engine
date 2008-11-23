#include "Model.h"

#ifndef _EXPLOSION_H_
#define _EXPLOSION_H_

class Explosion : public Model
{
    public:
        void handleCollision( Object* ) {}
        void prepare() {}
        void handleDeath( void ) {}
        
        Explosion();
        Explosion( float, float* );
        void processAI( Camera* ) {}
        void animate ( float, Camera * );
        
        void printTypeName(void) 
         {
            cout << "Explosion";
         }
};
#endif