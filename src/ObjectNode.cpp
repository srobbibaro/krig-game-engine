#include "ObjectNode.h"

#include "constants.h"

//------------------------------------------------------------------------------
ObjectNode::ObjectNode( void )
{
    next = prev = NULL;
}

//------------------------------------------------------------------------------
ObjectNode::~ObjectNode()
{
    next = prev = NULL;
}

//------------------------------------------------------------------------------
void ObjectNode::add( ObjectNode* newNode )
{
    newNode->prev = this;
    newNode->next = this->next;
    
    if ( this->next != NULL )
        this->next->prev = newNode;
    
    this->next = newNode;
}

//------------------------------------------------------------------------------
void ObjectNode::remove( void )
{
    if ( this != NULL )
    {
            if ( prev != NULL )
                prev->next = next;
        
            if ( next != NULL )
                next->prev = prev;
        
            //prev = NULL;
            //next = NULL;
        
        
            cout << "removing object: ";
            this->printTypeName(); 
            cout << endl;
    }
}



