#ifndef QUAD_TREE_NODE_H_
#define QUAD_TREE_NODE_H_

#include "Sphere.h"

class QuadTreeNode {
  public:
    QuadTreeNode(void);
    ~QuadTreeNode(void);

    QuadTreeNode* child[4];

    float min[2], max[2];
    Sphere boundingSphere;

    QuadTreeNode* next;
};

#endif
