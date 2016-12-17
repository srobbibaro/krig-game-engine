#ifndef _TEXT_GAME_OBJECT_H_
#define _TEXT_GAME_OBJECT_H_

#include "constants.h"
#include "Object.h"
#include <string>

class TextGameObject : public Object {
  public:
    TextGameObject();

    void printTypeName() { PRINT_DEBUG("text\n"); }
    void buildLuaObjectTable(lua_State *L);
    void transferLuaObjectTable(lua_State *L);

    void draw(Object*);
    void drawOutline(Object*) {};
    void drawShadow(Vector*) {};

    void handleCollision(Object*) {};
    void update(Vector*) {};
    void prepare() {};
    void animate(const float&, Object*); // Camera*

    std::string text;
    float color[4];
    float width;
    float fadeRate;

    static void render_string(void*, const char*);
    static void displayText(char *, const float&, const float&, const float&, const float&, const float&);
};

#endif
