//////////////////////////////////////////////////////////////////
// Description : Handles basic game logic and windowing.        //
//////////////////////////////////////////////////////////////////

// included files ///////////////////////////////
#include <GL/glut.h>
#include <AL/al.h>
#include <AL/alut.h>

#include <cstdio>
#include <ctype.h>
#include <cmath>
#include <ctime>
#include "Engine.h"
#include "TerrainEditor.h"
#include "constants.h"
#include <cstdlib>
#include <unistd.h>

// global variables /////////////////////////////
Engine *KRIG, *g_script_engine;

#if EDIT
TerrainEditor *terrainEditor;
int mainWin, colorWin;

//------------------------------------------------------------------------------
void displayPalette(void) {
  terrainEditor->displayPalette(KRIG);
  glutSwapBuffers();
  glutSetWindow(mainWin);
}

//------------------------------------------------------------------------------
void reshapePalette(int w, int h) {
  terrainEditor->reshapePalette(w, h);
}

//------------------------------------------------------------------------------
void mousePalette(int btn, int state, int x, int y) {
  terrainEditor->mousePalette(KRIG, btn, state, x, y);
  glutPostRedisplay();
}
#endif

//------------------------------------------------------------------------------
void display() {
#if EDIT
  glutSetWindow(mainWin);
#endif
  if (!KRIG->getIsRunning()) {
    PRINT_DEBUG("Shutting down...\n");

#if EDIT
    delete terrainEditor;
#endif

    delete KRIG;
    alutExit();

    PRINT_DEBUG("Finished shutting down.\n");

    exit(0);
  }

  KRIG->gameCycle();

#if EDIT
  glutSetWindow(colorWin);
  displayPalette();

  glutSetWindow(mainWin);
#endif
}

//------------------------------------------------------------------------------
void idle(void) {
  glutPostRedisplay();
}

//------------------------------------------------------------------------------
void reshape(GLint width, GLint height) {
  if (height == 0)
    height = 1;

  glViewport(0, 0, width, height);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  gluPerspective(45.0f, (GLfloat)800 / (GLfloat)600, 0.001f, 300.0f);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}
//------------------------------------------------------------------------------
void mouseMove(int x, int y) {
  KRIG->processMouseMove(x, y);
}

//------------------------------------------------------------------------------
void pressNormalKey(unsigned char key, GLint mouse_x, GLint mouse_y) {
  KRIG->processNormalKeyDown(key);
}

//------------------------------------------------------------------------------
void releaseNormalKey(unsigned char key, GLint mouse_x, GLint mouse_y) {
  KRIG->processNormalKeyUp(key);
}

//------------------------------------------------------------------------------
void pressKey(GLint key, GLint mouse_x, GLint mouse_y) {
  KRIG->processKeyDown(key);
}

//------------------------------------------------------------------------------
void releaseKey(int key, GLint mouse_x, GLint mouse_y) {
  KRIG->processKeyUp(key);
}

//------------------------------------------------------------------------------
void glutInit(void) {
#if !(EDIT)
  glutSetCursor(GLUT_CURSOR_NONE);
#endif
  glutIgnoreKeyRepeat(1);
  glutSpecialFunc(pressKey);
  glutSpecialUpFunc(releaseKey);
  glutDisplayFunc(display);
  glutIdleFunc(idle);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(pressNormalKey);
  glutKeyboardUpFunc(releaseNormalKey);
  //glutPassiveMotionFunc (mouseMove);
  glutMotionFunc (mouseMove);
}

//------------------------------------------------------------------------------
int main(int argc, char *argv[]) {
  if (argc < 2) {
    PRINT_ERROR("Must specify game directory as argument.\n");
    exit(1);
  }

  char* gameDir = argv[1];

  if (chdir(gameDir) != 0) {
    PRINT_ERROR("Could not find specified game directory.\n");
    exit(1);
  }

  srand(time(NULL));

  glutInit(&argc, argv);
  alutInit(&argc, argv);

  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

  KRIG = new Engine(gameDir);
  g_script_engine = KRIG;

  // setup window /////////////////////////////
  glutInitWindowSize(800, 600);

#if EDIT
  terrainEditor = new TerrainEditor();

  mainWin = glutCreateWindow("KRIG Game Engine - Editor Mode");
  glutSetWindow(mainWin);

  KRIG->initGL();
  glutInit();

  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

  // setup window /////////////////////////////
  glutInitWindowSize(260, 350);

  colorWin = glutCreateWindow("edit_terrain");

  glutPositionWindow(812, 10);
  glutDisplayFunc(display); // display callback invoked when window opened
  glutMouseFunc(mousePalette);
  glutReshapeFunc(reshapePalette);
  glutIdleFunc(idle);
  terrainEditor->initPalette();
  /////////////////////////////////////////////////

  glutSetWindow(mainWin);
#else
#if FULL_SCREEN
  char mode_string[24];
  sprintf(mode_string, "%dx%d:32@30", glutGet(GLUT_SCREEN_WIDTH), glutGet(GLUT_SCREEN_HEIGHT));
  glutGameModeString(mode_string);

  if (glutGameModeGet(GLUT_GAME_MODE_POSSIBLE)) {
    glutEnterGameMode();
  }
  else {
    PRINT_ERROR("Unable to change screen resolution and setup game mode.\n");
    exit(1);
  }
#else
  glutCreateWindow("KRIG Game Engine");
#endif

  KRIG->initGL();
  glutInit();
#endif

  KRIG->loadIntroCredits();

  glutMainLoop();
  return 0;
}
