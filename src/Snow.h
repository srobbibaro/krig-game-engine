#ifndef _SNOW_H_
#define _SNOW_H_

#include "ParticleSystem.h"

class Snow : public ParticleSystem
{
    public:
        Snow(Object* tOrigin);
        ~Snow(void);
        
        void update(float elapsedTime);
        void draw(void);
        void init(void);
        
    private:
        void initParticle(int index);
};

#endif


