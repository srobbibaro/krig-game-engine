#include "Model.h"

#include "Player.h"

#ifndef _ENEMY_SHIP_H_
#define _ENEMY_SHIP_H_

class EnemyShip : public Model
{
    private:
        Object* obj;
        Player* player;             // pointer to player
                
    public:
        void handleCollision( Object* );
        void prepare() {}
        
        EnemyShip();
        EnemyShip( string, string, float*, Player* );
        virtual ~EnemyShip() {}
        
        void processAI( Camera*, float elapsedTime );
        void fireShot();
        
        void printTypeName(void) 
         {
            cout << "EnemyShip";
         }
};


#endif
