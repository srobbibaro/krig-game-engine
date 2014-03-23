#ifndef _PARTICLE_SYSTEM_H_
#define _PARTICLE_SYSTEM_H_

#include <GL/glut.h>
#include "matrix.h"
#include "Object.h"

struct Particle
{
    Vector position;
    Vector velocity;
    float intensity;
};

class ParticleSystem
{
    public:
        ParticleSystem();

        virtual void update(float elapsedTime) = 0;
        virtual void draw(void) = 0;
        virtual void init(void) = 0;

    protected:
        virtual void initParticle(int index) = 0;
        int maxParticles;
        int numParticles;

        float* currentTime;
        float lastTime;

        Particle* particles;
        Object* origin;
};

#endif
