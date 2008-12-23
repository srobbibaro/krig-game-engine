#include "model.h"

#ifndef _PLAYER_H_
#define _PLAYER_H_

class Player : public Model
{
    public:
    //private:
                
   // public:
        //void handleCollision( Object* );
        
        Player();
        virtual ~Player();
        
        bool userControl;
        
        void printTypeName(void) 
         {
            cout << "Player";
         }
};


#endif
