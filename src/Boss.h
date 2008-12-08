#include "model.h"

#include "Player.h"

#ifndef _BOSS_H_
#define _BOSS_H_

class Boss : public Model
{
    private:
        Object* obj;                // used to generate new shots
        Player* player;             // pointer to player
        bool* complete;             // set once level is complete (boss is dead)
        
    public:
        void handleCollision( Object* );
        void prepare() {}
        
        Boss();
        Boss( string, string, float*, Player*, bool* );
        virtual ~Boss();
        
        void processAI( Object*, float elapsedTime ); //# Camera*
        void decrementShots( void );
        
        void printTypeName(void) 
         {
            cout << "Boss";
         }
};


#endif
