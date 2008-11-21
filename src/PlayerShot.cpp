#include "ObjectInclude.h"

//------------------------------------------------------------------------------
PlayerShot::PlayerShot()
: Model( NO_SCRIPT )
{
    load( MODEL_BLUE_SHOT );

    animCurrTime = NULL;
    player = NULL;
    
    life = 1;
}

//------------------------------------------------------------------------------
PlayerShot::PlayerShot( string tModelKey, float* tTime, Player *p )
: Model( NO_SCRIPT )
{ 
    load( tModelKey );
    
    animCurrTime = tTime;
    player = p;
    
    life = 1;
}

//------------------------------------------------------------------------------
PlayerShot::~PlayerShot()
{
    player = NULL;
}

//------------------------------------------------------------------------------
void PlayerShot::handleCollision( Object* temp )
{
    if ( typeid( *temp ) == typeid(EnemyShip) || typeid( *temp ) == typeid( Boss ) ||
         typeid( *temp ) == typeid( Asteroid ) || typeid( *temp ) == typeid( Beam ))
    {
        life--;
        state = DEAD;

        if (player != NULL) {
            if (player->numShots < 3)
                player->numShots++;
        }
    }
}

//------------------------------------------------------------------------------
void PlayerShot::handleDeath( void )
{
    if ( numShots < 3 )
        player->numShots++;
}
