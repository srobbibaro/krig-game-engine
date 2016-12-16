#ifndef _OBJECT_NODE_H_
#define _OBJECT_NODE_H_

// This class can be inherited by a class to gain the ability to access data as
// a linked list. This class should be upgraded in the future to allow derived
// class storage as a tree and not just a linked list.

class ObjectNode {
  public:
    ObjectNode();
    virtual ~ObjectNode();

    virtual void printTypeName() = 0;

    ObjectNode* next;
    ObjectNode* prev;
};

#endif
