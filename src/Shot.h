#include "Model.h"

#ifndef _SHOT_H_
#define _SHOT_H_

class Shot : public Model
{
    private:
        Object *owner;
                
    public:
        void handleCollision( Object* );
        void prepare() {}
        void handleDeath( void );
        
        Shot();
        ~Shot();
        Shot( string, float*, Object* );
        void processAI( Camera*, float elapsedTime ) {}
        
        void printTypeName(void) 
         {
            cout << "Shot";
         }
};
#endif
