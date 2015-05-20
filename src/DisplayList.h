#ifndef _DISPLAY_LIST_H_
#define _DISPLAY_LIST_H_

#include "QuadTreeNode.h"
#include "constants.h"

class DisplayList {
  public:
    DisplayList(void);
    ~DisplayList(void);

    void traverseList(void);

    void insertLast(QuadTreeNode* n);
    void clearList(void);

    QuadTreeNode* head;
    QuadTreeNode* tail;
};


#endif
