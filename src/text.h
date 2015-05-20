//////////////////////////////////////////////////////////////////
// Description : Functions for displaying stroke fonts and      //
//               bitmap fonts.                                  //
//////////////////////////////////////////////////////////////////
#include "constants.h"
#include "Object.h"
#include <string>

#ifndef _TEXT_H_
#define _TEXT_H_
class ScriptTextType : public Object {
  public:
    ScriptTextType();

    void printTypeName(void) { PRINT_DEBUG("text"); }

    void draw(Object*);
    void drawOutline(Object*) {}; // Camera*
    void drawShadow (Vector*) {}

    void handleCollision(Object*) {};
    void update(Vector*) {};
    void prepare(void) {};
    void animate(float, Object*); // Camera*

    string text;
    float color[4];
    float width;
    float fadeRate;
};

void render_string(void* font, const char* string);
void displayText(char *text, float x, float y, float z, float, float);

#endif
