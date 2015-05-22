#include "TerrainEditor.h"

#if EDIT
//------------------------------------------------------------------------------
void TerrainEditor::displayPalette(Engine *engine) {
  int count = 0;
  int xpos, zpos, type;
  float height, red, green, blue;
  char m[120];
  glClear(GL_COLOR_BUFFER_BIT);

  engine->getTerrainInfo(xpos, zpos, height, type, red, green, blue);

  glClear(GL_COLOR_BUFFER_BIT);
  glColor3f(0.0f, 0.0f, 0.0f);

  sprintf(m, "Red Attribute: %f", red);
  printString(m, 10, 10);

  glColor3f(0.0f, 0.0f, 0.0f);

  sprintf(m, "Green Attribute: %f", green);
  printString(m, 10, 50);

  glColor3f(0.0f, 0.0f, 0.0f);

  sprintf(m, "Blue Attribute: %f", blue);
  printString(m, 10, 90);

  sprintf(m, "Vertex: %d, %d height: %f", xpos, zpos, height);
  printString(m, 10, 130);

  sprintf(m, "Type: %d", type);
  printString(m, 10, 285);

  glBegin(GL_QUADS);
  // RED //
  glColor3f(0.0f, 0.0f, 0.0f);
  glVertex2f(0.0f,15.0f);

  glColor3f(0.0f, 0.0f, 0.0f);
  glVertex2f(0.0f,35.0f);

  glColor3f(1.0f, 0.0f, 0.0f);
  glVertex2f(127.0f,35.0f);

  glColor3f(1.0f, 0.0f, 0.0f);
  glVertex2f(127.0f,15.0f);

  glColor3f(1.0f, 0.0f, 0.0f);
  glVertex2f(127.0f,15.0f);

  glColor3f(1.0f, 0.0f, 0.0f);
  glVertex2f(127.0f,35.0f);

  glColor3f(1.0f, 1.0f, 1.0f);
  glVertex2f(255.0f,35.0f);

  glColor3f(1.0f, 1.0f, 1.0f);
  glVertex2f(255.0f,15.0f);

  // GREEN //
  glColor3f(0.0f, 0.0f, 0.0f);
  glVertex2f(0.0f,55.0f);

  glColor3f(0.0f, 0.0f, 0.0f);
  glVertex2f(0.0f,75.0f);

  glColor3f(0.0f, 1.0f, 0.0f);
  glVertex2f(127.0f,75.0f);

  glColor3f(0.0f, 1.0f, 0.0f);
  glVertex2f(127.0f,55.0f);

  glColor3f(0.0f, 1.0f, 0.0f);
  glVertex2f(127.0f,55.0f);

  glColor3f(0.0f, 1.0f, 0.0f);
  glVertex2f(127.0f,75.0f);

  glColor3f(1.0f, 1.0f, 1.0f);
  glVertex2f(255.0f,75.0f);

  glColor3f(1.0f, 1.0f, 1.0f);
  glVertex2f(255.0f,55.0f);

  // BLUE //
  glColor3f(0.0f, 0.0f, 0.0f);
  glVertex2f(0.0f,95.0f);

  glColor3f(0.0f, 0.0f, 0.0f);
  glVertex2f(0.0f,115.0f);

  glColor3f(0.0f, 0.0f, 1.0f);
  glVertex2f(127.0f,115.0f);

  glColor3f(0.0f, 0.0f, 1.0f);
  glVertex2f(127.0f,95.0f);

  glColor3f(0.0f, 0.0f, 1.0f);
  glVertex2f(127.0f,95.0f);

  glColor3f(0.0f, 0.0f, 1.0f);
  glVertex2f(127.0f,115.0f);

  glColor3f(1.0f, 1.0f, 1.0f);
  glVertex2f(255.0f,115.0f);

  glColor3f(1.0f, 1.0f, 1.0f);
  glVertex2f(255.0f,95.0f);
  glEnd();

  glBegin(GL_LINES);
  glColor3f(1.0f, 0.0f, 0.0f);
  glVertex2f(0.0f, 135.0f);
  glVertex2f(240.0f, 135.0f);

  for (int i = 0; i <= 240; i += 24) {
    glVertex2f(i, 135.0f);
    glVertex2f(i, 155.0f);
  }

  for (int i = 0; i < 240; i += 12) {
    glVertex2f(i, 135.0f);
    glVertex2f(i, 145.0f);
  }

  glColor3f(1.0f, 0.0f, 0.0f);
  glVertex2f(0.0f, 185.0f);
  glVertex2f(240.0f, 185.0f);

  for (int i = 0; i <= 240; i += 24) {
    glVertex2f(i, 185.0f);
    glVertex2f(i, 205.0f);
  }

  for (int i = 0; i < 240; i += 12) {
    glVertex2f(i, 185.0f);
    glVertex2f(i, 195.0f);
  }

  glColor3f(1.0f, 0.0f, 0.0f);
  glVertex2f(0.0f, 235.0f);
  glVertex2f(240.0f, 235.0f);

  for (int i = 0; i <= 240; i += 24) {
    glVertex2f(i, 235.0f);
    glVertex2f(i, 255.0f);
  }

  for (int i = 0; i < 240; i += 12) {
    glVertex2f(i, 235.0f);
    glVertex2f(i, 245.0f);
  }
  glEnd();

  glBegin(GL_QUADS);
  glColor3f(1.0f, 0.0f, 0.0f);
  glVertex2f(0.0f,325.0f);
  glVertex2f(0.0f,345.0f);
  glVertex2f(27.0f,345.0f);
  glVertex2f(27.0f,325.0f);

  glColor3f(red, green, blue);
  glVertex2f(1.0f,326.0f);
  glVertex2f(1.0f,344.0f);
  glVertex2f(26.0f,344.0f);
  glVertex2f(26.0f,326.0f);
  glEnd();

  glColor3f(0.0f, 0.0f, 0.0f);
  sprintf(m, "00                           05                           10");
  printString(m, 0, 165);

  sprintf(m, "10                           15                           20");
  printString(m, 0, 215);

  sprintf(m, "20                           25                           30");
  printString(m, 0, 265);

  sprintf(m, "land(0)    water(1)");
  printString(m, 0, 305);
}

//------------------------------------------------------------------------------
void TerrainEditor::initPalette(void) {
  glClearColor(1.0, 1.0, 1.0, 0.0);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0.0, 255, 350, 0.0);

  glMatrixMode(GL_MODELVIEW);
}

//------------------------------------------------------------------------------
void TerrainEditor::reshapePalette(int w, int h) {
  glViewport(0, 0, (GLsizei) w, (GLsizei) h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0.0f, 255.0f, 350.0f, 0.0f);
}

//------------------------------------------------------------------------------
void TerrainEditor::mousePalette(Engine *engine, int btn, int state, int x, int y) {
  int skip = 0;
  int xpos, zpos, type;
  float height, red, green, blue;

  if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
    engine->getTerrainInfo(xpos, zpos, height, type, red, green, blue);

    if (y >=15 && y <= 35) {
      // modify red
      red = (float)(x / 259.0f);
    }
    else if (y >=55 && y <= 75) {
      // modify green
      green = (float)(x / 259.0f);
    }
    else if (y >=95 && y <= 115) {
      // modify blue
      blue = (float)(x / 259.0f);
    }

    else if (y >=135 && y <= 155) {
      // modify height
      height = (float)(x / 244.0f * 10.0f);
    }
    else if (y >=185 && y <= 205) {
      // modify height
      height = (float)(x / 244.0f * 10.0f) + 10.0f;
    }
    else if (y >=235 && y <= 255) {
      // modify height
      height = (float)(x / 244.0f * 10.0f) + 20.0f;
    }

    else if (y >=185 && y <= 205) {
      // modify height
      height = (float)(x / 244.0f * 10.0f) + 10.0f;
    }
    else if (y >=295 && y <= 320 && x >= 0 && x <= 30) {
      type = 0;
    }

    else if (y >=295 && y <= 320 && x >= 70 && x <= 120) {
      type = 1;
    }
    else {
      skip = 1;
    }

    if (skip == 0)
      engine->updateTerrain(xpos, zpos, height, type, red, green, blue);
  }
  if(btn == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {}

  glutPostRedisplay();
}
#else
//------------------------------------------------------------------------------
void TerrainEditor::displayPalette(Engine *engine) {}

//------------------------------------------------------------------------------
void TerrainEditor::initPalette(void) {}

//------------------------------------------------------------------------------
void TerrainEditor::reshapePalette(int w, int h) {}

//------------------------------------------------------------------------------
void TerrainEditor::mousePalette(Engine *engine, int btn, int state, int x, int y) {}
#endif

//------------------------------------------------------------------------------
void TerrainEditor::printString(char* str, float x, float y) {
  glRasterPos2d(x, y);

  int i = 0;
  while (str[i] != '\0')
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[i++]);
}
