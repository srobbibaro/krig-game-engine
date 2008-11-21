#include "ObjectInclude.h"

//------------------------------------------------------------------------------
Shot::Shot()
: Model( NO_SCRIPT )
{
    load( MODEL_GREEN_SHOT );  
    
    animCurrTime = NULL;
    
    life = 1;
    
    owner = NULL;
}

//------------------------------------------------------------------------------
Shot::Shot( string tModelKey, float* tTime, Object *obj )
: Model( NO_SCRIPT )
{ 
   load( tModelKey );  
    
    animCurrTime = tTime;
    
    life = 1;
    
    owner = obj;
}

//------------------------------------------------------------------------------
Shot::~Shot()
{
    owner = NULL;
}

//------------------------------------------------------------------------------
void Shot::handleCollision( Object* temp )
{
    if ( typeid( *temp ) == typeid( Player ) ) {
        life--;
        if (life < 0)
            state = DEAD;    
            
        collision = false; 
    }
}

//------------------------------------------------------------------------------
void Shot::handleDeath( void )
{
    if ( owner != NULL )
        owner->decrementShots();
}
