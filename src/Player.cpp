#include "ObjectInclude.h"

//------------------------------------------------------------------------------
Player::Player()
: Model()
{
    // TODO: Investigate whether this class is still used
    //shotDelay = .75f;
    //numShots = 3;
    //numMissiles = 300;

    //score = 0;
}

//------------------------------------------------------------------------------
Player::~Player() {}

//------------------------------------------------------------------------------
//void Player::handleCollision( Object* temp )
//{
/*
    if ( typeid( *temp ) == typeid( EnemyShip ) || typeid( *temp ) == typeid( Shot )
        || typeid( *temp ) == typeid( Asteroid ) || typeid( *temp ) == typeid( Beam )
        || typeid(*temp) == typeid(CannonBall) )
    {
        life--;
        temp->collision = true;

        animStartTime = *animCurrTime;
        state = INVUL;

        if ( life < 1 ) {
            numLives--;
            life = 10;
        }

        s->PlaySFX("explosion1.wav");
    }
    else if ( typeid ( *temp ) == typeid( Boss ) ) {
        animStartTime = *animCurrTime;
        state = INVUL;
        temp->collision = true;

        life-=3;
        if ( life < 1 )
        {
            numLives--;
            life = 10;
        }

        s->PlaySFX("explosion1.wav");

    }
    else if ( typeid( *temp ) == typeid( Powerup ) ) {

    }
    else if ( typeid( *temp ) == typeid( Terrain ) )
    {
        controlPoints[0].y = ((Terrain*)temp)->getHeight(controlPoints[0].x, controlPoints[0].z );
        controlPoints[1].y = ((Terrain*)temp)->getHeight(controlPoints[1].x, controlPoints[1].z );
        controlPoints[2].y = ((Terrain*)temp)->getHeight(controlPoints[2].x, controlPoints[2].z );

        float height = calcTriangleCenter();

        if ( position.y < ((Terrain*)temp)->getHeight( position.x, position.z ) )
            {
                Vector normal, up;
                normal.calcNorm( controlPoints[0], controlPoints[1], controlPoints[2] );

                up.setVector( 0.0f, 1.0f, 0.0f );
                //rotationAxis.crossProduct( up, normal );

                //rotationAngle = -normal.dotProduct( up );
                position.y = height;
            }
    }
*/
//}

//------------------------------------------------------------------------------
/*
void Player::processAI( Object* c, float elapsedTime)
{
   // I'd like to redo this logic which restricts the player from moving off screen
   // to be a little smarter depending on where the camera is located...
   // as it is now, it is hardcored for a specific distance

    //if (testActiveZoneEnable) {
    {
        if (position.x < c->position.x-13.0f)
            position.x = c->position.x-13.0f;
        else if (position.x > c->position.x+13.0f)
            position.x = c->position.x+13.0f;

        if (position.y < c->position.y-10.0f)
            position.y = c->position.y-10.0f;
        else if (position.y > c->position.y+10.0f)
            position.y = c->position.y+10.0f;
    }
 */
    /*
    if ( numShots < 1 ) {
        if ( *animCurrTime > shotTime + shotDelay )
            numShots = 3;
    }
    */
    /*

    // this needs to be fixed to elapsedTime
    animateScript(elapsedTime);

     if (particleSystem != NULL)
        particleSystem->update(elapsedTime);
}
*/

/*
//------------------------------------------------------------------------------
void Player::fireMissle( Object* tobj )
{
    if ( numMissiles > 0 )
    {
        numMissiles--;

        pShot = new PlayerMissle( MODEL_RED_MISSLE, animCurrTime, this, tobj );
        float speed = velocity.getScaler(direction);
        PRINT_DEBUG("pl speed: %f\n", speed);
        setShot( pShot, (speed + SHOT_SPEED) );

        //pShot->setVelocity( 25.0f, 0.0f, 0.0f );
        //pShot->setPosition( position.x + 2, position.y, position.z );
        add(pShot);
    }
}
*/
