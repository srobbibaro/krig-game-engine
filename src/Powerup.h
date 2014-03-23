#include "Model.h"

#ifndef _POWERUP_H_
#define _POWERUP_H_

class Powerup : public Model
{
    public:
        int type;

        void handleCollision( Object* );
        void prepare() {}
        void handleDeath( void ) {}

        Powerup();
        Powerup( int, float* );
        void processAI( Camera*, float elapsedTime ) {}

        void printTypeName(void)
         {
            PRINT_DEBUG("Powerup");
         }
};
#endif
