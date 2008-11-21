#include "ObjectInclude.h"

//------------------------------------------------------------------------------
PlayerMissle::PlayerMissle()
: Model( NO_SCRIPT )
{
    load( MODEL_RED_MISSLE );

    animCurrTime = NULL;
    player = NULL;
    obj = NULL;
    
    isEnemy = false;
    
    life = 1;   
    
    totalTime = 0.0f;
    speed = 0.0f;
}

//------------------------------------------------------------------------------
PlayerMissle::PlayerMissle( string tModelKey, float* tTime, Player *p, Object* tObj )
: Model( NO_SCRIPT )
{ 
    load( tModelKey );

    animCurrTime = tTime;
    player = p;
    obj = tObj;
    
    isEnemy = false;
    
    life = 1;
    
    totalTime = 0.0f;
    speed = 5.0f;       
}

//------------------------------------------------------------------------------
void PlayerMissle::handleCollision( Object* temp )
{
    if ( typeid( *temp ) == typeid(EnemyShip) || typeid( *temp ) == typeid( Boss ) ||
         typeid( *temp ) == typeid( Asteroid ) || typeid( *temp ) == typeid( Beam ) ||
         typeid( *temp ) == typeid(SailBoat) || typeid( *temp) == typeid(BoatCannon) )
    {
        life = 0;
        state = DEAD;
    }
}

//------------------------------------------------------------------------------
void PlayerMissle::handleDeath( void )
{
    player->numShots--;
}

//------------------------------------------------------------------------------
void PlayerMissle::processAI( Camera* c )
{
    if ( obj != NULL )
    {
        if ( obj->state != DEAD ) {
            totalTime += elapsedTime;
            
            if (totalTime >= 0.010f) {
                totalTime = 0.0f;
            
            Vector temp( obj->position.x - position.x, obj->position.y - position.y, obj->position.z - position.z);
            temp.normalize();
          //  rotationVelocity.x = temp.z;
         //   rotationVelocity.y = temp.x;
           // rotationVelocity.z = temp.y * 6.28f;
           // rotationVelocity.x = 0.0f; //temp.y * 5.0f;
           // rotationVelocity.y = temp.z * 6.28f;
            
                Vector rotationAxis;
            float rotationAngle;
        
            //temp.scale(100.0);
            velocity = temp;
            
            temp.normalize();
            rotationAxis.crossProduct( baseDirection, temp );        
            rotationAxis.normalize();
            rotationAngle = -acos(baseDirection.dotProduct( temp ));        

            Quaternion r2;
            r2.buildFromAxis( rotationAxis, rotationAngle );
            
            //rotation.slerp(rotation, f, r2);
            
            rotation = r2;
            //rotationVelocity.setVector(0.0f, 0.0f, 0.0f);
            
            rotationVelocity = rotationAxis;
            
            
                //speed += .25f;
            }
            //velocity = temp * ((totalTime) + 15.0f);
            
//            rotationVelocity = temp * 2.0f;
            //direction = temp;
            //rotationVelocity.x = temp.x;
            //;rotationVelocity.y = temp.y;
            //rotationVelocity.z = temp.z;
            
            //Vector j;
            //j.setVector(0.0f, 0.0f, 1.0f);
            
            //Vector rotationAxis;
//            temp.normalize();
            //rotationAxis.crossProduct(direction, temp);
            //rotationAxis.normalize();
//            temp.normalize();
//            rotation.buildFromAxis(j, temp.y * 5.0f );
//            cout << " temp " << direction.x << " " << direction.y << " " << direction.z << endl;
            
            //speed  = 10.0f;
            /*
            Vector temp2;
            Vector up ( 0.0f, 1.0f, 0.0f );
            Vector right( 0.0f, 0.0f, 1.0f );
            Vector rotationAxis;
            float rotationAngle;
        
            //temp.scale(100.0);
            velocity = temp;
            
            temp.normalize();
            rotationAxis.crossProduct( right, temp );        
            rotationAxis.normalize();
            rotationAngle = acos(right.dotProduct( temp ));        

            rotation.buildFromAxis( rotationAxis, 90.0f );
            
            
            rotationVelocity.x = direction.x * 8.0;
            rotationVelocity.y = direction.y * 8.0;
            rotationVelocity.z = direction.z * 8.0;
            
            //temp.scale(3.0);
            //velocity = temp;
            
            
            */
        } else {
            obj = NULL;
        }
    }
} 
