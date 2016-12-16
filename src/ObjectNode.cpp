#include "ObjectNode.h"

#include "constants.h"

//------------------------------------------------------------------------------
ObjectNode::ObjectNode() {
  next = prev = NULL;
}

//------------------------------------------------------------------------------
ObjectNode::~ObjectNode() {
  next = prev = NULL;
}
