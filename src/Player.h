#include "ModelGameObject.h"

#ifndef _PLAYER_H_
#define _PLAYER_H_

class Player : public ModelGameObject {
  public:
    Player();
    virtual ~Player();

    void printTypeName(void) { PRINT_DEBUG("Player\n"); }
};

#endif
