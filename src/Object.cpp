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

#include <stdlib.h>

#include "Object.h"
#include "Scripting.h"
#include "Snow.h"

GLint numObjects;
Object* object;

//------------------------------------------------------------------------------
Object::Object()
: ObjectNode()
{
    scriptName = "";
    particleSystem = NULL;
    L = NULL;

    init();
}

//------------------------------------------------------------------------------
Object::~Object()
{
   cleanup();
}

void Object::cleanup(void)
{
    if (L != NULL)
        lua_close(L);

    if (particleSystem != NULL)
        delete particleSystem;

    L = NULL;
    particleSystem = NULL;

    scriptIndex = -1;
}

void Object::init(void)
{
    cleanup();

    initSettings();
}

void Object::initSettings()
{
    velocity.setVector(0.0f, 0.0f, 0.0f);
    speed.setVector(0.0f, 0.0f, 0.0f);
    scaleRate.setVector( 0.0f, 0.0f, 0.0f );
    rotationVelocity.setVector(0.0f, 0.0f, 0.0f);

    setRotationEuler(0.0f, 0.0f, 0.0f);
    position.setVector(0.0f, 0.0f, 0.0f);
    scale.setVector( 1.0f, 1.0f, 1.0f );

    rotation.loadMultIdentity();
    baseDirection.setVector( 0.0f, 0.0f, 1.0f );
    direction.setVector( 0.0f, 0.0f, 1.0f );
    up.setVector(0.0f, 1.0f, 0.0f);
    orth.setVector(1.0f, 0.0f, 0.0f);

    // set default orientation and disable interpolation
    rInterpStart.loadMultIdentity();
    rInterpEnd.loadMultIdentity();

    valInterpBegin_ = valInterpCurrent_ = valInterpEnd_ = 0.0f;
    isInterpolationEnabled_ = false;

    // setup default collision detection
    boundingSphere.setSphere(0.0f, 0.0f, 0.0f, 0.0f);
    collisionBox[0].setVector( 0.0f, 0.0f, 0.0f );
    collisionBox[1].setVector( 0.0f, 0.0f, 0.0f );
    isCollisionDetectionEnabled_ = true;

    state = NORMAL;
    active = true;
    suspendTime = 0.0f;
    typeId = -1;

    isInView = true;
    isDrawEnabled_ = true;

    lastLight.setVector(0.0f, 0.0f, 0.0f);
    scaleChanged = true;
    rotationChanged = true;

    isAlwaysLit_ = false;
}

void Object::setScript( string name )
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


    luaL_loadfile(L, name.c_str());
    scriptIndex = luaL_ref( L, LUA_REGISTRYINDEX );
}

void Object::loadScript(string name)
{
    // Load this object's animation script
    //luaL_dofile(L, scriptName.c_str());

    if (scriptIndex == -1)
        return;

    lua_rawgeti( L, LUA_REGISTRYINDEX, scriptIndex);
    lua_call(L, 0, 0);

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
    if (suspendTime > 0.0f) {
        suspendTime -= elapsedTime;
        if (suspendTime > 0.0f)
            return;
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
void Object::processCollisions( Object* temp )
{
       if ( this != temp &&
         active && temp->active &&
         state == NORMAL && temp->state == NORMAL &&
         isCollisionDetectionEnabled_ && temp->isCollisionDetectionEnabled_ &&
         this != NULL && temp != NULL ) {
 /*

 */
    //# try a sphere-based collision

        float radius1 = boundingSphere.getRadius();
        float radius2 = boundingSphere.getRadius();
        float radius_sum = radius1 + radius2;
        radius_sum = radius_sum * radius_sum;

        float distance =
            (position.x - temp->position.x)*( position.x - temp->position.x)+
            (position.y - temp->position.y)*( position.y - temp->position.y)+
            (position.z - temp->position.z)*( position.z - temp->position.z);

            if (radius_sum >= distance) {

             Vector boxA[2];
    Vector boxB[2];

    boxA[0].setVector(collisionBox[0].x + position.x, collisionBox[0].y + position.y, collisionBox[0].z + position.z);
    boxA[1].setVector(collisionBox[1].x + position.x, collisionBox[1].y + position.y, collisionBox[1].z + position.z);
    boxB[0].setVector(temp->collisionBox[0].x + temp->position.x, temp->collisionBox[0].y + temp->position.y, temp->collisionBox[0].z + temp->position.z);
    boxB[1].setVector(temp->collisionBox[1].x + temp->position.x, temp->collisionBox[1].y + temp->position.y, temp->collisionBox[1].z + temp->position.z);

             if( ((boxA[0].x >= boxB[0].x && boxA[0].x <= boxB[1].x) ||
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
            (boxB[1].z >= boxA[0].z && boxB[1].z <= boxA[1].z)) ) {
                //cout << "Collision! object 1=" << this->typeId << " object 2=" << temp->typeId << endl;
                handleCollision( temp );
                temp->handleCollision(this);
            }
        }
    }

    if ( temp->next != NULL )
        processCollisions(((Object*)temp->next));
}



//------------------------------------------------------------------------------
float Object::calcTriangleCenter( void )
{
    float th1 = ( .5 )*controlPoints[0].y + (.5*controlPoints[1].y );
    float finalHeight = ( .5 )*th1 + ( .5 * controlPoints[2].y );

    return( finalHeight );
}

//------------------------------------------------------------------------------
/*
int Object::processExtendedCommand( const ScriptCommand &t )
{
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
	   glTranslatef(position.x, position.y, position.z);

        glColor3f(1.0f, 0.0f, 0.0f);


        float radius = boundingSphere.getRadius();

        float rad = 6.28f/16.0f;

        glBegin(GL_LINE_LOOP);
            for (int i = 0; i < 16; i++) {
                glVertex3f((cos(rad * i) * radius), (sin(rad * i) * radius), 0.0f);
            }
         glEnd();


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
	   glTranslatef( position.x, position.y, position.z);
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
void Object::setSpeed( GLfloat x, GLfloat y, GLfloat z )
{ speed.setVector( x, y, z ); }

//------------------------------------------------------------------------------
void Object::setSpeed( const Vector &v )
{ speed = v; }

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
Object* Object::getRoot()
{
    Object* temp = this;

    while ( temp->prev != NULL ) {
        temp = (Object*)temp->prev;
    }

    return temp;
}


