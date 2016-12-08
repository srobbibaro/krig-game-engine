#include "constants.h"
#include "Object.h"
#include <string>

#ifndef _TEXT_GAME_OBJECT_H_
#define _TEXT_GAME_OBJECT_H_
class TextGameObject : public Object {
  public:
    TextGameObject();

    void printTypeName(void) { PRINT_DEBUG("text\n"); }
    void buildLuaObjectTable(lua_State *L);
    void transferLuaObjectTable(lua_State *L);

    void draw(Object*);
    void drawOutline(Object*) {}; // Camera*
    void drawShadow (Vector*) {}

    void handleCollision(Object*) {};
    void update(Vector*) {};
    void prepare(void) {};
    void animate(float, Object*); // Camera*

    std::string text;
    float color[4];
    float width;
    float fadeRate;

    static void render_string(void* font, const char* string);
    static void displayText(char *text, float x, float y, float z, float, float);
};

#endif
