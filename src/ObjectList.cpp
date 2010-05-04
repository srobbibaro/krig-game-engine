#include "ObjectList.h"
#include "ObjectNode.h"

ObjectList::ObjectList()
{
    head = NULL;
    size = 0;
}

ObjectList::~ObjectList()
{
    head = NULL;
}

void ObjectList::insertFront(ObjectNode* objectNode)
{
    if (objectNode == NULL)
        return;

    objectNode->next = head;
    objectNode->prev = NULL;

    if (head != NULL)
        head->prev = objectNode;

    head = objectNode;

    size++;
}

void ObjectList::remove(ObjectNode* objectNode)
{
    if (objectNode == NULL)
        return;

    if (objectNode->prev != NULL)
        objectNode->prev->next = objectNode->next;
    else
        head = objectNode->next;

    if (objectNode->next != NULL)
        objectNode->next->prev = objectNode->prev;

    objectNode->prev = objectNode->next = NULL;

    size--;
}
