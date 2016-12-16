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
