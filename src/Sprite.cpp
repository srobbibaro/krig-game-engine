#include <GL/glut.h>
#include "Sprite.h"
#include "constants.h"
#include "api.h"

//------------------------------------------------------------------------------
Sprite::Sprite() : Object() {
  textureId = -1;
}

//------------------------------------------------------------------------------
void Sprite::draw(Object* camera) {

  glPushMatrix();
    glColor3f(1.0f, 1.0f, 1.0f);
    glTranslatef(position_.x, position_.y, position_.z);
    glScalef(scale_.x, scale_.y, scale_.z);

    if (textureId >= 0) {
      glEnable(GL_TEXTURE_2D);
      glBindTexture(GL_TEXTURE_2D, textureIds[textureId]);
    }

    glBegin(GL_QUADS);
      glTexCoord2f(1.0f, 1.0f);
      glVertex3f (-1.0f, -1.0f, 0.0f);

      glTexCoord2f(0.0f, 1.0f);
      glVertex3f (1.0f, -1.0f, 0.0f);

      glTexCoord2f(0.0f, 0.0f);
      glVertex3f (1.0f, 1.0f, 0.0f);

      glTexCoord2f(1.0f, 0.0f);
      glVertex3f (-1.0f, 1.0f, 0.0f);
    glEnd();

    glDisable(GL_TEXTURE_2D);
  glPopMatrix();
}

//------------------------------------------------------------------------------
void Sprite::setTexture(string textureKey) {
  textureId = textureHash[textureKey];
}
