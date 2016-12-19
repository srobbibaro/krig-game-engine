/**
 * @file SpriteGameObject.h
 * @brief manage 2D sprite (texture)-based game objects.
 *
 * Sprite game object.
 */
#ifndef _SPRITE_GAME_OBJECT_H_
#define _SPRITE_GAME_OBJECT_H_

#include "constants.h"
#include "Object.h"

class SpriteGameObject : public Object {
  public:
    SpriteGameObject();

    void printTypeName() { PRINT_DEBUG("sprite\n"); }

    void draw(Object*);
    void drawOutline(Object*) {}; // Camera*
    void drawShadow (Vector*) {}

    void handleCollision(Object*) {};
    void update(Vector*)          {};
    void prepare()                {};

    void setTexture(const std::string&);

  private:
    int textureId;
};
#endif
