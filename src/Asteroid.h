#include "Model.h"

#ifndef _ASTEROID_H_
#define _ASTEROID_H_

class Asteroid : public Model
{
    private:
        
    public:
        void handleCollision( Object* );
        void prepare() {}
        
        Asteroid();
        Asteroid( string, string, float* );
        void processAI( Camera*, float elapsedTime ) {} 
        
        void printTypeName(void) 
        {
            cout << "Asteroid";
        }
};


#endif
