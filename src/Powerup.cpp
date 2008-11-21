#include "ObjectInclude.h"

using namespace std;

//------------------------------------------------------------------------------
Powerup::Powerup()
: Model( NO_SCRIPT )
{
   type = 1;
    
   load( MODEL_POWERUP_1 );
    
    state = NORMAL;
//    isVisible = true;
//    active = true;
    collision = true;

    animCurrTime = NULL;
    animStartTime = 0.0f;
    
    life = 1;   
}

//------------------------------------------------------------------------------
Powerup::Powerup( int tType, float* tTime )
: Model( NO_SCRIPT )
{
    type = tType;
    
    if ( type == 1 ) {
        load( MODEL_POWERUP_1 );
    } 
    else if ( type == 2 ) {
        load( MODEL_POWERUP_2 );
    }   
    
    state = NORMAL;
//    isVisible = true;
//    active = true;
    collision = true;

    animCurrTime = tTime;
    animStartTime = *tTime;
    
    life = 1;   
}

//------------------------------------------------------------------------------
void Powerup::handleCollision( Object* temp )
{
    if ( typeid( *temp ) == typeid( Player ) ) {
        state = DEAD;
        
        if ( type == 1 ) {
            temp->numMissiles += 5;
            if ( temp->numMissiles > 30 )
                temp->numMissiles  = 30;
        }
        else if ( type == 2 ){
            temp->numShots = 25;   
        }
    }
}
