#include "ObjectNode.h"

#include "constants.h"

//------------------------------------------------------------------------------
ObjectNode::ObjectNode(void) {
  next = prev = NULL;
}

//------------------------------------------------------------------------------
ObjectNode::~ObjectNode() {
  next = prev = NULL;
}
