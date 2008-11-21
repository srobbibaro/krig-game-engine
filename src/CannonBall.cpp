#include "ObjectInclude.h"

//------------------------------------------------------------------------------
CannonBall::CannonBall( string tModelKey, float* tTime )
: Model( NO_SCRIPT )
{ 
    load( tModelKey );
    
    animCurrTime = tTime;
    
    life = 1;
}

//------------------------------------------------------------------------------
void CannonBall::handleCollision( Object* temp )
{
    if ( typeid( *temp ) == typeid( Player ) )
    {
        temp->life--;     
    }
    else if ( typeid( *temp ) == typeid( Terrain ) )
    {
    }
}

//------------------------------------------------------------------------------
void CannonBall::processAI( Camera* )
{     
}

//------------------------------------------------------------------------------
void CannonBall::animate( float timeElapsed, Camera* c ) 
{
    if ( active == false )
    {
        if ( position.x < c->position.x + 30 && position.x > c->position.x - 30 )
            active = true;
    }
    else if ( active == true )
    {
        if ( position.x > c->position.x + 30 || position.x < c->position.x - 30 ||
             position.y > 100 || position.y <  -100 ||
             position.z > 100 || position.z < -100 )
        {
             state = DEAD;
        }
        else
        {
        
            processAI( c );

            // calculate new position and orientation //
            
            velocity.y = velocity.y + ( -10.0f*timeElapsed);
            
            position.x += velocity.x * timeElapsed;   
            position.y += velocity.y * timeElapsed;   
            position.z += velocity.z * timeElapsed;   
    
            //rotationAngle += rotVelocity * timeElapsed;
    
            scaleFactor.x += scaleRate.x * timeElapsed;
            scaleFactor.y += scaleRate.y * timeElapsed;
            scaleFactor.z += scaleRate.z * timeElapsed;
            /////////////////////////////////////////////

        }
    }
}
