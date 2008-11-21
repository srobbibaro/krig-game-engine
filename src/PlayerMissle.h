#include "Model.h"

#ifndef _PLAYERMISSLE_H_
#define _PLAYERMISSLE_H_

class PlayerMissle : public Model
{
    private:
        Player* player;             // pointer to player
        Object* obj;
        
        float totalTime;
                
    public:
        void handleCollision( Object* );
        void prepare() {}
        void handleDeath( void );
        
        PlayerMissle();
        PlayerMissle( string, float*, Player*, Object* );
        void processAI( Camera* );
    
        
        void printTypeName(void) 
         {
            cout << "PlayerMissle";
         }
};
#endif

