/**
 * @file ObjectList.h
 * @brief Create a linked list of game objects.
 *
 * Game object management through linked list.
 */
#ifndef _OBJECT_LIST_H_
#define _OBJECT_LIST_H_

#include "ObjectNode.h"
#include "constants.h"

class ObjectList {
  public:
    ObjectList();
    ~ObjectList();

    void insertFront(ObjectNode*);
    void remove(ObjectNode*);

    ObjectNode* head;
    int size;
};

#endif
