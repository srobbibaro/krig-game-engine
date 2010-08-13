#include "model.h"

#ifndef _PLAYER_H_
#define _PLAYER_H_

class Player : public Model
{
    public:
        Player();
        virtual ~Player();

        void printTypeName(void)
        {
#if DEBUG
            printf("Player");
#endif
        }
};


#endif
