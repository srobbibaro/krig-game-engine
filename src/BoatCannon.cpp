#include "ObjectInclude.h"

//------------------------------------------------------------------------------
BoatCannon::BoatCannon()
: Model( NO_SCRIPT )
{
    load( MODEL_BOAT_CANNON );  
    
    animCurrTime = NULL;
    
    life = 2;
    
    player = NULL;
    isEnemy = true;  
    
    shotDelay = 1.75f;
    numShots = 1; 
}

//------------------------------------------------------------------------------
BoatCannon::BoatCannon( string tModelKey, string script, float* tTime, Object* p )
: Model( script )
{ 
    load( tModelKey );  
    
    animCurrTime = tTime;
    
    life = 2;
    
    player = p;
    isEnemy = true;  
    
    shotDelay = 1.75f;
    numShots = 1;   
}

//------------------------------------------------------------------------------
void BoatCannon::handleCollision( Object* temp )
{
    if ( typeid( *temp ) == typeid( Terrain ) ) {
        controlPoints[0].y = ((Terrain*)temp)->getHeight(controlPoints[0].x, controlPoints[0].z );
        controlPoints[1].y = ((Terrain*)temp)->getHeight(controlPoints[1].x, controlPoints[1].z );        
        controlPoints[2].y = ((Terrain*)temp)->getHeight(controlPoints[2].x, controlPoints[2].z );
        
        float height = calcTriangleCenter();
        
        if ( position.y < ((Terrain*)temp)->getHeight( position.x, position.z ) ) {
                Vector up;
                up.setVector( 0.0f, 1.0f, 0.0f );
                normal.calcNorm( controlPoints[0], controlPoints[1], controlPoints[2] );
                normal.normalize();
                //normal.scale(1.57);
                Vector rotationAxis;
                rotationAxis.crossProduct( up, normal );
                rotationAxis.normalize();
                float rotationAngle = -normal.dotProduct( up );
                rotation.buildFromAxis(rotationAxis, rotationAngle);
                //Quaternion tempQ;
                //tempQ.buildFromEuler(normal);
                
                //Quaternion tempQ2 = rotation;
                
                //rotation = tempQ;// * tempQ2;
                
                
                
                
                
                position.y = height;
        }
    }
}

//------------------------------------------------------------------------------
void BoatCannon::processAI( Camera* )
{
    if ( numShots < 1 ) {
        if ( *animCurrTime > ( shotTime + shotDelay ) )
            numShots++;
    }
     
     position.x;
     player->position.x;
     
    //if ( position.x < player->position.x + 50 && 
    //     position.x > player->position.x - 50 )
    {
        if ( numShots > 0 ) {
            numShots--;
            shotTime = *animCurrTime;      
            
            obj = new CannonBall( MODEL_CANNON_BALL, animCurrTime );
            
            Vector cannon(0.0, .71, .71 );
            
            Vector tempV;
    
            GLfloat temp;
            
            Matrix transform;    
            // setup transformation matrices ////////////
            //transform.setRotation( rotationAngle, rotationAxis.x, rotationAxis.y, rotationAxis.z );
    
            tempV.rotateVector( transform, cannon );
            tempV.normalize();
            
            tempV.scale(25);
        

            obj->setPosition( position.x, position.y, position.z );
            obj->setVelocity( tempV.x, tempV.y, tempV.z );
            //obj->setRotationAxis( 0.0, 0.0, 1.0, 0.0 );
            obj->setRotationVelocity( 8.0f, 8.0f, 8.0f );
            add( obj );
        }
     }
}
//------------------------------------------------------------------------------
