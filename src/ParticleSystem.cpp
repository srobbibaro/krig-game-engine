#include "ParticleSystem.h"

//------------------------------------------------------------------------------
ParticleSystem::ParticleSystem(void) {}

/*
//------------------------------------------------------------------------------
void Particle::generate( int routine, Vector camPos )
{
    if ( routine == 1 )
    {
        setPosition( camPos.x - 70 + ( rand() % 140 ), camPos.y+ ( rand() % 50 ), camPos.z-75 );
        setVelocity( gameSpeed, 0, 0 );
    }

}

//------------------------------------------------------------------------------
void Particle::init( int routine, Vector camPos )
{
    if ( routine == 1 )
    {
        setPosition( camPos.x - 45 + ( rand() % 90 ), camPos.y-25 +( rand() % 55 ), camPos.z-75 );
        setVelocity( gameSpeed, 0, 0 );
    }
    else if ( routine == 2 )
    {
        setPosition( camPos.x - 45 + ( rand() % 90 ), camPos.y-25+rand() % 55, camPos.z-75+rand() %75 );
        setVelocity( -rand() % 5-2 , -rand() % 8 - 3 , 0 );
    }
}

//------------------------------------------------------------------------------
void ParticleSystem::update( float elapsedTime )
{
    for (int i = 0; i < numParticles; i++) {
        particle[i].position.x += velocity.x * elapsedTime;
        particle[i].position.y += velocity.y * elapsedTime;
        particle[i].position.z += velocity.z * elapsedTime;
    }
}

//------------------------------------------------------------------------------
void Particle::draw( int routine, Vector r, Vector u )
{
    int rnd = rand() % 2000;
    if ( routine == 1 && rnd != 243 )
    {
       //glColor3f( position.y/30, position.y/30, position.y/30 );
        glColor3f( 1.0f, 1.0f, 1.0f );

        glBegin( GL_QUADS );
            glVertex3f( position.x-.1, position.y-.1, position.z );
            glVertex3f( position.x+.1, position.y-.1, position.z );
            glVertex3f( position.x+.1, position.y+.1, position.z );
            glVertex3f( position.x-.1, position.y+.1, position.z );
        glEnd();
    }
    else if ( routine == 2 )
    {

    }
}

//------------------------------------------------------------------------------
void Particle::setPosition( GLfloat x, GLfloat y, GLfloat z )
{ position.setVector( x, y, z ); }

//------------------------------------------------------------------------------
void Particle::setVelocity( GLfloat x, GLfloat y, GLfloat z )
{ velocity.setVector( x, y, z ); }

//------------------------------------------------------------------------------
ParticleSystem::ParticleSystem()
{
    maxParticles = 2000;
    particles = new Particle[maxParticles];
}

//------------------------------------------------------------------------------
void ParticleSystem::setGameSpeed( float tgs )
{
   for ( int i = 0; i < actParticles; i++ )
   {
        particles[i].gameSpeed = tgs;

        if( routine == 1 )
            particles[i].setVelocity( tgs, 0, 0 );
    }
}

//------------------------------------------------------------------------------
void ParticleSystem::update( float elapsedTime, Camera *cam )
{
    for ( int i = 0; i < actParticles; i++ )
    {
        particles[i].update( routine, elapsedTime, cam );
    }
}

//------------------------------------------------------------------------------
void ParticleSystem::generate( Vector camPos )
{
    for ( int i = 0; i < actParticles; i++ )
    {
        particles[i].init( routine, camPos );
    }
}

//------------------------------------------------------------------------------
void ParticleSystem::draw(Vector r, Vector u)
{
    for ( int i = 0; i < actParticles; i++ )
    {
        particles[i].draw(routine, r, u);
    }
}

//------------------------------------------------------------------------------
void ParticleSystem::setRoutine(int tr )
{
    routine = tr;

    if ( routine == 1 )
        actParticles = 100;
    else
        actParticles = maxParticles;
}
*/
