#include "catch.hpp"
#include "ObjectList.h"
#include "ObjectNode.h"

namespace {

class TestListNode : public ObjectNode {
  public:
    void printTypeName() {}
};

}  // namespace

SCENARIO( "ObjectList linked list", "[ObjectList]" ) {
  GIVEN( "An empty list" ) {
    ObjectList list;

    THEN( "head is null and size is zero" ) {
      REQUIRE(list.head == NULL);
      REQUIRE(list.size == 0);
    }

    WHEN( "inserting NULL" ) {
      list.insertFront(NULL);
      THEN( "list unchanged" ) {
        REQUIRE(list.head == NULL);
        REQUIRE(list.size == 0);
      }
    }
  }

  GIVEN( "Three nodes inserted at front" ) {
    ObjectList list;
    TestListNode n1, n2, n3;
    list.insertFront(&n1);
    list.insertFront(&n2);
    list.insertFront(&n3);

    THEN( "size is three and order is n3, n2, n1" ) {
      REQUIRE(list.size == 3);
      REQUIRE(list.head == &n3);
      REQUIRE(n3.next == &n2);
      REQUIRE(n2.next == &n1);
      REQUIRE(n1.next == NULL);
      REQUIRE(n1.prev == &n2);
      REQUIRE(n2.prev == &n3);
      REQUIRE(n3.prev == NULL);
    }

    WHEN( "removing the middle node" ) {
      list.remove(&n2);
      THEN( "links repair and size drops" ) {
        REQUIRE(list.size == 2);
        REQUIRE(n3.next == &n1);
        REQUIRE(n1.prev == &n3);
        REQUIRE(n2.next == NULL);
        REQUIRE(n2.prev == NULL);
      }
    }

    WHEN( "removing the head" ) {
      list.remove(&n3);
      THEN( "new head is n2" ) {
        REQUIRE(list.size == 2);
        REQUIRE(list.head == &n2);
        REQUIRE(n2.prev == NULL);
      }
    }

    WHEN( "removing NULL" ) {
      int sizeBefore = list.size;
      list.remove(NULL);
      THEN( "list unchanged" ) {
        REQUIRE(list.size == sizeBefore);
      }
    }
  }
}
