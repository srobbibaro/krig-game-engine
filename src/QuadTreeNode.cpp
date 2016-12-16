#include "QuadTreeNode.h"

//------------------------------------------------------------------------------
QuadTreeNode::QuadTreeNode() {
  for (int i = 0; i < 4; i++) {
    child[i] = NULL;
  }

  min[0] = min[1] = max[0] = max[1] = 0.0f;

  next = NULL;
}

//------------------------------------------------------------------------------
QuadTreeNode::~QuadTreeNode() {}
