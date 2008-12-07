#include "ObjectInclude.h"

//------------------------------------------------------------------------------
Asteroid::Asteroid()
: Model( NO_SCRIPT )
{
    load( MODEL_ASTEROID );  
    
    animCurrTime = NULL;
    
    life = 3;
}

//------------------------------------------------------------------------------
Asteroid::Asteroid( string tModelKey, string script, float* tTime )
: Model( script )
{ 
    load( tModelKey );
      
    animCurrTime = tTime;
    
    life = 3;
}

//------------------------------------------------------------------------------
void Asteroid::handleCollision( Object* temp )
{
    if ( typeid( *temp ) == typeid( Player ) || typeid( *temp ) == typeid( PlayerShot ) || typeid( *temp ) == typeid( PlayerMissle ) )
    {
        s->PlaySFX("explosion1.wav");
        life--;
        if ( life < 1)
            state = DEAD;
    }

    collision = false;
}
