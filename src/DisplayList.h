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

    QuadTreeNode* head;
    QuadTreeNode* tail;
};

#endif
