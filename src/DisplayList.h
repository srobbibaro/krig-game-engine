/**
 * @file DisplayList.h
 * @brief Maintains a list of viewable QuadTreeNodes.
 *
 * List of viewable QuadTreeNodes.
 */
#ifndef _DISPLAY_LIST_H_
#define _DISPLAY_LIST_H_

#include "QuadTreeNode.h"

class DisplayList {
  public:
    DisplayList();
    ~DisplayList();

    void traverseList();

    void insertLast(QuadTreeNode* n);
    void clearList();

    // Public so engine code can walk the list directly: QuadTree builds it,
    // GameLevel owns and clears it, Terrain walks it for rendering.
    QuadTreeNode* head;
    QuadTreeNode* tail;
};

#endif
