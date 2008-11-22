#include "ObjectInclude.h"

//------------------------------------------------------------------------------
Boss::Boss()
: Model( NO_SCRIPT )
{
    load( MODEL_BOSS_A );
    
    animCurrTime = NULL;
    
    life = 40;
    
    isEnemy = true;  
    
    shotDelay = 0.5f;
    numShots = 1;   
    
    complete = NULL;
}

//------------------------------------------------------------------------------
Boss::Boss( string tModelKey, string script, float* tTime, Player* p, bool* c )
: Model( script )
{ 
    load( tModelKey );
    
    animCurrTime = tTime;
    
    life = 40;
    
    isEnemy = true;  
    
    shotDelay = 0.5f;
    numShots = 1;   
    
    complete = c;
}

//------------------------------------------------------------------------------
Boss::~Boss()
{
    complete = NULL;
    obj = NULL;
}

//------------------------------------------------------------------------------
void Boss::handleCollision( Object* temp )
{
    if ( typeid( *temp ) == typeid( PlayerShot ) )
    {
        life-=1;
        if ( life <= 0 )
        {
            state = DEAD;
            //player->score += 1000;
            s->PlaySFX(5);
            
            obj = new Explosion ( 5.0f, animCurrTime );
            obj->position = position;
            obj->setVelocity(0.0f, 0.0f, 0.0f);
            add( obj );
            *complete = true;
        }
        else
            s->PlaySFX(3);
    }
    else if ( typeid( *temp ) == typeid( PlayerMissle ) )
    {
        life-=1;
        if ( life <= 0 )
        {
            state = DEAD;
            //player->score += 1000;
            s->PlaySFX(5);
            
            obj = new Explosion ( 5.0f, animCurrTime );
            obj->position = position;
            obj->setVelocity(0.0f, 0.0f, 0.0f);
            add( obj );
            *complete = true;
        }
        else
            s->PlaySFX(3);
    }
    
    else if ( typeid( *temp ) == typeid( Terrain ) )
    {
        controlPoints[0].y = ((Terrain*)temp)->getHeight(controlPoints[0].x, controlPoints[0].z );
        controlPoints[1].y = ((Terrain*)temp)->getHeight(controlPoints[1].x, controlPoints[1].z );        
        controlPoints[2].y = ((Terrain*)temp)->getHeight(controlPoints[2].x, controlPoints[2].z );
        
        
        if ( position.y < controlPoints[0].y || position.y < controlPoints[1].y || 
             position.y < controlPoints[2].y )
            {
                Vector normal, up;
                normal.calcNorm( controlPoints[0], controlPoints[1], controlPoints[2] );
                up.setVector( 0.0f, 1.0f, 0.0f );
                //rotationAxis.crossProduct( up, normal );
                
                //rotationAngle = -normal.dotProduct( up );
                position.y = ((Terrain*)temp)->getHeight( position.x, position.z );
            }
    }
}

//------------------------------------------------------------------------------
void Boss::processAI( Camera* c )
{
    int r;   
    float tx, tz, rot;
    
    // give the boss more shots, if he has waited long enough
    if ( numShots < 1 ) {
        if ( *animCurrTime > shotTime + shotDelay ) {
            numShots++;
        }
    }
      
    // if shots are available, fire a shot 
    if ( numShots > 0 ) {
    /*
        numShots--;
        shotTime = *animCurrTime;         

        obj = new Shot( MODEL_GREEN_SHOT, animCurrTime, this);
        add(obj);
            
        float speed = velocity.getScaler(direction);
        setShot( obj, (speed + SHOT_SPEED) );
        
        obj->scaleFactor = scaleFactor;
        obj->position.y= (collisionBox[0].y + 0.9);
   
            
        obj = new Shot(MODEL_GREEN_SHOT, animCurrTime, this);
        add(obj);
             
       speed = velocity.getScaler(direction);
       
        setShot( obj, (speed + SHOT_SPEED) );
        
        obj->scaleFactor = scaleFactor;
        obj->position.y=(collisionBox[1].y - 0.9);
        
        
        s->PlaySFX(0);
        */
     }
     
   animateScript(elapsedTime);
}

//------------------------------------------------------------------------------
void Boss::decrementShots ( void )
{   //numShots--;    
}
