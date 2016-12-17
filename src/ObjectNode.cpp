#include "ObjectNode.h"
#include <cstddef>

//------------------------------------------------------------------------------
ObjectNode::ObjectNode() {
  next = prev = NULL;
}

//------------------------------------------------------------------------------
ObjectNode::~ObjectNode() {
  next = prev = NULL;
}
