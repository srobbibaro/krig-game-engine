#ifndef _DISPLAY_LIST_H_
#define _DISPLAY_LIST_H_

#include "QuadTreeNode.h"

class DisplayList
{
    public:
    DisplayList(void);
    ~DisplayList(void);
    
    QuadTreeNode* head;
    QuadTreeNode* tail;
    void traverseList(void);
    
    void insertLast(QuadTreeNode* n); 
    void clearList(void);  
};


#endif
