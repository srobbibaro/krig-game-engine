#include "model.h"

#ifndef _PLAYER_H_
#define _PLAYER_H_

class Player : public Model
{
    public:
    //private:
        Object *pShot;          // pointer to player's laser shot
        long score;
        int numLives;
                
   // public:
        void handleCollision( Object* );
        void prepare() {}
        
        Player( string );
        virtual ~Player();
        void processAI( Camera*, float );
        void animate( float, Camera* );
        void fireShot ( float );
        void fireMissle( Object* tobj );
        long getScore();
        int getNumLives();
        void setScore(long);
        void setNumLives(int);
        
        bool userControl;
        
        void printTypeName(void) 
         {
            cout << "Player";
         }
};


#endif
