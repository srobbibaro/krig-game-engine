#include "ObjectInclude.h"


//------------------------------------------------------------------------------
Beam::Beam()
: Model( NO_SCRIPT )
{
    load( MODEL_BEAM );  
    
    animCurrTime = NULL;
    
    life = 1;
}

//------------------------------------------------------------------------------
Beam::Beam( string tModelKey, string script, float* tTime )
: Model( script )
{ 
    load( tModelKey );  
    
    animCurrTime = tTime;
    
    life = 1;
}

//------------------------------------------------------------------------------
void Beam::handleCollision( Object* temp )
{
    if ( typeid( *temp ) == typeid( Player ) )
    {
         collision = false;
    }
    else if ( typeid( *temp ) == typeid( Terrain ) )
    {
    }
}

//------------------------------------------------------------------------------
void Beam::processAI( Camera* )
{     
}

