/**
 * @file StarField.h
 * @brief star field particle system.
 *
 * Star field particle system to be used in a game level.
 */
#ifndef _STAR_FIELD_H_
#define _STAR_FIELD_H_

#define MAX_STAR_PARTICLES 300

#include "ParticleSystem.h"

class StarField: public ParticleSystem {
  public:
    StarField(Object* tOrigin);
    ~StarField();

    void update(const float&);
    void draw();
    void init();

  private:
    void initParticle(const int&);
};

#endif
