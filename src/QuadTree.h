/**
 * @file QuadTree.h
 * @brief representation of a quadtree.
 *
 * The quadtree is a simplified version of of an octtree used to partition the
 * viewable scene.
 *
 * See: https://en.wikipedia.org/wiki/Quadtree
 */
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

    // traverseTree is a debug-only utility (PRINT_DEBUG) and a no-op in
    // production builds. It is not called in the normal game loop.
    void traverseTree();

    int buildTree(QuadTreeNode*& p, const float& xMin, const float& xMax, const float& zMin, const float& zMax, const float& scaleFactor);
    void traverseTree(QuadTreeNode* n);
    int buildDisplayList(QuadTreeNode* n, DisplayList* l, Camera* c);
    int buildLeafList(QuadTreeNode* n, DisplayList* l);

    // Public so tests and GameLevel can access the tree directly.
    // The destructor does not delete the tree — callers are responsible
    // for cleanup (see freeQuadTreeNode in test/quadtree.cpp).
    QuadTreeNode* root;
};

#endif
