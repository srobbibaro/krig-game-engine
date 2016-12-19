/**
 * @file TerrainEditor.h
 * @brief operations for displaying and interfacing with the terrain editor.
 *
 * Generate, modify, and store terrains.
 */
#ifndef _TERRAIN_EDITOR_H_
#define _TERRAIN_EDITOR_H_

#include <GL/glut.h>
#include "Engine.h"

class TerrainEditor {
  public:
    TerrainEditor() {};
    ~TerrainEditor() {};

    void displayPalette(Engine *engine);
    void initPalette();
    void reshapePalette(const int&, const int&);
    void mousePalette(Engine* engine, const int&, const int&, const int&, const int&);

  private:
    void printString(char* str, const float&, const float&);
};
#endif
