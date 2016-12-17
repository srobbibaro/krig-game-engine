/**
 * @file ParticleSystem.h
 * @brief Base class from which all concrete particle systems are derived.
 *
 * Base particle system class which allows for a specialized class to create
 * a basic particle system to be used in a game level.
 */
#ifndef _PARTICLE_SYSTEM_H_
#define _PARTICLE_SYSTEM_H_

#include <GL/glut.h>
#include "Matrix.h"
#include "Vector.h"

class Object;

struct Particle {
  Vector position;
  Vector velocity;
  float intensity;
};

class ParticleSystem {
  public:
    ParticleSystem();
    virtual ~ParticleSystem();

    virtual void update(const float&) = 0;
    virtual void draw() = 0;
    virtual void init() = 0;

  protected:
    virtual void initParticle(const int&) = 0;

    int maxParticles;
    int numParticles;

    float* currentTime;
    float lastTime;

    Particle* particles;
    Object* origin;
};

#endif
