#include "constants.h"
#include "Object.h"

#ifndef _SPRITE_H_
#define _SPRITE_H_
class Sprite : public Object {
  public:
    Sprite();

    void printTypeName(void) { PRINT_DEBUG("sprite\n"); }

    void draw(Object*);
    void drawOutline(Object*) {}; // Camera*
    void drawShadow (Vector*) {}

    void handleCollision(Object*) {};
    void update(Vector*) {};
    void prepare(void) {};

    void setTexture(string);

  private:
    int textureId;
};
#endif
