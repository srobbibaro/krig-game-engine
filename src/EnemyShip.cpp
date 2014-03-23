#include "ObjectInclude.h"

//------------------------------------------------------------------------------
EnemyShip::EnemyShip()
: Model( NO_SCRIPT )
{
    load( MODEL_ENEMY_SHIP );

    animCurrTime = NULL;

    life = 1;

    player = NULL;
    isEnemy = true;

    shotDelay = 1.0f;
    numShots = 1;
}

//------------------------------------------------------------------------------
EnemyShip::EnemyShip( string tModelKey, string script, float* tTime, Player* p )
: Model( script )
{
    load( tModelKey );

    animCurrTime = tTime;

    life = 1;

    player = p;
    isEnemy = true;

    shotDelay = 1.0f;
    numShots = 1;
}

//------------------------------------------------------------------------------
void EnemyShip::handleCollision( Object* temp )
{
    if ( typeid( *temp ) == typeid( Player ) || typeid( *temp ) == typeid( PlayerShot ) )
    {
        life--;
        if ( life <= 0 )
        {
           state = DEAD;
            if ( player != NULL )
                player->score += 100;

            s->PlaySFX("explosion1.wav");


            if ( rand() % 100 > 50 ) {
                if ( rand() % 3 > 1 )
                    obj = new Powerup( 2, animCurrTime );
                else
                    obj = new Powerup( 1, animCurrTime );

                //obj->setRotationAxis( 0.0, 1.0f, 0.0, 0.0f );
                //obj->setRotVelocity( 3.0f );
            }
            else {
                obj = new Explosion ( 2.0f, animCurrTime );

            }

            obj->position = position;
            obj->setVelocity(0.0f, 0.0f, 0.0f);
            add( obj );

            collision = false;
        }
    }
    else if ( typeid( *temp ) == typeid( PlayerMissle ) )
    {
        life-=2;
        if ( life <= 0 )
        {
            state = DEAD;
            if ( player != NULL )
                player->score += 100;
            s->PlaySFX("explosion1.wav");
            obj = new Explosion ( 2.0f, animCurrTime );
            obj->position = position;
            obj->setVelocity(0.0f, 0.0f, 0.0f);
            add( obj );
        }

        collision = false;
    }

    else if ( typeid( *temp ) == typeid( Terrain ) )
    {
        controlPoints[0].y = ((Terrain*)temp)->getHeight(controlPoints[0].x, controlPoints[0].z );
        controlPoints[1].y = ((Terrain*)temp)->getHeight(controlPoints[1].x, controlPoints[1].z );
        controlPoints[2].y = ((Terrain*)temp)->getHeight(controlPoints[2].x, controlPoints[2].z );


        if ( position.y < controlPoints[0].y || position.y < controlPoints[1].y ||
             position.y < controlPoints[2].y )
            {
                position.y = ((Terrain*)temp)->getHeight( position.x, position.z );

                life--;
                if ( life <= 0 )
                {
                    state = DEAD;

                    s->PlaySFX("explosion1.wav");
                    obj = new Explosion ( 2.0f, animCurrTime );
                    obj->position = position;
                    obj->setVelocity(0.0f, 0.0f, 0.0f);
                    add( obj );
                }
            }
    }
}

//------------------------------------------------------------------------------
void EnemyShip::processAI( Camera*, float elapsedTime )
{
    if ( state != DEAD ) {
        if ( numShots < 1 ) {
            if ( *animCurrTime > shotTime + shotDelay )
                numShots++;
        }

        if ( direction.intersectBox( position, player->collisionBox, 1.0 ) ) {
            if ( numShots > 0 ) {
                //fireShot();
            }
        }
     }
}

//------------------------------------------------------------------------------
void EnemyShip::fireShot()
{
    numShots--;
    shotTime = *animCurrTime;

    obj = new Shot( MODEL_GREEN_SHOT, animCurrTime, this );
    add( obj );
    //obj->setPosition( position.x -2, position.y, position.z );
    //obj->setVelocity( velocity.x - 7.0, 0.0f, 0.0f );
    float speed = velocity.getScaler(direction);

    PRINT_DEBUG_LVL(5, "es speed: %f\n", speed);
    setShot( obj, (speed + SHOT_SPEED) );

    s->PlaySFX("laser.wav");
}

