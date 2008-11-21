#include "Model.h"

#ifndef _SAIL_BOAT_H_
#define _SAIL_BOAT_H_

class SailBoat : public Model
{
    private:
        
    public:
        void handleCollision( Object* );
        void prepare() {}
        
        SailBoat();
        SailBoat( string, string, float* );
        void processAI( Camera* );
        
        void printTypeName(void) 
         {
            cout << "Powerup";
         }
};

#endif
