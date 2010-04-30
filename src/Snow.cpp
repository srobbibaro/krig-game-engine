#include <stdlib.h>

#include "Snow.h"

//------------------------------------------------------------------------------
Snow::Snow(Object* tOrigin)
{
    origin = tOrigin;

    maxParticles = 3000;
    numParticles = 3000;

    particles = new Particle[maxParticles];

    init();
}

//------------------------------------------------------------------------------
Snow::~Snow(void)
{
    delete[] particles;
}

//------------------------------------------------------------------------------
void Snow::init(void)
{
    for (int i = 0; i < numParticles; i++) {
        initParticle(i);
    }
}

//------------------------------------------------------------------------------
void Snow::draw(void)
{
    glColor3f( 1.0f, 1.0f, 1.0f );

    for (int i = 0; i < numParticles; i++) {

              Vector up, normal;
                up.setVector( 0.0f, 0.0f, 1.0f );

                Vector originPosition = origin->getPosition();

                normal.setVector((originPosition.x-particles[i].position.x), 0, (originPosition.z-particles[i].position.z));
                normal.normalize();

                Vector rotationAxis;
                rotationAxis.crossProduct( up, normal );
                rotationAxis.normalize();
                float rotationAngle = up.dotProduct( normal );

                glPushMatrix();

                rotationAngle = acos(rotationAngle);
                rotationAngle = ((rotationAngle*180)/3.14);

                glTranslatef(particles[i].position.x, particles[i].position.y, particles[i].position.z);
                glRotatef(rotationAngle, rotationAxis.x, rotationAxis.y, rotationAxis.z);


                Vector t;
                t.setVector((originPosition.x-particles[i].position.x), (originPosition.y-particles[i].position.y), (originPosition.z-particles[i].position.z));
                t.normalize();

                rotationAngle = normal.dotProduct(t);

                if (t.y < 0)
			         glRotatef(acos(rotationAngle)*180/3.14,1,0,0);
                else
			         glRotatef(acos(rotationAngle)*180/3.14,-1,0,0);




        glBegin( GL_TRIANGLES );
            glVertex3f(-0.1, -0.1, 0.0);
            glVertex3f(0.1, -0.1, 0.0);
            glVertex3f(0.1, 0.1, 0.0);

            glVertex3f(-0.1, -0.1, 0.0);
            glVertex3f(0.1, 0.1, 0.0);
            glVertex3f(-0.1, 0.1, 0.0);

            glVertex3f(-0.14, 0.0, -0.001);
            glVertex3f(0.0, -0.14, -0.001);
            glVertex3f(0.14, 0.0, -0.001);

            glVertex3f(-0.14, 0.0, -0.001);
            glVertex3f(0.14, 0.0, -0.001);
            glVertex3f(0.0, 0.14, -0.001);
        glEnd();

        glPopMatrix();
    }
}

//------------------------------------------------------------------------------
void Snow::initParticle(int index)
{
    Vector originPosition = origin->getPosition();

    particles[index].position.setVector(
        originPosition.x +  rand() % 80 - 40,
        originPosition.y +  20.0f + rand() % 10, //rand() %  40 - 20,
        originPosition.z +  rand() % 40 - 40
    );

    particles[index].velocity.setVector(
        -(rand() % 3),
        -(1.0f + rand() % 5),
        0.0f
    );
}

//------------------------------------------------------------------------------
void Snow::update(float elapsedTime)
{
    Vector originPosition = origin->getPosition();

    for (int i = 0; i < numParticles; i++) {
        particles[i].position.x += particles[i].velocity.x * elapsedTime;
        particles[i].position.y += particles[i].velocity.y * elapsedTime;
        particles[i].position.z += particles[i].velocity.z * elapsedTime;

        if ( particles[i].position.x > originPosition.x + 40.0f ||
             particles[i].position.x < originPosition.x - 40.0f ||
             particles[i].position.y > originPosition.y + 30.0f ||
             particles[i].position.y < originPosition.y - 30.0f
         )
            initParticle(i);


    }
}
