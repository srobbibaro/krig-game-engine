//////////////////////////////////////////////////////////////////
// Description : Functions for displaying stroke fonts and      //
//               bitmap fonts.                                  //
//////////////////////////////////////////////////////////////////
#include <GL/glut.h>
#include "text.h"
#include "constants.h"
#include "api.h"

//------------------------------------------------------------------------------
void render_string(void* font, const char* string) {
  // Renders a bitmap font string
  char* p = (char*) string;
  while (*p != '\0') glutBitmapCharacter(font, *p++);
}

//------------------------------------------------------------------------------
void displayText(char *text, float x, float y, float z, float scaleX, float scaleY) {
  // Renders a stroke font string
  glPushMatrix();
    glTranslatef (x, y, z);
    glScalef (scaleX, scaleY, 0.0f);
    for (char* p = text; *p; p++)
      glutStrokeCharacter(GLUT_STROKE_ROMAN, *p);
  glPopMatrix();
}

//------------------------------------------------------------------------------
ScriptTextType::ScriptTextType() : Object() {
  text     = "";
  color[0] = 0.0f; color[1] = 0.0f; color[2] = 0.0f; color[3] = 1.0f;
  width    = 10.0f;
  fadeRate = 0.0f;
}

//------------------------------------------------------------------------------
void ScriptTextType::draw(Object* camera) {
  if (isInView_) {
    glEnable(GL_BLEND);

    glPushMatrix();
      glColor4fv(color);
      glLineWidth(width);
      //glTranslatef(position.x, position.y, position.z );
      glRasterPos3f(position_.x, position_.y, position_.z);
      glScalef (scale_.x, scale_.y, scale_.z);
      for (int i = 0; i < text.length(); i++) {
        //glutStrokeCharacter(GLUT_STROKE_ROMAN, text[i]);
        glutBitmapCharacter(GLUT_BITMAP_8_BY_13, text[i]);
      }
    glPopMatrix();

    glDisable(GL_BLEND);
  }
}

//------------------------------------------------------------------------------
void ScriptTextType::animate(float timeElapsed, Object* camera) {
  Object::animate(timeElapsed, camera);

  color[3] += timeElapsed * fadeRate;

  if (color[3] > 1.0f)
    color[3] = 1.0f;
  else if (color[3] < 0.0f)
    color[3] = 0.0f;
}

void ScriptTextType::buildLuaObjectTable(lua_State *L) {
  Object::buildLuaObjectTable(L);

  lua_pushstring(L, "text");
  lua_pushstring(L, text.c_str());
  lua_rawset(L, -3);

  lua_pushstring(L, "color");
  returnArray(L, color, 4);
  lua_rawset(L, -3);

  lua_pushstring(L, "width");
  lua_pushnumber(L, width);
  lua_rawset(L, -3);

  lua_pushstring(L, "fade_rate");
  lua_pushnumber(L, fadeRate);
  lua_rawset(L, -3);
}

void ScriptTextType::transferLuaObjectTable(lua_State *L) {
  Object::transferLuaObjectTable(L);

  int index = -1;
  lua_pushstring(L, "width");
  lua_gettable(L, 1);
  if (lua_isnumber(L, index)) {
    width = lua_tonumber(L, -1);
  }
  lua_pop(L, 1);

  index = -1;
  lua_pushstring(L, "fade_rate");
  lua_gettable(L, 1);
  if (lua_isnumber(L, index)) {
    fadeRate = lua_tonumber(L, -1);
  }
  lua_pop(L, 1);

  index = -1;
  lua_pushstring(L, "color");
  lua_gettable(L, 1);
  loadArray(L, color, 4, -1);
  lua_pop(L, 1);
  /*
  index = -1;
  lua_pushstring(L, "text");
  lua_gettable(L, 1);
  text = string(lua_tostring(L, -1));
  lua_pop(L, 1);
*/
}
