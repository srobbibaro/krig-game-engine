#ifndef _STAR_FIELD_H_
#define _STAR_FIELD_H_

#define MAX_STAR_PARTICLES 300 

#include "ParticleSystem.h"

class StarField: public ParticleSystem {
  public:
    StarField(Object* tOrigin);
    ~StarField(void);

    void update(float elapsedTime);
    void draw(void);
    void init(void);

  private:
    void initParticle(int index);
};

#endif
