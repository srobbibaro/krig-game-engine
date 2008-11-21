#include "Model.h"

#include "Player.h"

#ifndef _BOATCANNON_H_
#define _BOATCANNON_H_

class BoatCannon : public Model
{
    private:
        Object* obj;
        Vector normal;
        
    public:
        void handleCollision( Object* );
        void prepare() {}
        
        BoatCannon();
        BoatCannon( string, string, float*, Object* );
        void processAI( Camera* );
        
        void printTypeName(void) 
         {
            cout << "BoatCannon";
         }
};

#endif
