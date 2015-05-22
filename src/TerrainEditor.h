#ifndef _TERRAIN_EDITOR_H_
#define _TERRAIN_EDITOR_H_

#include <GL/glut.h>
#include "Engine.h"

class TerrainEditor {
  public:
    TerrainEditor() {};
    ~TerrainEditor() {};

    void displayPalette(Engine *engine);
    void initPalette(void);
    void reshapePalette(int w, int h);
    void mousePalette(Engine* engine, int btn, int state, int x, int y);

  private:
    void printString(char* str, float x, float y);
};
#endif
