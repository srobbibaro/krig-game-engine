#ifndef _PARTICLE_SYSTEM_H_
#define _PARTICLE_SYSTEM_H_

#include <GL/glut.h>
#include "matrix.h"
#include "Object.h"

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
