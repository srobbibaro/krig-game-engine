#include <iostream>
#include "windows.h"
#include "DisplayList.h"

using namespace std;

//------------------------------------------------------------------------------
DisplayList::DisplayList(void)
{
    head = NULL;
    tail = NULL;
}

//------------------------------------------------------------------------------
DisplayList::~DisplayList(void)
{
}
 
//------------------------------------------------------------------------------
void DisplayList::traverseList(void)
{
    QuadTreeNode *n = head;
   
    while (n != NULL) {
        //cout << n->min[0] << " " << n->max[0] << " " << n->min[1] << " " << n->max[1] << endl;
        n = n->next;
    }
}

//------------------------------------------------------------------------------
void DisplayList::insertLast(QuadTreeNode* n)
{
    if (head == NULL) {
        head = n;
        head->next = NULL;
    }
    if (tail == NULL)
        tail = n;
    else {
        tail->next = n;
        tail = n;
    }
        
    tail->next = NULL;
}

//------------------------------------------------------------------------------
void DisplayList::clearList(void)
{
    head = NULL;
    tail = NULL;
}

