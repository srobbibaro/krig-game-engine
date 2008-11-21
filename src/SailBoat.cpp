#include "ObjectInclude.h"

//------------------------------------------------------------------------------
SailBoat::SailBoat()
: Model( NO_SCRIPT )
{
    load( MODEL_SAIL_BOAT );  
    
    animCurrTime = NULL;
    
    life = 1;
    
    isEnemy = true;  
}

//------------------------------------------------------------------------------
SailBoat::SailBoat( string tModelKey, string script, float* tTime )
: Model( script )
{ 
    load( tModelKey );
    
    animCurrTime = tTime;
    
    life = 1;
    
    isEnemy = true;  
}

//------------------------------------------------------------------------------
void SailBoat::handleCollision( Object* temp )
{
    if ( typeid( *temp ) == typeid( Player ) )
    {
          
    }
    else if ( typeid( *temp ) == typeid( Terrain ) )
    {
        controlPoints[0].y = ((Terrain*)temp)->getHeight(controlPoints[0].x, controlPoints[0].z );
        controlPoints[1].y = ((Terrain*)temp)->getHeight(controlPoints[1].x, controlPoints[1].z );        
        controlPoints[2].y = ((Terrain*)temp)->getHeight(controlPoints[2].x, controlPoints[2].z );
        
        float height = ((Terrain*)temp)->getHeight( position.x, position.z ); //calcTriangleCenter();
        
        if ( position.y < ((Terrain*)temp)->getHeight( position.x, position.z ) )
            {
                Vector normal;
                normal.calcNorm( controlPoints[0], controlPoints[1], controlPoints[2] );

                Vector up ( 0.0f, 1.0f, 0.0f );
                //rotationAxis.crossProduct( up, normal );        
        
                //rotationAngle = -normal.dotProduct( up );
                
                position.y = height;
            }
    }
}

//------------------------------------------------------------------------------
void SailBoat::processAI( Camera* )
{   
}

