#include "Model.h"

#ifndef _PLAYERSHOT_H_
#define _PLAYERSHOT_H_

class PlayerShot : public Model
{
    private:
        Player* player;             // pointer to player
                
    public:
        void handleCollision( Object* );
        void prepare() {}
        void handleDeath( void );
        
        PlayerShot();
        ~PlayerShot();
        PlayerShot( string, float*, Player* );
        void processAI( Camera* ) {}
    
        
        void printTypeName(void) 
         {
            cout << "PlayerShot";
         }
};
#endif
