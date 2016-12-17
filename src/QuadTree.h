#ifndef QUAD_TREE_H_
#define QUAD_TREE_H_

#include "QuadTreeNode.h"
#include "DisplayList.h"
#include "Terrain.h"
#include "Camera.h"

class QuadTree {
  public:
    QuadTree();
    ~QuadTree();

    void buildTree(Terrain* t);
    int buildDisplayList(DisplayList* l, Camera* c);
    int buildLeafList(DisplayList* l);

    void traverseTree();

    int buildTree(QuadTreeNode* &, const float&, const float&, const float&, const float&, const float&);
    void traverseTree(QuadTreeNode* n);
    int buildDisplayList(QuadTreeNode* n, DisplayList* l, Camera* c);
    int buildLeafList(QuadTreeNode* n, DisplayList* l);

    QuadTreeNode* root;
};

#endif
