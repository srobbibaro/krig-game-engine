#include "ObjectInclude.h"

//------------------------------------------------------------------------------
Player::Player( string tModelKey )
: Model(  )
{ 
    load( tModelKey );

    active = true;
    state = NORMAL;
    testActiveZoneEnable = false;
    
    shotDelay = .75f;
    numShots = 3;
    numMissiles = 300;
    
    score = 0;
    numLives = 3;
    pShot = NULL;
    
    life = 10;
    
    userControl = true;
}

//------------------------------------------------------------------------------
Player::~Player()
{
    pShot = NULL;
}

//------------------------------------------------------------------------------
void Player::handleCollision( Object* temp )
{
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
        
        s->PlaySFX(3);
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
        
        s->PlaySFX(3);
    
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
}

//------------------------------------------------------------------------------
void Player::processAI( Camera* c)
{
   // I'd like to redo this logic which restricts the player from moving off screen
   // to be a little smarter depending on where the camera is located...
   // as it is now, it is hardcored for a specific distance 

    if (testActiveZoneEnable) {
        if (position.x < c->position.x-13.0f)
            position.x = c->position.x-13.0f;
        else if (position.x > c->position.x+13.0f)
            position.x = c->position.x+13.0f;
    
        if (position.y < c->position.y-10.0f)
            position.y = c->position.y-10.0f;
        else if (position.y > c->position.y+10.0f)
            position.y = c->position.y+10.0f;
    }
        
    if ( numShots < 1 ) {
        if ( *animCurrTime > shotTime + shotDelay )
            numShots = 3;
    }
   
    // this needs to be fixed to elapsedTime
    animateScript(0.0f);
}

//------------------------------------------------------------------------------
void Player::animate( float timeElapsed, Camera* c )
{
    if ( state == INVUL )
    {
        isVisible = !isVisible;
        
        if ( *animCurrTime > ( animStartTime + INVUL_DURATION ) )
        {
            state = NORMAL;
            isVisible = true;
        }
    }
     
         // calculate new position and orientation //
            if ( speed == 0 ) {
                position.x += velocity.x * timeElapsed;   
                position.y += velocity.y * timeElapsed;   
                position.z += velocity.z * timeElapsed;   
            }
            else {
                switch (speedDir)
                {
                    case 0:
                        direction.scale(speed*timeElapsed);
                        position.x += direction.x;
                        position.y += direction.y;
                        position.z += direction.z;
                        direction.normalize();
                        break;
                    case 1:
                    {
                        Vector rotationAxis;
            
                        rotationAxis.crossProduct(up, direction);
                        rotationAxis.normalize();
                        
                        rotationAxis.scale(speed*timeElapsed);
                        position.x += rotationAxis.x;
                        position.y += rotationAxis.y;
                        position.z += rotationAxis.z;
                        
                        break;
                    }
                    case 2:
                        up.scale(speed*timeElapsed);
                        position.x += up.x;
                        position.y += up.y;
                        position.z += up.z;
                        direction.normalize();
                        break;
                }
            }
    
    scaleFactor.x += scaleRate.x * timeElapsed;
    scaleFactor.y += scaleRate.y * timeElapsed;
    scaleFactor.z += scaleRate.z * timeElapsed;
    /////////////////////////////////////////////
    
    if (!interp) {
                if ( 
                    rotationVelocity.x != 0.0f ||
                    rotationVelocity.y != 0.0f ||
                    rotationVelocity.z != 0.0f 
                ) {
                    Vector tempV;
                    Quaternion tempQ;
	
                    tempV.x = rotationVelocity.x * timeElapsed;
                    tempV.y = rotationVelocity.y * timeElapsed;
                    tempV.z = rotationVelocity.z * timeElapsed;
	             
                    tempQ.buildFromEuler(tempV);	
                    rotation = rotation * tempQ;
                }
            }
            else {
                float endVal = valInterpEnd - valInterpStart;
                float curVal = *valInterpPtr - valInterpStart;
              
                float t = 0.0f;
                                              
                if ( endVal > 0 ) {
                    if ( curVal > endVal )
                        t = 1.0f;
                    else if ( curVal < 0.0f )
                        t = 0.0f;
                    else
                        t = curVal / endVal; 
                }
                else if ( endVal < 0 ) {
                    if ( curVal < endVal )
                        t = 1.0f;
                    else if ( curVal > 0.0f )
                        t = 0.0f;
                    else
                        t = curVal / endVal; 
                }
                                
                rotation.slerp(rInterpStart, t, rInterpEnd );
            }
            /////////////////////////////////////////////
        
    
    processAI(c);
}

//------------------------------------------------------------------------------
void Player::fireShot( float gameSpeed )
{   
    if ( numShots > 0  ) {          
        numShots--;
        shotTime = *animCurrTime;
            
        pShot = new PlayerShot( MODEL_BLUE_SHOT, animCurrTime, this );
        float speed = velocity.getScaler(direction);
        setShot( pShot, (speed + SHOT_SPEED) );
        
        add( pShot );
            
        s->PlaySFX(0);          
    }
}

//------------------------------------------------------------------------------
void Player::fireMissle( Object* tobj )
{
    if ( numMissiles > 0 )
    {
        numMissiles--;

        pShot = new PlayerMissle( MODEL_RED_MISSLE, animCurrTime, this, tobj );
        float speed = velocity.getScaler(direction);
        //cout << "pl speed: " << speed;
        setShot( pShot, (speed + SHOT_SPEED) );
        
        //pShot->setVelocity( 25.0f, 0.0f, 0.0f );
        //pShot->setPosition( position.x + 2, position.y, position.z );
        add(pShot);
    }
}

//------------------------------------------------------------------------------
long Player::getScore()
{   return score; }
//------------------------------------------------------------------------------
int Player::getNumLives()
{ return numLives; }
//------------------------------------------------------------------------------
void Player::setScore( long tscore )
{ score = tscore; }
//------------------------------------------------------------------------------
void Player::setNumLives( int tNumLives )
{ numLives = tNumLives; }
