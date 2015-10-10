#include <cstdlib>
#include <string.h>
#include <dirent.h>

#include "Engine.h"
#include "api.h"
#include "buffer.h"

//------------------------------------------------------------------------------
Engine::Engine() {
  // initialize resources used by the engine //
  keyState_.initKeyState();
  specialKeyState_.initKeyState();

  loadModels();

  currentLevel_ = NULL;

  for (int i = 0; i < MAX_GAME_LEVELS; ++i)
    idToGameLevelMap_[i] = NULL;

  // setup game timer /////////////////////////
  if(!timer_.init()) {
    PRINT_ERROR("Timer initialization failed.\n");
    exit (1);
  }
  ///////////////////////////////////////////////

  fps_ = 0.0f;

#if EDIT
  // terrain editing values //
  paint       = false;
  paintColor  = false;

  lastX       = 0.0f;
  lastY       = 0.0f;

  last_x      = 0;
  last_z      = 0;
  last_red    = 0.0f;
  last_green  = 0.0f;
  last_blue   = 0.0f;
  last_height = 0.0f;
  last_type   = 0;
  ////////////////////////////
#endif

  luaState_   = NULL;
  mainCamera_ = c1_ = c2_ = c3_ = c4_ = NULL;

  isPaused_ = false;

  mouseX_ = 0.0f;
  mouseY_ = 0.0f;

  isRunning_      = true;
  isIntroRunning_ = false;
}

//------------------------------------------------------------------------------
bool Engine::loadGame(string file) {
  // If the lua state has not been initialized for this object, attempt to
  // initialize it.
  if (file == "" || luaState_ != NULL)
    return false;

  luaState_ = lua_open();

  // If the lua state still could not be initialized, then exit the game.
  // ... we can do something smarter with this in the finished product.
  if (!luaState_) {
    PRINT_ERROR("Could not create Lua state.\n");
    exit(-1);
  }

  // load Lua base libraries
  luaL_openlibs(luaState_);

  // Load our library
  luaopen_krigApi(luaState_);

  // load the script
  PRINT_DEBUG("Loading Lua game script '%s'...\n", file.c_str());
  luaL_dofile(luaState_, file.c_str());

  // Find the update function and call it
  lua_getglobal(luaState_, SCRIPT_CALLBACK_ON_LOAD);

  if (lua_isfunction(luaState_, -1)) {
    lua_call(luaState_, 0, 0);
  }
  else {
    PRINT_DEBUG_LVL(4, "'%s' function not defined.\n", SCRIPT_CALLBACK_ON_LOAD);
    lua_pop(luaState_, 1);
  }

  return (true);
}

//------------------------------------------------------------------------------
bool Engine::loadIntroCredits() {
  // load the intro script from the buffer
  PRINT_DEBUG("Loading Lua game script 'intro credits'...\n");
  loadLevelFromBuffer(intro_level_script_buffer);

  isIntroRunning_ = true;

  return (true);
}

//------------------------------------------------------------------------------
void Engine::updateGame(float elapsedTime) {
  // Attempt to execute the script only if the lua state has already been
  // initialized with a script
  if (!luaState_)
    return;

  // Find the update function and call it
  lua_getglobal(luaState_, SCRIPT_CALLBACK_ON_UPDATE);

  if (lua_isfunction(luaState_, -1)) {
    lua_pushnumber(luaState_, elapsedTime);
    lua_call(luaState_, 1, 0);
  }
  else {
    PRINT_DEBUG_LVL(4, "'%s' function not defined.\n", SCRIPT_CALLBACK_ON_UPDATE);
    lua_pop(luaState_, 1);
  }
}

//------------------------------------------------------------------------------
void Engine::unloadGame() {
  // Attempt to execute the on_unload function only if the lua state has
  // already been initialized with a script
  if (!luaState_)
    return;

  // Find the update function and call it
  lua_getglobal(luaState_, SCRIPT_CALLBACK_ON_UNLOAD);

  if (lua_isfunction(luaState_, -1)) {
    lua_call(luaState_, 0, 0);
  }
  else {
    PRINT_DEBUG_LVL(4, "'%s' function not defined.\n", SCRIPT_CALLBACK_ON_UNLOAD);
    lua_pop(luaState_, 1);
  }

  lua_close(luaState_);
  luaState_ = NULL;
}

//------------------------------------------------------------------------------
void Engine::gameCycle() {
  timeElapsed_ = timer_.getElapsedSeconds();
  updateGame(timeElapsed_);
  fps_ = timer_.getFPS();

  if (currentLevel_ != NULL && !isPaused_)
    currentLevel_->setElapsedTime(timeElapsed_);

  if (currentLevel_ != NULL) {
    if (currentLevel_->checkComplete()) {
      if (isIntroRunning_) {
        unloadGame();
        loadGame("./scripts/main.lua");
        isIntroRunning_ = false;
      }
    }
    else {
      if (!isPaused_) {
        mainCamera_->update(currentLevel_->getElapsedTime());

        currentLevel_->animateLevel();

        //processCommands();

        currentLevel_->updateLevel();
        currentLevel_->prepareLevel();   // collision detection

        //currentLevel_->updateLevel();

        prepare();

        //Matrix m;
        //mainCamera_->worldRotation.buildRotationMatrix(m);

        //glMultMatrixf(m.data);
        currentLevel_->drawLevel();

        currentLevel_->getMusic()->Update();

        glClear(GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();

        glDepthFunc(GL_ALWAYS);
        currentLevel_->postDraw();
        glDepthFunc(GL_LESS);

        glutSwapBuffers();
      }
    }
  }

  processCommands();
}

//------------------------------------------------------------------------------
void Engine::prepare() {
  glClear(GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();

  mainCamera_->prepareGLView();

  glDepthMask(GL_FALSE);
  currentLevel_->drawSky();
  glDepthMask(GL_TRUE);

  //glDepthFunc(GL_ALWAYS);
  //    currentLevel_->postDraw();
  // glDepthFunc(GL_LESS);

  Matrix translationMatrix;

  Vector position = mainCamera_->getPosition();

  translationMatrix.setTranslation(-position.x, -position.y, -position.z);

  glMultMatrixf(translationMatrix.data);
}

//------------------------------------------------------------------------------
void Engine::initGL() {
  GLfloat shaderData[16] = {
    0.4f, 0.4f, 0.4f, 0.7f, 0.7f, 0.7f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
  };

  //glClearColor(0.0f, 0.0f, 0.3f, 1.0f);
  glClearColor(1.0f, 0.0f, 1.0f, 1.0f);

  glShadeModel(GL_SMOOTH); // use smooth shading
  glEnable(GL_DEPTH_TEST); // remove hidden surfaces

  // create 1D bitmap for lighting ////////////
  glGenTextures(1, &shaderTexture_[0]);

  glBindTexture(GL_TEXTURE_1D, shaderTexture_[0]);
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

  glTexImage1D(GL_TEXTURE_1D, 0, 1, 16, 0, GL_LUMINANCE, GL_FLOAT, shaderData);
  /////////////////////////////////////////////

  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glFrontFace(GL_CCW);
  glEnable(GL_CULL_FACE);
  glPolygonMode(GL_FRONT, GL_FILL);
  glEnable(GL_BLEND);

  // setup projection matrix //////////////////
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective (45.0 , (800.0f / 600.0f) , 0.001f, 300.0f);
  glMatrixMode(GL_MODELVIEW);

  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
  glDisable(GL_LIGHTING);
  glBindTexture(GL_TEXTURE_1D, shaderTexture_[0]);
  glLineWidth(1.0f);

  glDepthFunc(GL_LESS);

  // setup display lists //////////////////////
  lists_ = glGenLists(3);

  // list to setup cel shading ////////////////
  glNewList(lists_, GL_COMPILE);
  glDisable(GL_BLEND);
  //glEnable (GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  glEnable(GL_TEXTURE_1D);
  //glBindTexture(GL_TEXTURE_1D, shaderTexture[0]);
  glLineWidth(1.0f);
  glEndList();

  // list to setup to draw outline ////////////
  glNewList(lists_ + 1, GL_COMPILE);
  glDisable(GL_TEXTURE_1D);
  glEnable(GL_BLEND);
  glDepthFunc(GL_LESS);
  glLineWidth(3.0f);
  glCullFace(GL_FRONT);
  glPolygonMode(GL_BACK, GL_LINE);
  glEndList();

  // list to reset display settings
  glNewList(lists_ + 2, GL_COMPILE);
  glPolygonMode(GL_BACK, GL_FILL);
  glDepthFunc(GL_LESS);
  glCullFace(GL_BACK);
  glEndList();

  glHint(GL_POLYGON_SMOOTH, GL_NICEST);
  glEnable(GL_POLYGON_SMOOTH);
  glEnable(GL_BLEND);
}

//------------------------------------------------------------------------------
void Engine::processKeyUp(int key) {
  PRINT_DEBUG_LVL(1, "special key up (%d)\n", key);
  specialKeyState_.keys[key] = KEY_STATE_RELEASED;
}

//------------------------------------------------------------------------------
void Engine::processKeyDown(int key) {
  PRINT_DEBUG_LVL(1, "special key down (%d)\n", key);
  specialKeyState_.keys[key] = KEY_STATE_PRESSED;
}

//------------------------------------------------------------------------------
void Engine::processCommands() {
  keyState_.initKeyState();
  specialKeyState_.initKeyState();
}

//------------------------------------------------------------------------------
void Engine::processNormalKeyUp(unsigned char key) {
  PRINT_DEBUG_LVL(1, "normal key up (%d)\n", key);
  keyState_.keys[key] = KEY_STATE_RELEASED;
}

//------------------------------------------------------------------------------
void Engine::processNormalKeyDown(unsigned char key) {
  PRINT_DEBUG_LVL(1, "normal key down (%d)\n", key);
  keyState_.keys[key] = KEY_STATE_PRESSED;

#if EDIT
  switch (key) {
    // Select current camera
    case '1':
      mainCamera_ = c1_;
      currentLevel_->setCamera(mainCamera_);
      break;
    case '2':
      mainCamera_ = c2_;
      currentLevel_->setCamera(mainCamera_);
      break;
    case '3':
      mainCamera_ = c3_;
      currentLevel_->setCamera(mainCamera_);
      break;
    case '4':
      mainCamera_ = c4_;
      currentLevel_->setCamera(mainCamera_);
      break;

      // Save terrain
    case 'O':
    case 'o':
      currentLevel_->saveTerrain("terrains/new_terrain.txt");
      break;

      // Toggle settings
    case 'G':
    case 'g':
      currentLevel_->toggleGrid();
      break;
    case 'B':
    case 'b':
      currentLevel_->toggleBoundingBoxes();
      break;
    case 'C':
    case 'c':
      currentLevel_->toggleControlTriangles();
      break;

    case '.':
      // used for profiling
      exit(0);
      break;
  }
#endif
}

void Engine::playLevel(int number) {
  currentLevel_ = idToGameLevelMap_[number];
}

//------------------------------------------------------------------------------
void Engine::prepLevelLoad(int number) {
  if (idToGameLevelMap_[number])
    delete idToGameLevelMap_[number];

  if (!mainCamera_) {
    // setup camera(s) for the current level
    c1_ = new Camera(1); c1_->setProjectionMatrix();
    mainCamera_ = c1_;

    c2_ = new Camera(2); c2_->setProjectionMatrix();
    c3_ = new Camera(3); c3_->setProjectionMatrix();
    c4_ = new Camera(4); c4_->setProjectionMatrix();

    c2_->setPosition(0.0f, 100.0f, 0.0f);
    c2_->setRotationEuler(1.57, 0.0f, 0.0f);
    c3_->setPosition(0.0f, 0.0f, 0.0f);
    c4_->setPosition(0.0f, 0.0f, 0.0f);
  }

  mainCamera_->init();
  mainCamera_->unloadScript();

  idToGameLevelMap_[number] = new GameLevel(lists_);
  idToGameLevelMap_[number]->setCamera(mainCamera_);
}

//------------------------------------------------------------------------------
void Engine::loadLevel(const char* levelFile, int number) {
  if (number >= MAX_GAME_LEVELS || number < 0) {
    PRINT_ERROR("Could not load specified level %d: Out of range (max = %d).\n", number, MAX_GAME_LEVELS);
    return;
  }

  prepLevelLoad(number);
  idToGameLevelMap_[number]->loadLevel(levelFile);
  timeElapsed_ = timer_.getElapsedSeconds();
  playLevel(0);
}

//------------------------------------------------------------------------------
void Engine::loadLevelFromBuffer(const char* buffer) {
  prepLevelLoad(0);
  idToGameLevelMap_[0]->loadLevelFromBuffer(buffer);
  timeElapsed_ = timer_.getElapsedSeconds();
  playLevel(0);
}

//------------------------------------------------------------------------------
void Engine::loadModels() {
  DIR *dir = opendir("./models/");

  if (dir == NULL) {
    PRINT_DEBUG_LVL(2, "'models' directory not present; no models will be loaded.\n");
    return;
  }

  dirent *de;
  ModelStorage *model;
  char filePath[MAX_PATH_LEN];
  string hashKey;

  while ((de = readdir(dir)) != NULL) {
    // only consider model files with the .mdl extension //
    if (strstr(de->d_name, ".mdl") != NULL) {
      // build full path to load //
      strcpy(filePath, "./models/");
      strcat(filePath, de->d_name);

      // load model file into model storage //
      PRINT_DEBUG("Loading model file '%s'...\n", filePath);

      model = new ModelStorage();
      model->load(filePath);

      // TODO: Investigate progress on "shadows" feature
      //model->buildEdges();

      // insert model file into model hash //
      hashKey = string(de->d_name);
      Model::modelHash[hashKey] = model;

      PRINT_DEBUG("done.\n");
    }
  }

  closedir(dir);
}

//------------------------------------------------------------------------------
void Engine::processMouseMove(int x, int y) {
  mouseX_ = x;
  mouseY_ = y;
}

//------------------------------------------------------------------------------
void Engine::shutdown() {
  isRunning_ = false;
}

//------------------------------------------------------------------------------
Engine::~Engine() {
  unloadGame();

  if (currentLevel_)
    delete currentLevel_;

  if (mainCamera_) {
    delete c1_;
    delete c2_;
    delete c3_;
    delete c4_;
  }

  currentLevel_ = NULL;
  c1_ = c2_ = c3_ = c4_ = mainCamera_ = NULL;
}

//------------------------------------------------------------------------------
void Engine::pause() {
  isPaused_ = !isPaused_;
}

#if EDIT
//------------------------------------------------------------------------------
void Engine::getTerrainInfo(int &x, int &z, float &height, int &type, float &red, float &green, float &blue) {
  if (mainCamera_ == NULL)
    return;

  Vector position = c2_->getPosition();

  x = (int)(position.x / 5.0f);
  z = -(int)(position.z / 5.0f);
  currentLevel_->getTerrainInfo(x, z, height, type, red, green, blue);
}

//------------------------------------------------------------------------------
void Engine::updateTerrain(int &x, int &z, float &height, int &type, float &red, float &green, float &blue) {
  if (!mainCamera_)
    return;

  if (mainCamera_->id_ == 2 || mainCamera_->id_ == 3) {
    Vector position = c2_->getPosition();
    x = (int)(position.x / 5.0f);
    z = -(int)(position.z / 5.0f);
    currentLevel_->updateTerrain(x,z,height,type,red,green,blue);

    last_x      = x;
    last_z      = z;
    last_type   = type;
    last_red    = red;
    last_green  = green;
    last_blue   = blue;
    last_height = height;
  }
}

//------------------------------------------------------------------------------
void Engine::updateColor(float &red, float &green, float &blue) {
  if (!mainCamera_)
    return;

  if (mainCamera_->id_ == 2 || mainCamera_->id_ == 3) {
    Vector position = c2_->getPosition();

    int x = (int)(position.x / 5.0f);
    int z = -(int)(position.z / 5.0f);
    currentLevel_->updateColor(x, z, red, green, blue);

    last_red   = red;
    last_green = green;
    last_blue  = blue;
  }
}
#endif
