/**
 * @file Snow.h
 * @brief snow particle system.
 *
 * Snow particle system to be used in a game level.
 */
#ifndef _SNOW_H_
#define _SNOW_H_

#define MAX_SNOW_PARTICLES 500

#include "ParticleSystem.h"

class Snow : public ParticleSystem {
  public:
    Snow(Object* tOrigin);
    ~Snow();

    void update(const float&);
    void draw();
    void init();

  private:
    void initParticle(const int&);
};

#endif
