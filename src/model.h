#include <map>
#include <string>

#include "Object.h"

#ifndef _MODEL_H_
#define _MODEL_H_

class Terrain;

struct Triangle {
  GLuint vertices[3];   // index into model's vertex array
  GLfloat colors[3][3]; // [vertex][rgb]
};

struct Edge {
  int vertices[2];
  int triangleIndex[2];
};

struct TransformedEdge {
  Vector p1, p2;
};

struct ModelStorage {
  GLuint numVertices;   // number of vertices in the model
  GLuint numTriangles;  // number of triangles in the model
  GLfloat initalScale;  // the scaleFactor that was used to scale the vertices
  GLfloat** baseVertex; // list of base vertices ( not transformed )
  Vector* normal;       // list of normals that comprise the model
  Triangle* triangle;

  void load(char[]);
};

extern std::map <std::string, ModelStorage*> modelHash;

class Model : public Object {
  public:
    Model();
    virtual ~Model();
    void load(string);
    void unload(void);

    void draw(Object*);
    void drawOutline(Object*);
    void drawShadow (Vector*);
    void handleCollision(Object*);
    void update(Vector*);
    void animate(float, Object*);

    void buildEdges();

    void printTypeName() {}

    void orientOnTerrain(Terrain *temp, Quaternion baseRotation);
    void setHeightFromTerrain(Terrain *temp, float offset);

  protected:
    std::string modelKey_;

    // transformed data //
    GLfloat *lightIntensity; // light intensity due to each normal, updated per cycle
    GLfloat **updatedVertex; // list of vertices that comprise the model

    // TODO: Determine if this data is included for shadows (which is not fully
    // implemented).
    int numEdges;
    Edge *edges;

    TransformedEdge *sEdge;
    int numSEdge;
};

#endif
