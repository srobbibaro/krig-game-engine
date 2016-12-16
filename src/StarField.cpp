#include <stdlib.h>

#include "StarField.h"

//------------------------------------------------------------------------------
StarField::StarField(Object* tOrigin) {
  origin = tOrigin;

  maxParticles = MAX_STAR_PARTICLES;
  numParticles = MAX_STAR_PARTICLES;

  particles = new Particle[maxParticles];

  init();
}

//------------------------------------------------------------------------------
StarField::~StarField() {
  delete[] particles;
}

//------------------------------------------------------------------------------
void StarField::init() {
  for (int i = 0; i < numParticles; ++i) {
    initParticle(i);
  }
}

//------------------------------------------------------------------------------
void StarField::draw() {
  Vector originPosition = origin->getPosition();

  for (int i = 0; i < numParticles; ++i) {
    glColor4f( 1.0f, 1.0f, 1.0f, particles[i].velocity.x );

    glPushMatrix();
      glTranslatef(
        originPosition.x + particles[i].position.x,
        originPosition.y + particles[i].position.y,
        originPosition.z - particles[i].position.z
      );

      const float size = particles[i].intensity;
      glScalef(size, size, size);

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
void StarField::initParticle(const int &index) {
  particles[index].position.setVector(
    -200.0f + rand() % 400,
    -150.0f + rand() % 300,
    250.0f
  );

  // TODO: For now we'll use the values already in the Particle struct to
  // control size and flicker, but this isn't ideal
  particles[index].intensity = 3.0f + rand() % 5;

  bool flickerEnable = (rand() % 1000) < 10;
  float flickerRate = flickerEnable ?
    (-(float)(rand() % 500)) / 500.0f : 0.0f;

  particles[index].velocity.setVector(
    1.0f,           // current alpha value
    flickerRate,    // rate of change
    0.0f            // not presently used
  );
}

//------------------------------------------------------------------------------
void StarField::update(const float &elapsedTime) {
  for (int i = 0; i < numParticles; ++i) {
    Vector attributes = particles[i].velocity;

    attributes.x += (attributes.y * elapsedTime);

    if (attributes.y > 0) {
      if (attributes.x >= 1.0f) {
        attributes.x = 1.0f;
        attributes.y = 0.0f;
      }
    }
    else if (attributes.y < 0){
      if (attributes.x <= 0.0f) {
        attributes.x = 0.0f;
        attributes.y *= -1.0f;
      }
    }
    else {
      bool flickerEnable = (rand() % 1000) < 10;
      attributes.y = flickerEnable ?
        (-(float)(rand() % 500)) / 500.0f : 0.0f;
    }

    particles[i].velocity = attributes;
  }
}
