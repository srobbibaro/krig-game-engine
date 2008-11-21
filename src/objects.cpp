//////////////////////////////////////////////////////////////////
// Authors     : Steve Robbibaro     sar2                       //
//             : Andrew Yochum       amyst14                    //
//                                                              //
// Filename    : objects.cpp                                    //
//                                                              //
// Description : Functions displaying models and storing their  //
//               transformation data                            //
//                                                              //
// Date        : 12-1-03                                        //
//////////////////////////////////////////////////////////////////

#include "objects.h"

GLint numObjects;
Object* object;

//------------------------------------------------------------------------------
Object::Object() 
: ObjectNode()
{
    scaleFactor.setVector( 1.0f, 1.0f, 1.0f );
    scaleRate.setVector( 0.0f, 0.0f, 0.0f );
    
    state = NORMAL; 
    active = false;
    animStartTime = 0.0f;
    isVisible = true;
    s = NULL;
    collision = false;
    isEnemy = false;
       
    testActiveZoneEnable = true;
}

//------------------------------------------------------------------------------
Object::~Object()
{
    s = NULL;
}

 
//------------------------------------------------------------------------------
void Object::setScaleFactor( GLfloat x, GLfloat y, GLfloat z )
{ scaleFactor.setVector( x, y, z ); }

//------------------------------------------------------------------------------
void Object::setScaleFactor( const Vector &v )
{
    scaleFactor = v;
}

//------------------------------------------------------------------------------
void Object::setState( unsigned char tState )
{ state = tState; }

//------------------------------------------------------------------------------
void Object::setScaleRate( GLfloat x, GLfloat y, GLfloat z )
{
    scaleRate.setVector( x, y, z );
}

//------------------------------------------------------------------------------
void Object::setScaleRate( const Vector &v )
{
    scaleRate = v;
}

//------------------------------------------------------------------------------
void Object::setSoundClass ( Sound *snd )
{ s = snd; }

//------------------------------------------------------------------------------
void Object::drawObjects( Camera* c )
{
    if ( active == true && state != DEAD && isVisible == true )
        draw(c);
    
    if ( next != NULL )
        ((Object*)next)->drawObjects(c);
}
    
//------------------------------------------------------------------------------
void Object::drawObjectOutlines( Camera* c )
{
    if ( active == true && state != DEAD && isVisible == true)
        drawOutline( c );
    
    if ( next != NULL )
    {    
        ((Object*)next)->drawObjectOutlines(c);
    }
}

//------------------------------------------------------------------------------
void Object::drawShadows( Vector* l )
{
    if ( active == true && state != DEAD && isVisible == true )
        drawShadow( l );
    
    if ( next != NULL )
    {    
        ((Object*)next)->drawShadows(l);
    }
}
//------------------------------------------------------------------------------
void Object::updateObjects( Vector* light  )
{
    if ( active == true && state != DEAD )
        update( light );
    
    if ( next != NULL )
        ((Object*)next)->updateObjects( light );
        
    if ( state == DEAD )
    {
        remove();
        delete this;
    }
}

//------------------------------------------------------------------------------
void Object::processCollisions( Object* temp )
{
    Vector boxA[2];
    Vector boxB[2];  
		
    boxA[0] = collisionBox[0];
    boxA[1] = collisionBox[1];
    boxB[0].setVector(temp->collisionBox[0].x, temp->collisionBox[0].y, temp->collisionBox[0].z );
    boxB[1].setVector(temp->collisionBox[1].x, temp->collisionBox[1].y, temp->collisionBox[1].z );

    if ( (Object*)this != temp && active == true && state == NORMAL && (collision == true || temp->state != INVUL ) 
    && this != NULL && temp != NULL )
    {
        if(
            ((boxA[0].x >= boxB[0].x && boxA[0].x <= boxB[1].x) ||
            (boxA[1].x >= boxB[0].x && boxA[1].x <= boxB[1].x)  ||
            (boxB[0].x >= boxA[0].x && boxB[0].x <= boxA[1].x)  ||
            (boxB[1].x >= boxA[0].x && boxB[1].x <= boxA[1].x)) &&
            ((boxA[0].y >= boxB[0].y && boxA[0].y <= boxB[1].y) ||
            (boxA[1].y >= boxB[0].y && boxA[1].y <= boxB[1].y)  ||
            (boxB[0].y >= boxA[0].y && boxB[0].y <= boxA[1].y)  ||
            (boxB[1].y >= boxA[0].y && boxB[1].y <= boxA[1].y)) &&
            ((boxA[0].z >= boxB[0].z && boxA[0].z <= boxB[1].z) ||
            (boxA[1].z >= boxB[0].z && boxA[1].z <= boxB[1].z)  ||
            (boxB[0].z >= boxA[0].z && boxB[0].z <= boxA[1].z)  ||
            (boxB[1].z >= boxA[0].z && boxB[1].z <= boxA[1].z)) 
        )
        {
            handleCollision( temp );
            
            if ( next != NULL )
                ((Object*)next)->processCollisions(temp);
        }
    }

    if ( temp->next != NULL )
    {    
        processCollisions(((Object*)temp->next));
    }
}

//------------------------------------------------------------------------------
void Object::prepareObjects()
{
        if ( next != NULL )
        {
            ((Object*)next)->prepareObjects();
        }
        processCollisions( getRoot() );
}

//------------------------------------------------------------------------------
Object* Object::getRoot()
{
    Object* temp = this;

    while ( temp->prev != NULL ) {
        temp = (Object*)temp->prev;
    }
     
    return temp;
}


//------------------------------------------------------------------------------
void Object::showCollisionBox( void )
{
	glDisable( GL_CULL_FACE );
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
	glPushMatrix();
		
        glColor3f(1.0f, 0.0f, 0.0f);
			
        glBegin( GL_QUADS );
            
        glVertex3f( collisionBox[0].x, collisionBox[1].y, collisionBox[1].z );
        glVertex3f( collisionBox[0].x, collisionBox[0].y, collisionBox[1].z );
        glVertex3f( collisionBox[1].x, collisionBox[0].y, collisionBox[1].z );
        glVertex3f( collisionBox[1].x, collisionBox[1].y, collisionBox[1].z );
        
        glVertex3f( collisionBox[0].x, collisionBox[1].y, collisionBox[1].z );
        glVertex3f( collisionBox[0].x, collisionBox[1].y, collisionBox[0].z );
        glVertex3f( collisionBox[0].x, collisionBox[0].y, collisionBox[0].z );
        glVertex3f( collisionBox[0].x, collisionBox[0].y, collisionBox[1].z );
        
        glVertex3f( collisionBox[1].x, collisionBox[1].y, collisionBox[1].z );
        glVertex3f( collisionBox[1].x, collisionBox[0].y, collisionBox[1].z );
        glVertex3f( collisionBox[1].x, collisionBox[0].y, collisionBox[0].z );
        glVertex3f( collisionBox[1].x, collisionBox[1].y, collisionBox[0].z );
                
        glVertex3f( collisionBox[1].x, collisionBox[1].y, collisionBox[0].z );
        glVertex3f( collisionBox[1].x, collisionBox[0].y, collisionBox[0].z );
        glVertex3f( collisionBox[0].x, collisionBox[0].y, collisionBox[0].z );
        glVertex3f( collisionBox[0].x, collisionBox[1].y, collisionBox[0].z );
        
        glVertex3f( collisionBox[0].x, collisionBox[1].y, collisionBox[0].z );
        glVertex3f( collisionBox[0].x, collisionBox[1].y, collisionBox[1].z );
        glVertex3f( collisionBox[1].x, collisionBox[1].y, collisionBox[1].z );
        glVertex3f( collisionBox[1].x, collisionBox[1].y, collisionBox[0].z );

        glVertex3f( collisionBox[0].x, collisionBox[0].y, collisionBox[0].z );
        glVertex3f( collisionBox[1].x, collisionBox[0].y, collisionBox[0].z );
        glVertex3f( collisionBox[1].x, collisionBox[0].y, collisionBox[1].z );
        glVertex3f( collisionBox[0].x, collisionBox[0].y, collisionBox[1].z );
           
        glEnd();
     
    glPopMatrix();	
	glEnable( GL_CULL_FACE );
	glPolygonMode(GL_FRONT, GL_FILL);	
 
    if ( next != NULL )
        ((Object*)next)->showCollisionBox();
}

//------------------------------------------------------------------------------
void Object::showControlTriangle()
{
	glDisable( GL_CULL_FACE );
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
	glPushMatrix();
		
        glColor3f(1.0f, 0.0f, 0.0f);
        glBegin( GL_TRIANGLES );
            
        glVertex3f( controlPoints[0].x, controlPoints[0].y, controlPoints[0].z );
        glVertex3f( controlPoints[1].x, controlPoints[1].y, controlPoints[1].z );
        glVertex3f( controlPoints[2].x, controlPoints[2].y, controlPoints[2].z );
        glEnd();
     
    glPopMatrix();	
	glEnable( GL_CULL_FACE );
	glPolygonMode(GL_FRONT, GL_FILL);	
 
    if ( next != NULL )
        ((Object*)next)->showControlTriangle();
}

//------------------------------------------------------------------------------
void Object::animateObjects( float timeElapsed, Camera* c )
{   
    animate( timeElapsed, c );
     
    if ( next != NULL )
        ((Object*)next)->animateObjects( timeElapsed, c );
        
}

//------------------------------------------------------------------------------
float Object::calcTriangleCenter( void )
{
    float th1 = ( .5 )*controlPoints[0].y + (.5*controlPoints[1].y );
    float finalHeight = ( .5 )*th1 + ( .5 * controlPoints[2].y );
    
    return( finalHeight );
}

//------------------------------------------------------------------------------
bool Object::testActiveZone( Camera* c )
{   
   if ( !testActiveZoneEnable )
      return true;

    return ( 
        ( position.z > ( c->position.z - ACTIVE_VOLUME_LENGTH ) ) && 
        ( position.z < ( c->position.z + ACTIVE_VOLUME_LENGTH ) ) && // near-far
         
        ( position.x > ( c->position.x - ACTIVE_VOLUME_LENGTH ) ) && 
        ( position.x < ( c->position.x + ACTIVE_VOLUME_LENGTH ) ) && // left-right
                       
        ( position.y > ( c->position.y - ACTIVE_VOLUME_LENGTH ) ) &&  
        ( position.y < ( c->position.y + ACTIVE_VOLUME_LENGTH ) )    // up-down
    );
}

//------------------------------------------------------------------------------
bool Object::checkActiveEnemy( void )
{ return ( ( active == true ) && ( state != DEAD ) && ( isEnemy == true ) ); }

//------------------------------------------------------------------------------
void Object::decrementShots ( void )
{   //numShots--; 
}

//------------------------------------------------------------------------------
void Object::setShot( Object* obj, float tVelocity )
{ 
    Vector tv;
    bool hit = direction.intersectBox( position, collisionBox, 1.0f, tv );
            
    if ( hit ) {   
        obj->setPosition( tv );
    }
    else {
        obj->position = position;        
    }
            
    // still undecided about what velocity the shots should be given //
    //obj->velocity.x = ( direction.x * tVelocity );       
    //obj->velocity.y = ( direction.y * tVelocity );
    //obj->velocity.z = ( direction.z * tVelocity );
            
    obj->speed = tVelocity;
    ///////////////////////////////////////////////////////////////////
            
    obj->rotationVelocity.x = direction.x * 8.0;
    obj->rotationVelocity.y = direction.y * 8.0;
    obj->rotationVelocity.z = direction.z * 8.0;
            
    obj->setRotationQuaternion(rotation);             
}

//------------------------------------------------------------------------------
int Object::processExtendedCommand( const ScriptCommand &t )
{  
    int next = 1;
    /*
    //cout << "Extended Object Routine: " << t.routine << endl << endl;
    int index1, index2, index3, op;
   
    switch ( t.routine )
    {
        case SCRIPT_SET_SCALE_MEM:
            index1 = (int)t.p1;
            index2 = (int)t.p2;
            index3 = (int)t.p3;
            
            if ( index1 >= 0 && index1 < MEM_LEN &&
                 index2 >= 0 && index2 < MEM_LEN &&
                 index3 >= 0 && index3 < MEM_LEN )
             {
                scaleFactor.setVector( memVars[index1], memVars[index2], memVars[index3] );
             }
            break;
        case SCRIPT_SET_SCALE_RATE_MEM:
            index1 = (int)t.p1;
            index2 = (int)t.p2;
            index3 = (int)t.p3;
            
            if ( index1 >= 0 && index1 < MEM_LEN &&
                 index2 >= 0 && index2 < MEM_LEN &&
                 index3 >= 0 && index3 < MEM_LEN )
             {
                scaleRate.setVector( memVars[index1], memVars[index2], memVars[index3] );
             }
            break;
        case SCRIPT_ADD_SCALE_MEM:
            index1 = (int)t.p1;
            index2 = (int)t.p2;
            index3 = (int)t.p3;
            
            if ( index1 >= 0 && index1 < MEM_LEN &&
                 index2 >= 0 && index2 < MEM_LEN &&
                 index3 >= 0 && index3 < MEM_LEN )
             {
                scaleFactor.x += memVars[index1];
                scaleFactor.y += memVars[index2];
                scaleFactor.z += memVars[index3];
             }
            break;
        case SCRIPT_ADD_SCALE_RATE_MEM:
            index1 = (int)t.p1;
            index2 = (int)t.p2;
            index3 = (int)t.p3;
            
            if ( index1 >= 0 && index1 < MEM_LEN &&
                 index2 >= 0 && index2 < MEM_LEN &&
                 index3 >= 0 && index3 < MEM_LEN )
             {
                scaleRate.x += memVars[index1];
                scaleRate.y += memVars[index2];
                scaleRate.z += memVars[index3];
             }
            break;

        case SCRIPT_SET_SCALE_VAL:
            scaleFactor.setVector(t.p1, t.p2, t.p3);
            break;
        case SCRIPT_SET_SCALE_RATE_VAL:
            scaleRate.setVector(t.p1, t.p2, t.p3);
            break;
        case SCRIPT_ADD_SCALE_VAL:
            scaleFactor.x += t.p1;
            scaleFactor.y += t.p2;
            scaleFactor.z += t.p3;
            break;
        case SCRIPT_ADD_SCALE_RATE_VAL:
            scaleRate.x += t.p1;
            scaleRate.y += t.p2;
            scaleRate.z += t.p3;
            break;

        case SCRIPT_SET_TEST_ZONE_ENABLE_VAL:
            op = (int)t.p1;
            
            if (op == 0)
                testActiveZoneEnable = false;
            else
                testActiveZoneEnable = true;
            
            break;
        case SCRIPT_SET_STATE_VAL:
            state = (int)t.p1;
            break;
        case SCRIPT_STORE_STATE_IN_MEM:
            index1 = (int)t.p1;
            
            if ( index1 >= 0 && index1 < MEM_LEN ) {
                memVars[index1] = (float)state;
            }
            break;
        
        case SCRIPT_STORE_LIFE_IN_MEM:
            index1 = (int)t.p1;
            
            if ( index1 >= 0 && index1 < MEM_LEN ) {
                memVars[index1] = (float)life;
            }
            break;
        case SCRIPT_FIRE_WEAPON:
            fireShot();
            break;
        case SCRIPT_STORE_SCALE_IN_MEM:
            index1 = (int)t.p1;
            index2 = (int)t.p2;
            index3 = (int)t.p3;
            
            if ( index1 >= 0 && index1 < MEM_LEN &&
                 index2 >= 0 && index2 < MEM_LEN &&
                 index3 >= 0 && index3 < MEM_LEN )
             {
                    memVars[index1] = scaleFactor.x;
                    memVars[index2] = scaleFactor.y;
                    memVars[index3] = scaleFactor.z;
            }
            break;
        case SCRIPT_STORE_SCALE_RATE_IN_MEM:
            index1 = (int)t.p1;
            index2 = (int)t.p2;
            index3 = (int)t.p3;
            
            if ( index1 >= 0 && index1 < MEM_LEN &&
                 index2 >= 0 && index2 < MEM_LEN &&
                 index3 >= 0 && index3 < MEM_LEN )
             {
                    memVars[index1] = scaleRate.x;
                    memVars[index2] = scaleRate.y;
                    memVars[index3] = scaleRate.z;
            }
            break;
    }
    */
    
    return( next );
}


