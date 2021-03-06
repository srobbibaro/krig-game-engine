//////////////////////////////////////////////////////////////////
// Description : Functions for terrain generation and display.  //
//////////////////////////////////////////////////////////////////
#include "Terrain.h"

#include <cstdlib>
#include <fstream>

//------------------------------------------------------------------------------
Terrain::Terrain() : Object() {
  vertex_         = NULL;
  lightIntensity_ = NULL;
  color_          = NULL;
  type_           = NULL;
  vertexNormal_   = NULL;

  init();

  typeId_         = 100;
}

//------------------------------------------------------------------------------
void Terrain::init() {
  unload();

  xSize_          = 0;
  zSize_          = 0;
  scaleFactor_    = 1.0f;

  isCurveEnabled_ = true;
  curveDistance_  = 200.0f;
  curveRate_      = 0.05f;

  enableSphereTest_ = false;
}

//------------------------------------------------------------------------------
Terrain::~Terrain() {
  // TODO: On linux there is an issue on unload
}

//------------------------------------------------------------------------------
void Terrain::draw(Object* c) {
  if (!vertex_ || !lightIntensity_ || !color_ || !type_) {
    return;
  }

  int x1, x2;
  int z1, z2;

  float xStart, zStart;
  float xPer1, xPer2, zPer1, zPer2;
  float tPer;
  GLfloat v[3];

  xStart = zStart = 0;

  glPushMatrix();
  QuadTreeNode* n = displayList_->head;

  while (n != NULL) {
    glBegin(GL_TRIANGLE_STRIP);
    xStart = n->min[0] / scaleFactor_;
    zStart = n->min[1] /  scaleFactor_;

    x1 = (int)xStart;
    x2 = ((int)xStart) + 1;

    z1 = (int)zStart;
    z2 = ((int)zStart) + 1;

    v[0] = vertex_[x1][z1][0];
    v[2] = vertex_[x1][z1][2];

    Vector cPosition = c->getPosition();

    xPer1 = (vertex_[x1][z1][0] - cPosition.x) * (vertex_[x1][z1][0] - cPosition.x);
    xPer2 = (vertex_[x2][z1][0] - cPosition.x) * (vertex_[x2][z1][0] - cPosition.x);
    zPer1 = (vertex_[x1][z1][2] - cPosition.z) * (vertex_[x1][z1][2] - cPosition.z);
    zPer2 = (vertex_[x1][z2][2] - cPosition.z) * (vertex_[x1][z2][2] - cPosition.z);

    tPer = sqrt(xPer1 + zPer1);

    //offset = 1000000;

    if (isCurveEnabled_ && tPer > curveDistance_) {
      tPer = tPer - curveDistance_;
      v[1] = -(curveRate_ * (tPer)) * (curveRate_ * (tPer))  + vertex_[x1][z1][1];
    }
    else
      v[1] = vertex_[x1][z1][1];

    glColor3fv(color_[x1][z1]);
    glTexCoord1f(lightIntensity_[x1][z1]);
    glVertex3fv(v);

    v[0] = vertex_[x2][z1][0];
    v[2] = vertex_[x2][z1][2];

    tPer = sqrt(xPer2 + zPer1);

    if (isCurveEnabled_ && tPer > curveDistance_) {
      tPer = tPer - curveDistance_;
      v[1] = -(curveRate_ * (tPer)) * (curveRate_ * (tPer))  + vertex_[x2][z1][1];
    }
    else
      v[1] = vertex_[x2][z1][1];

    glColor3fv(color_[x2][z1]);
    glTexCoord1f(lightIntensity_[x2][z1]);
    glVertex3fv(v);

    v[0] = vertex_[x1][z2][0];
    v[2] = vertex_[x1][z2][2];

    tPer = sqrt(xPer1 + zPer2);

    if (isCurveEnabled_ && tPer > curveDistance_) {
      tPer = tPer - curveDistance_;
      v[1] = -(curveRate_ * (tPer)) * (curveRate_ * (tPer))  + vertex_[x1][z2][1];
    }
    else
      v[1] = vertex_[x1][z2][1];

    glColor3fv(color_[x1][z2]);
    glTexCoord1f(lightIntensity_[x1][z2]);
    glVertex3fv(v);

    tPer = sqrt(xPer2 + zPer2);

    v[0] = vertex_[x2][z2][0];
    v[2] = vertex_[x2][z2][2];

    if (isCurveEnabled_ && tPer > curveDistance_) {
      tPer = tPer - curveDistance_;
      v[1] = -(curveRate_ * (tPer)) * (curveRate_ * (tPer) )  + vertex_[x2][z2][1];
    }
    else
      v[1] = vertex_[x2][z2][1];

    glColor3fv(color_[x2][z2]);
    glTexCoord1f(lightIntensity_[x2][z2]);
    glVertex3fv(v);
    glEnd();

    n = n->next;
  }

  glPopMatrix();
}

//------------------------------------------------------------------------------
void Terrain::drawOutline(Object* c) {
  if (!vertex_ || !lightIntensity_ || !color_ || !type_) {
    return;
  }

  int x1, x2;
  int z1, z2;

  float xStart, zStart;
  float xPer1, xPer2, zPer1, zPer2 ;
  float tPer;
  GLfloat v[3];

  xStart = zStart = 0;

  glPushMatrix();
  QuadTreeNode* n = displayList_->head;

  glColor3f(0.0f, 0.0f, 0.0f);

  while (n) {
    glBegin(GL_TRIANGLES);
    xStart = n->min[0] / scaleFactor_;
    zStart = n->min[1] / scaleFactor_;

    x1 = (int)xStart;
    x2 = ((int)xStart) + 1;

    z1 = (int)zStart;
    z2 = ((int)zStart) +1;

    v[0] = vertex_[x1][z1][0];
    v[2] = vertex_[x1][z1][2];

    Vector cPosition = c->getPosition();

    xPer1 = (vertex_[x1][z1][0] - cPosition.x) * (vertex_[x1][z1][0] - cPosition.x);
    xPer2 = (vertex_[x2][z1][0] - cPosition.x) * (vertex_[x2][z1][0] - cPosition.x);
    zPer1 = (vertex_[x1][z1][2] - cPosition.z) * (vertex_[x1][z1][2] - cPosition.z);
    zPer2 = (vertex_[x1][z2][2] - cPosition.z) * (vertex_[x1][z2][2] - cPosition.z);

    tPer = sqrt(xPer1 + zPer1);

    if (isCurveEnabled_ && tPer > curveDistance_) {
      tPer = tPer - curveDistance_;
      v[1] = -(curveRate_ * (tPer)) * (curveRate_ * (tPer))  + vertex_[x1][z1][1];
    }
    else
      v[1] = vertex_[x1][z1][1];

    glVertex3fv( v );

    v[0] = vertex_[x2][z1][0];
    v[2] = vertex_[x2][z1][2];

    tPer = sqrt(xPer2 + zPer1);

    if (isCurveEnabled_ && tPer > curveDistance_) {
      tPer = tPer - curveDistance_;
      v[1] = -(curveRate_ * (tPer)) * (curveRate_ * (tPer))  + vertex_[x2][z1][1];
    }
    else
      v[1] = vertex_[x2][z1][1];

    glVertex3fv(v);

    v[0] = vertex_[x1][z2][0];
    v[2] = vertex_[x1][z2][2];

    tPer = sqrt(xPer1 + zPer2);

    if (isCurveEnabled_ && tPer > curveDistance_) {
      tPer = tPer - curveDistance_;

      v[1] = -(curveRate_ * (tPer)) * (curveRate_ * (tPer))  + vertex_[x1][z2][1];
    }
    else
      v[1] = vertex_[x1][z2][1];

    glVertex3fv(v);

    tPer = sqrt(xPer2 + zPer2);

    v[0] = vertex_[x2][z2][0];
    v[2] = vertex_[x2][z2][2];

    if (isCurveEnabled_ && tPer > curveDistance_) {
      tPer = tPer - curveDistance_;
      v[1] = -(curveRate_ * (tPer)) * (curveRate_ * (tPer))  + vertex_[x2][z2][1];
    }
    else
      v[1] = vertex_[x2][z2][1];

    glVertex3fv(v);
    glEnd();

    n = n->next;
  }

  glPopMatrix();
}

//------------------------------------------------------------------------------
void Terrain::generate() {}

//------------------------------------------------------------------------------
void Terrain::calcViewableTerrainNorm() {
  if (!vertex_ || !lightIntensity_ || !color_ || !type_) {
    return;
  }

  int x1, x2;
  int z1, z2;

  float xStart, zStart;

  xStart = zStart = 0;

  Vector surfaceNormal;
  Vector temp1;
  Vector temp2;
  GLfloat tempLightIntensity;

  Vector temp[5];
  int count = 0;

  Matrix tempMatrix;
  tempMatrix.loadIdentity();

  QuadTreeNode* n = displayList_->head;

  while (n != NULL) {
    xStart = n->min[0] / scaleFactor_;
    zStart = n->min[1] /  scaleFactor_;

    x1 = (int)xStart;
    x2 = ((int)xStart) + 1;

    z1 = (int)zStart;
    z2 = ((int)zStart) + 1;

    PRINT_DEBUG_LVL(5, "x1=%d, z1=%d, x2=%d, z2=%d\n", x1, z1, x2, z2);

    vertexNormal_[x1][z1].x = 0.0f;
    vertexNormal_[x1][z1].y = 0.0f;
    vertexNormal_[x1][z1].z = 0.0f;

    vertexNormal_[x1+1][z1].x = 0.0f;
    vertexNormal_[x1+1][z1].y = 0.0f;
    vertexNormal_[x1+1][z1].z = 0.0f;

    vertexNormal_[x1][z1+1].x = 0.0f;
    vertexNormal_[x1][z1+1].y = 0.0f;
    vertexNormal_[x1][z1+1].z = 0.0f;

    vertexNormal_[x1+1][z1+1].x = 0.0f;
    vertexNormal_[x1+1][z1+1].y = 0.0f;
    vertexNormal_[x1+1][z1+1].z = 0.0f;

    n = n->next;
  }

  n = displayList_->head;

  while (n != NULL) {
    xStart = n->min[0] / scaleFactor_;
    zStart = n->min[1] /  scaleFactor_;

    x1 = (int)xStart;
    x2 = ((int)xStart) + 1;

    z1 = (int)zStart;
    z2 = ((int)zStart) + 1;

    if (x2 >= xSize_ || z2 >= zSize_) {
      continue;
    }

    temp[0].setVector(vertex_[x1][z1][0], vertex_[x1][z1][1], vertex_[x1][z1][2]);
    temp[1].setVector(vertex_[x1][z2][0], vertex_[x1][z2][1], vertex_[x1][z2][2]);
    temp[2].setVector(vertex_[x2][z1][0], vertex_[x2][z1][1], vertex_[x2][z1][2]);
    temp[3].setVector(vertex_[x2][z2][0], vertex_[x2][z2][1], vertex_[x2][z2][2]);

    surfaceNormal.calcNorm(temp[0], temp[2], temp[1]);

    vertexNormal_[x1][z1].x += surfaceNormal.x;
    vertexNormal_[x1][z1].y += surfaceNormal.y;
    vertexNormal_[x1][z1].z += surfaceNormal.z;

    vertexNormal_[x1][z2].x += surfaceNormal.x;
    vertexNormal_[x1][z2].y += surfaceNormal.y;
    vertexNormal_[x1][z2].z += surfaceNormal.z;

    vertexNormal_[x2][z1].x += surfaceNormal.x;
    vertexNormal_[x2][z1].y += surfaceNormal.y;
    vertexNormal_[x2][z1].z += surfaceNormal.z;

    surfaceNormal.calcNorm(temp[2], temp[3], temp[1]);

    vertexNormal_[x2][z1].x += surfaceNormal.x;
    vertexNormal_[x2][z1].y += surfaceNormal.y;
    vertexNormal_[x2][z1].z += surfaceNormal.z;

    vertexNormal_[x1][z2].x += surfaceNormal.x;
    vertexNormal_[x1][z2].y += surfaceNormal.y;
    vertexNormal_[x1][z2].z += surfaceNormal.z;

    vertexNormal_[x2][z2].x += surfaceNormal.x;
    vertexNormal_[x2][z2].y += surfaceNormal.y;
    vertexNormal_[x2][z2].z += surfaceNormal.z;

    n = n->next;
  }

  // calculate light intensity at every vertex
  n = displayList_->head;

  while (n != NULL) {
    xStart = n->min[0] / scaleFactor_;
    zStart = n->min[1] / scaleFactor_;

    x1 = (int)xStart;
    x2 = ((int)xStart) + 1;

    z1 = (int)zStart;
    z2 = ((int)zStart) + 1;

    vertexNormal_[x1][z1].normalize();

    temp2.rotateVector(tempMatrix, vertexNormal_[x1][z1]);
    temp2.normalize();

    tempLightIntensity = temp2.dotProduct(*light_);

    // light correction //
    if (tempLightIntensity == 1.0f)
      tempLightIntensity = 0.5f;
    else if (tempLightIntensity < 0.0f)
      tempLightIntensity = 0.0f;

    lightIntensity_[x1][z1] = tempLightIntensity;

    n = n->next;
  }
}

//------------------------------------------------------------------------------
void Terrain::calcTerrainNorm(Vector* light) {
  if (!vertex_ || !lightIntensity_ || !color_ || !type_) {
    PRINT_ERROR("Could not save terrain.\n");
    return;
  }

  Vector surfaceNormal;
  Vector temp1;
  Vector temp2;
  GLfloat tempLightIntensity;

  Vector temp[5];
  int count = 0;

  Matrix tempMatrix;
  tempMatrix.loadIdentity();

  int x1, x2;
  int z1, z2;

  // calculate surface normal
  for (int z = 0; z < (zSize_-1); z++) {
    for (int x = 0; x < (xSize_-1); x++)  {
      x1 = x;
      x2 = x+1;
      z1 = z;
      z2 = z+1;

      temp[0].setVector(vertex_[x1][z1][0], vertex_[x1][z1][1], vertex_[x1][z1][2]);
      temp[1].setVector(vertex_[x1][z2][0], vertex_[x1][z2][1], vertex_[x1][z2][2]);
      temp[2].setVector(vertex_[x2][z1][0], vertex_[x2][z1][1], vertex_[x2][z1][2]);
      temp[3].setVector(vertex_[x2][z2][0], vertex_[x2][z2][1], vertex_[x2][z2][2]);

      surfaceNormal.calcNorm(temp[0], temp[2], temp[1]);

      vertexNormal_[x1][z1].x += surfaceNormal.x;
      vertexNormal_[x1][z1].y += surfaceNormal.y;
      vertexNormal_[x1][z1].z += surfaceNormal.z;

      vertexNormal_[x1][z2].x += surfaceNormal.x;
      vertexNormal_[x1][z2].y += surfaceNormal.y;
      vertexNormal_[x1][z2].z += surfaceNormal.z;

      vertexNormal_[x2][z1].x += surfaceNormal.x;
      vertexNormal_[x2][z1].y += surfaceNormal.y;
      vertexNormal_[x2][z1].z += surfaceNormal.z;

      surfaceNormal.calcNorm(temp[2], temp[3], temp[1]);

      vertexNormal_[x2][z1].x += surfaceNormal.x;
      vertexNormal_[x2][z1].y += surfaceNormal.y;
      vertexNormal_[x2][z1].z += surfaceNormal.z;

      vertexNormal_[x1][z2].x += surfaceNormal.x;
      vertexNormal_[x1][z2].y += surfaceNormal.y;
      vertexNormal_[x1][z2].z += surfaceNormal.z;

      vertexNormal_[x2][z2].x += surfaceNormal.x;
      vertexNormal_[x2][z2].y += surfaceNormal.y;
      vertexNormal_[x2][z2].z += surfaceNormal.z;
    }
  }

  // calculate light intensity at every vertex
  for (int z = 0; z < zSize_; z++) {
    for (int x = 0; x < xSize_; x++) {
      vertexNormal_[x][z].normalize();

      temp2.rotateVector(tempMatrix, vertexNormal_[x][z]);
      temp2.normalize();

      tempLightIntensity = temp2.dotProduct(*light);

      // light correction //
      if (tempLightIntensity == 1.0f)
        tempLightIntensity = 0.5f;
      else if (tempLightIntensity < 0.0f)
        tempLightIntensity = 0.0f;

      lightIntensity_[x][z] = tempLightIntensity;
    }
  }
}

//------------------------------------------------------------------------------
void Terrain::update(Vector* light) {
  if (vertex_ == NULL || lightIntensity_ == NULL || color_ == NULL || type_ == NULL) {
    return;
  }

  if (lastLight_.x != light->x || lastLight_.y != light->y || lastLight_.z != light->z) {
    calcViewableTerrainNorm();
    lastLight_.setVector(light->x, light->y, light->z);
  }
}

//------------------------------------------------------------------------------
float Terrain::getHeight(const float &x, const float &z) {
  if (vertex_ == NULL || lightIntensity_ == NULL || color_ == NULL || type_ == NULL) {
    return 0.0f;
  }

  float terX, terZ;

  // x and z value in terms of the terrain
  terX = x / scaleFactor_;
  terZ = -z / scaleFactor_;

  // calculate cell row and column into array
  int row, col1, col2;
  col1 = (int)(terX);
  row = (int)(terZ);
  col2 = col1 + 1;

  if (col1 < 0 || col2 < 0 || row < 0) {
    PRINT_DEBUG("Error: x=%f, z=%f, row=%d, col1=%d, col2=%d\n", x, -z, row, col1, col2);
    return 0.0f;
  }

  // test to make sure still in terrain
  if (col1 >= xSize_ || col2 >= xSize_ || row >= zSize_) {
    // TODO: Does subtracting the xSize from col2 provide any benefit?
    //col2 -= xSize_;
    PRINT_DEBUG("Calculated coordinates outside of the terrain...\n");
    return 0.0f;
  }

  // calculate percentage into cell
  float perX, perZ;
  perX = fmod(terX, xSize_) - col1;
  perZ = float(terZ) - row;

  // get height values at all 4 corners
  float height1, height2, height3, height4;
  height1 = vertex_[col1][row][1];
  height2 = vertex_[col2][row][1];
  height3 = vertex_[col1][row+1][1];
  height4 = vertex_[col2][row+1][1];

  // calculate the final height using bilinear interpolation
  float th1 = (1 - perZ )*height1 + (perZ*height3);
  float th2 = (1 - perZ )*height2 + (perZ*height4);

  float finalHeight = (1 - perX) * th1 + (perX * th2);

  PRINT_DEBUG_LVL(5, "returning height=%f\n", finalHeight);

  return (finalHeight);
}

//------------------------------------------------------------------------------
void Terrain::animate(const float &elapsedTime, Object* c) {
  if (vertex_ == NULL || lightIntensity_ == NULL || color_ == NULL || type_ == NULL) {
    return;
  }

  GLfloat min[] = {9999.0f, 9999.0f, 9999.0f};
  GLfloat max[] = {-9999.0f, -9999.0f, -9999.0f};

  totalTime_ += elapsedTime;

  int x1, x2;
  int z1, z2;

  float xStart, zStart;
  float xPer1, xPer2, zPer1, zPer2 ;
  float tPer;
  GLfloat v[3];

  xStart = zStart = 0;

  QuadTreeNode* n = displayList_->head;

  PRINT_DEBUG_LVL(5, "--------------\n");

  while (n != NULL) {
    xStart = n->min[0] / scaleFactor_;
    zStart = n->min[1] / scaleFactor_;

    x1 = (int)xStart;
    x2 = ((int)xStart) + 1;

    z1 = (int)zStart;
    z2 = ((int)zStart) +1;

    PRINT_DEBUG_LVL(5, "x1=%d, z1=%d\n", x1, z1);

    //if (z1 < zSize_ - 1 && z1 > 3) {
    if (type_[x1][z1] == 1) {
      if (totalTime_ > 0.5f) {
        if (rand() % 100 < 20)
          type_[x1][z1] = 2;
      }
    }
    else if (type_[x1][z1] == 2) {
      vertex_[x1][z1][1] += (elapsedTime);

      if (vertex_[x1][z1][1] > 2.0f) {
        vertex_[x1][z1][1] = 2.0f;
        type_[x1][z1] = 3;
      }
    }
    else if (type_[x1][z1] == 3) {
      vertex_[x1][z1][1] -= (elapsedTime);

      if (vertex_[x1][z1][1] < 0.0f) {
        vertex_[x1][z1][1] =  0.0f;
        type_[x1][z1] = 1;
      }
    }
    //}

    n = n->next;
  }

  if (totalTime_ > 0.5f)
    totalTime_ = 0.0f;

  calcViewableTerrainNorm();

  //collisionBox_[0].setVector( min[0], min[1], min[2] );
  //collisionBox_[1].setVector( max[0], max[1], max[2] );
}

//------------------------------------------------------------------------------
void Terrain::load(const char* filePath, Vector* light) {
  init();

  GLfloat min[] = { 9999.0f, 9999.0f, 9999.0f };
  GLfloat max[] = { -9999.0f, -9999.0f, -9999.0f };

  light_ = light;

  std::ifstream fin;
  fin.open(filePath);
  fin >> xSize_;
  fin >> zSize_;
  fin >> scaleFactor_;

  vertex_ = new GLfloat**[xSize_];
  lightIntensity_ = new GLfloat*[xSize_];
  color_ = new GLfloat**[xSize_];
  type_ = new GLint*[xSize_];

  for (int i = 0; i < xSize_; i++) {
    vertex_[i] = new GLfloat*[zSize_];
    lightIntensity_[i] = new GLfloat[zSize_];
    color_[i] = new GLfloat*[zSize_];
    type_[i] = new GLint[zSize_];

    for (int j = 0; j < zSize_; j++) {
      vertex_[i][j] = new GLfloat[3];
      color_[i][j] = new GLfloat[3];
    }
  }

  for (int z = 0; z < zSize_; z++) {
    for (int x = 0; x < xSize_; x++) {
      fin >> vertex_[x][z][1];
      vertex_[x][z][0] = x * scaleFactor_;
      vertex_[x][z][2] = -z * scaleFactor_;
      fin >> color_[x][z][0] >> color_[x][z][1] >> color_[x][z][2];
      fin >> type_[x][z];

      if (type_[x][z] == 1) {
        vertex_[x][z][1] = (rand() % 100 / 100.0f) * 2.0f;
        int t = rand() % 100;
        if (t < 20) {
          type_[x][z] = 2;
        }
        else if (t < 40) {
          type_[x][z] = 3;
        }
      }

      for (int i = 0; i < 3; i++) {
        if (vertex_[x][z][i] < min[i])
          min[i] = vertex_[x][z][i];

        if (vertex_[x][z][i] > max[i])
          max[i] = vertex_[x][z][i];
      }
    }
  }

  fin.close();

  vertexNormal_ = new Vector*[xSize_];
  for (int i = 0; i < xSize_; i++) {
    vertexNormal_[i] = new Vector[zSize_];
  }

  // calculate normals in code for now, later add static calculation to be
  // loaded in... only water will be dynamic
  calcTerrainNorm(light);

  collisionBox_[0].setVector( min[0], min[1], min[2] );
  collisionBox_[1].setVector( max[0], max[1], max[2] );

  PRINT_DEBUG("Terrain Collision Box min x=%f, y=%f, z=%f\n", collisionBox_[0].x, collisionBox_[0].y, collisionBox_[0].z);
  PRINT_DEBUG("Terrain Collision Box max x=%f, y=%f, z=%f\n", collisionBox_[1].x, collisionBox_[1].y, collisionBox_[1].z);
}

//------------------------------------------------------------------------------
void Terrain::unload() {
  if (vertex_ != NULL && lightIntensity_ != NULL && color_ != NULL && type_ != NULL && vertexNormal_ != NULL) {
    for (int i = 0; i < xSize_; i++) {
      for (int j = 0; j < zSize_; j++) {
        delete[] vertex_[i][j];
        delete[] color_[i][j];
      }

      delete[] vertex_[i];
      delete[] lightIntensity_[i];
      delete[] color_[i];
      delete[] vertexNormal_[i];
      delete[] type_[i];
    }

    delete[] vertex_;
    delete[] lightIntensity_;
    delete[] color_;
    delete[] vertexNormal_;
    delete[] type_;
  }

  vertex_ = NULL;
  lightIntensity_ = NULL;
  color_ = NULL;
  vertexNormal_ = NULL;
  type_ = NULL;
}

//------------------------------------------------------------------------------
void Terrain::drawGrid() {
  glPushMatrix();
  //glDepthFunc( GL_ALWAYS );
  glColor3f(1.0f, 0.0f, 0.0f);
  glBegin(GL_LINES);

  for (int x = 0; x < xSize_; x++) {
    glVertex3f(x*scaleFactor_, 10.0f, 0.0f);
    glVertex3f(x*scaleFactor_, 10.0f, -(zSize_ - 1) * scaleFactor_);
  }

  for (int z = 0; z < zSize_; z++ ) {
    glVertex3f(0.0f, 10.0f, -z * scaleFactor_);
    glVertex3f((xSize_ - 1) * scaleFactor_, 10.0f, -z * scaleFactor_);
  }

  glEnd();
  glPopMatrix();
}

//------------------------------------------------------------------------------
void Terrain::save(const char* filePath, Vector* light) {
  if (vertex_ == NULL || lightIntensity_ == NULL || color_ == NULL || type_ == NULL) {
    PRINT_ERROR("Could not save terrain.\n");
    return;
  }

  std::ofstream fin;
  fin.open(filePath);
  fin << xSize_ << std::endl;
  fin << zSize_ << std::endl;
  fin << scaleFactor_ << std::endl;

  for (int z = 0; z < zSize_; z++) {
    for (int x = 0; x < xSize_; x++) {
      fin << vertex_[x][z][1] << std::endl;

      fin << color_[x][z][0] <<  "\t" << color_[x][z][1]<< "\t" << color_[x][z][2] << std::endl;
      int type = type_[x][z] == 0 ? 0 : 1;
      fin << type << std::endl;
    }
  }

  fin.close();
}

void Terrain::setVertexHeight(const int &x, const int &z, const float &height) {
  if (!vertex_)
    return;

  if ( x >= 0 && x <= xSize_ - 1 && z >= 0 && z <= zSize_ - 1) {
    vertex_[x][z][1] = height;
  }
}

void Terrain::setVertexType(const int &x, const int &z, const int &type) {
  if (!type_)
    return;

  if ( x >= 0 && x <= xSize_ - 1 && z >= 0 && z <= zSize_ - 1) {
    type_[x][z] = type;
  }
}

void Terrain::setVertexColor(const int &x, const int &z, const Vector &color) {
  if (!color_)
    return;

  if ( x >= 0 && x <= xSize_ - 1 && z >= 0 && z <= zSize_ - 1) {
    color_[x][z][0] = color.x;
    color_[x][z][1] = color.y;
    color_[x][z][2] = color.z;
  }
}

//------------------------------------------------------------------------------
float Terrain::getVertexHeight(const int &x, const int &z) {
  if (!vertex_)
    return 0.0f;

  if ( x >= 0 && x <= xSize_ - 1 && z >= 0 && z <= zSize_ - 1) {
    return vertex_[x][z][1];
  }

  return 0.0f;
}

//------------------------------------------------------------------------------
int Terrain::getVertexType(const int &x, const int &z) {
  if (!type_)
    return 0;

  if ( x >= 0 && x <= xSize_ - 1 && z >= 0 && z <= zSize_ - 1) {
    return type_[x][z];
  }

  return 0;
}

//------------------------------------------------------------------------------
Vector Terrain::getVertexColor(const int &x, const int &z) {
  Vector color;
  color.setVector(0.0f, 0.0f, 0.0f);

  if (!color_)
    return color;

  if ( x >= 0 && x <= xSize_ - 1 && z >= 0 && z <= zSize_ - 1) {
    color.setVector(color_[x][z][0], color_[x][z][1], color_[x][z][2]);
    return color;
  }

  return color;
}

void Terrain::buildLuaObjectTable(lua_State *L) {
  Object::buildLuaObjectTable(L);
}

void Terrain::transferLuaObjectTable(lua_State *L) {
  Object::transferLuaObjectTable(L);
}
