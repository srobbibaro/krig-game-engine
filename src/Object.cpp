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

#include "Object.h"
#include "Scripting.h"
#include "Snow.h"

GLint numObjects;
Object* object;

//------------------------------------------------------------------------------
Object::Object() 
: ObjectNode()
{
    scale.setVector( 1.0f, 1.0f, 1.0f );
    scaleRate.setVector( 0.0f, 0.0f, 0.0f );
    
    velocity.setVector(0.0f, 0.0f, 0.0f);
    position.setVector(0.0f, 0.0f, 0.0f);
    setRotationEuler(0.0f, 0.0f, 0.0f);
    
    collisionBox[0].setVector( 0.0f, 0.0f, 0.0f );
    collisionBox[1].setVector( 0.0f, 0.0f, 0.0f );
    
    position.setVector(0.0f, 0.0f, 0.0f);
    
    state = NORMAL; 
    active = false;

    isVisible = true;
    s = NULL;
    collision = false;
       
    //#testActiveZoneEnable = true;
    
    particleSystem = NULL;
    L = NULL;
    
    typeId = 0;
    
    init();
}

//------------------------------------------------------------------------------
Object::~Object()
{
    s = NULL;
    
     if (L != NULL)
        lua_close(L);
        
    if (particleSystem != NULL)
        delete particleSystem;
    
    particleSystem = NULL;
}

void Object::init(void)
{
    rotation.loadMultIdentity();
    baseDirection.setVector( 0.0f, 0.0f, 1.0f );
    direction.setVector( 0.0f, 0.0f, 1.0f );
    up.setVector(0.0f, 1.0f, 0.0f);
          
    speed = 0.0f;
    speedDir = 0;
    
    velocity.setVector(0.0f, 0.0f, 0.0f);
            
    rInterpStart.loadMultIdentity();
    rInterpEnd.loadMultIdentity();
    
    tieMemVarIndex = 0;
    valInterpStart = 0.0f;
    valInterpEnd = 0.0f;
    interp = false;
        
    //scriptLastTime = 0.0f;
    //scriptNum = NO_SCRIPT;
    //scriptName = NO_SCRIPT;
    //currentScriptCommand = 0;
    
    //#animCurrTime = NULL;  
    
    L = NULL;
    
    suspend = false;
    suspendTime = 0.0f;
    
    if (particleSystem != NULL)
        delete particleSystem;
    
    particleSystem = NULL;
}

void Object::loadScript(string name)
{
    scriptName = name;
    
    // If the lua state has not been initialized for this object, attempt to
    // initialize it.     
    if (scriptName == "" || L != NULL)
        return;
        
    L = lua_open();
    
    // If the lua state still could not be initialized, then exit the game.
    // ... we can do something smarter with this in the finished product.
    if (L == NULL) {
        printf("Error creating Lua state.\n");
	    exit(-1);
    }

    // Load the base lua libraries
    luaL_openlibs(L);
        
    // Register our functions for use in lua (currently defined in 
    // Object.h)
    registerFunctions(L, 2);
        
    // Load this object's animation script
    luaL_dofile(L, scriptName.c_str());
        
    // Find the update function and call it
    lua_getglobal(L, "on_load");
	    
    // Push a pointer to the current object for use within the lua function
    lua_pushlightuserdata(L, (void*)this);
	   
    // Call the function with 1 argument and no return values
    lua_call(L, 1, 0);

    // get the result //
    //position.z = (float)lua_tonumber(L, -1);
    //position.y = (float)lua_tonumber(L, -2);
    //position.x = (float)lua_tonumber(L, -3);
    //lua_pop(L, 1);
}

void Object::unloadScript()
{
    // Attempt to execute the on_unload function only if the lua state has 
    // already been initialized with a script
    if (L == NULL)
        return;
    
    // Find the update function and call it
    lua_getglobal(L, "on_unload");
	    
    // Push a pointer to the current object for use within the lua function
    lua_pushlightuserdata(L, (void*)this);
	   
    // Call the function with 1 argument and no return values
    lua_call(L, 1, 0);

    // get the result //
    //position.z = (float)lua_tonumber(L, -1);
    //position.y = (float)lua_tonumber(L, -2);
    //position.x = (float)lua_tonumber(L, -3);
    //lua_pop(L, 1);
    
    lua_close(L);
    L = NULL;
}

//------------------------------------------------------------------------------
void Object::animateScript(float elapsedTime)
{
    // Attempt to execute the script only if the lua state has already been
    // initialized with a script
    if (L == NULL)
        return;
        
    // If the script is suspended, do not run until time has elapsed
    if (suspend) {
        suspendTime -= elapsedTime; 
        if (suspendTime > 0.0f) 
            return;
        suspend = false;
        suspendTime = 0.0f;
    }
    
    // Find the update function and call it
    lua_getglobal(L, "on_update");
	    
    // Push a pointer to the current object for use within the lua function
    lua_pushlightuserdata(L, (void*)this);
	   
	// Push the time passed since the last iteration of the game loop
    lua_pushnumber(L, elapsedTime);
    
    // Call the function with 2 argument and no return values
    lua_call(L, 2, 0);

    // get the result //
    //position.z = (float)lua_tonumber(L, -1);
    //position.y = (float)lua_tonumber(L, -2);
    //position.x = (float)lua_tonumber(L, -3);
    //lua_pop(L, 1);
}
 
//------------------------------------------------------------------------------
void Object::drawObjects( Object* c )
{
    if ( active == true && state != DEAD && isVisible == true )
        draw(c);
    
    if ( next != NULL )
        ((Object*)next)->drawObjects(c);
}
    
//------------------------------------------------------------------------------
void Object::drawObjectOutlines( Object* c )
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
        
    if ( state == DEAD ) {
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
    if ( next != NULL ) {
        ((Object*)next)->prepareObjects();
    }
    processCollisions( getRoot() );
}

//------------------------------------------------------------------------------
void Object::animateObjects( float timeElapsed, Object* c )
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
bool Object::testActiveZone( Object* c )
{   
   //#if ( !testActiveZoneEnable )
   //#   return true;

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
/*
int Object::processExtendedCommand( const ScriptCommand &t )
{  
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
}

*/

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
void Object::setPosition( GLfloat x, GLfloat y, GLfloat z )
{ position.setVector( x, y, z ); }

//------------------------------------------------------------------------------
void Object::setPosition( Vector t )
{ position = t; }
 
//------------------------------------------------------------------------------ 
void Object::setRotationAxis( GLfloat vx, GLfloat vy, GLfloat vz, GLfloat vw )
{ rotation.buildFromAxis( Vector( vx, vy, vz ), vw ); }

//------------------------------------------------------------------------------
void Object::setRotationAxis( const Vector &v, GLfloat a )
{ rotation.buildFromAxis( v, a ); }

//------------------------------------------------------------------------------
void Object::setRotationEuler( GLfloat x, GLfloat y, GLfloat z )
{ rotation.buildFromEuler( x, y, z ); }

//------------------------------------------------------------------------------
void Object::setRotationEuler( const Vector &v )
{ rotation.buildFromEuler( v ); }

//------------------------------------------------------------------------------            
void Object::setRotationQuaternion( const Quaternion &q )
{ rotation = q; }

//------------------------------------------------------------------------------
void Object::setRotationVelocity( GLfloat xAngle, GLfloat yAngle, GLfloat zAngle )
{ rotationVelocity.setVector( xAngle, yAngle, zAngle ); }

//------------------------------------------------------------------------------            
void Object::setRotationVelocity( const Vector &v )
{ rotationVelocity = v; }

//------------------------------------------------------------------------------
void Object::setVelocity( GLfloat x, GLfloat y, GLfloat z )
{ velocity.setVector( x, y, z ); }

//------------------------------------------------------------------------------
void Object::setVelocity( const Vector &v )
{ velocity = v; }      

//------------------------------------------------------------------------------
void Object::setTimer( float* temp )
{
    //#animCurrTime = temp;   
}

//------------------------------------------------------------------------------
void Object::setInterpolationVariable(int index)
{
    switch (index)
    {
        case 0:
            //#valInterpPtr = animCurrTime;
            break;
        case 1:
            valInterpPtr = &position.x;
            break;
        case 2:
            valInterpPtr = &position.y;
            break;
        case 3:
            valInterpPtr = &position.z;
            break;
        //#default:
            //#valInterpPtr = animCurrTime;
    }
}

void Object::setParticleSystem(int particleSystemNumber)
{
    if (particleSystemNumber > 0 && particleSystem == NULL) {
        particleSystem = new Snow(this);
        particleSystem->update(4.0f);
    } 
}

//------------------------------------------------------------------------------
void Object::setScale( GLfloat x, GLfloat y, GLfloat z )
{ scale.setVector( x, y, z ); }

//------------------------------------------------------------------------------
void Object::setScale( const Vector &v )
{ scale = v; }

//------------------------------------------------------------------------------
void Object::setState( unsigned char tState )
{ state = tState; }

//------------------------------------------------------------------------------
void Object::setScaleRate( GLfloat x, GLfloat y, GLfloat z )
{ scaleRate.setVector( x, y, z ); }

//------------------------------------------------------------------------------
void Object::setScaleRate( const Vector &v )
{ scaleRate = v; }

//------------------------------------------------------------------------------
void Object::setSoundClass ( Sound *snd )
{ s = snd; }

//------------------------------------------------------------------------------
Object* Object::getRoot()
{
    Object* temp = this;

    while ( temp->prev != NULL ) {
        temp = (Object*)temp->prev;
    }
     
    return temp;
}


