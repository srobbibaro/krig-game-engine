#include "Explosion.h"

//------------------------------------------------------------------------------
Explosion::Explosion()
: Model( NO_SCRIPT )
{
    load( MODEL_EXPLOSION );
    
    duration = 5.0;
    state = INVUL;
    isVisible = true;
    active = true;

    scaleRate.setVector( 1.0f, 1.0f, 1.0f );
    
    animCurrTime = NULL;
    animStartTime = 0.0f;

    life = 1;
}

//------------------------------------------------------------------------------
Explosion::Explosion( float dur, float* tTime )
: Model( NO_SCRIPT )
{
    load( MODEL_EXPLOSION );
    
    duration = dur;
    state = INVUL;
    isVisible = true;
    active = true;

    scaleRate.setVector( 1.0f, 1.0f, 1.0f );
    
    animCurrTime = tTime;
    animStartTime = *tTime;

    life = 1;
}

//------------------------------------------------------------------------------
void Explosion::animate ( float timeElapsed, Camera *c )
{ 
    if ( *animCurrTime > ( animStartTime + duration ) )
        state = DEAD;

    if ( active == false )
    {
        if ( testActiveZone( c ) == true  ) //position.x < c->position.x + 30 && position.x > c->position.x - 30 )
            active = true;
    }
    else if ( active == true )
    {
        //if ( position.x > c->position.x + 30 || position.x < c->position.x - 30 ||
         //    position.y > 100 || position.y <  -100 ||
         //    position.z > 100 || position.z < -100 )
        if ( testActiveZone(c) == false )
        {
             state = DEAD;
        }
        else
        {
            processAI( c, timeElapsed );

            // calculate new position and orientation //
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
