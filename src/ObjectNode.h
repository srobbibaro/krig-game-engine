#ifndef _OBJECT_NODE_H_
#define _OBJECT_NODE_H_

// This class can be inherited by a class to gain the ability to access data as
// a linked list. This class should be upgraded in the future to allow derived 
// class storage as a tree and not just a linked list.

class ObjectNode
{
    public:
        ObjectNode* next;
        ObjectNode* prev;
        
        ObjectNode( void );
        virtual ~ObjectNode();
        void add( ObjectNode* );
        void remove( void );
        
        virtual void printTypeName(void) = 0;
};

#endif
