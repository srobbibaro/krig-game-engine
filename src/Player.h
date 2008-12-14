#include "model.h"

#ifndef _PLAYER_H_
#define _PLAYER_H_

class Player : public Model
{
    public:
    //private:
                
   // public:
        //void handleCollision( Object* );
        void prepare() {}
        
        Player();
        virtual ~Player();
        void processAI( Object*, float ); // Camera*
        void animate( float, Camera* );
        void fireShot ( float );
        void fireMissle( Object* tobj );
        
        bool userControl;
        
        void printTypeName(void) 
         {
            cout << "Player";
         }
};


#endif
