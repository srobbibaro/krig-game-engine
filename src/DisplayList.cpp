#include "DisplayList.h"
#include "constants.h"

//------------------------------------------------------------------------------
DisplayList::DisplayList() {
  head = NULL;
  tail = NULL;
}

//------------------------------------------------------------------------------
DisplayList::~DisplayList() {}

//------------------------------------------------------------------------------
void DisplayList::traverseList() {
  QuadTreeNode *n = head;

  while (n != NULL) {
    PRINT_DEBUG_LVL(2, "Node:\n");
    PRINT_DEBUG_LVL(2, "0=%f,%f\n", n->min[0], n->max[0]);
    PRINT_DEBUG_LVL(2, "1=%f,%f\n", n->min[1], n->max[1]);
    n = n->next;
  }
}

//------------------------------------------------------------------------------
void DisplayList::insertLast(QuadTreeNode* n) {
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
void DisplayList::clearList() {
  head = NULL;
  tail = NULL;
}
