#ifndef QUAD_TREE_H_
#define QUAD_TREE_H_

#include "QuadTreeNode.h"
#include "DisplayList.h"
#include "terrain.h"
#include "Camera.h"

class QuadTree
{
    public:
        QuadTree(void);
        ~QuadTree(void);

        int buildTree(Terrain* t);
        int buildDisplayList(DisplayList* l, Camera* c);
        int buildLeafList(DisplayList* l);

        void traverseTree(void);

        QuadTreeNode* root;
//    private:
        int buildTree(QuadTreeNode* &p, float xMin, float xMax, float zMin, float zMax, float scaleFactor);
        void traverseTree(QuadTreeNode* n);
        int buildDisplayList(QuadTreeNode* n, DisplayList* l, Camera* c);
        int buildLeafList(QuadTreeNode* n, DisplayList* l);
};

#endif
