/**
 * @file QuadTreeNode.h
 * @brief definition of a quatree node containing min and max coordinates.
 *
 * The quadtree is a simplified version of of an octtree used to partition the
 * viewable scene.
 *
 * See: https://en.wikipedia.org/wiki/Quadtree
 */
#ifndef QUAD_TREE_NODE_H_
#define QUAD_TREE_NODE_H_

#include "Sphere.h"

class QuadTreeNode {
  public:
    QuadTreeNode();
    ~QuadTreeNode();

    QuadTreeNode* child[4];

    float min[2], max[2];
    Sphere boundingSphere;

    QuadTreeNode* next;
};

#endif
