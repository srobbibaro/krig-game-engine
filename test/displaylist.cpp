#include "catch.hpp"
#include "DisplayList.h"
#include "QuadTreeNode.h"

SCENARIO( "DisplayList", "[DisplayList]" ) {
  GIVEN( "An empty display list" ) {
    DisplayList list;

    THEN( "head and tail are null" ) {
      REQUIRE(list.head == NULL);
      REQUIRE(list.tail == NULL);
    }

    // traverseList is a debug-only utility (PRINT_DEBUG_LVL(4,...)) that has
    // been vestigial since the initial commit — its body was a commented-out
    // cout statement in 2008. It is a compile-time no-op when DEBUG=0.
    WHEN( "traverseList on empty list" ) {
      THEN( "it does not crash" ) {
        list.traverseList();
      }
    }
  }

  GIVEN( "One node inserted" ) {
    DisplayList list;
    QuadTreeNode a;

    list.insertLast(&a);

    THEN( "head and tail both point to the single node" ) {
      REQUIRE(list.head == &a);
      REQUIRE(list.tail == &a);
      REQUIRE(list.head->next == NULL);
    }
  }

  GIVEN( "Two nodes inserted last" ) {
    DisplayList list;
    QuadTreeNode a, b;

    list.insertLast(&a);
    list.insertLast(&b);

    THEN( "head is first insert, tail is second" ) {
      REQUIRE(list.head == &a);
      REQUIRE(list.tail == &b);
      REQUIRE(list.head->next == &b);
      REQUIRE(list.tail->next == NULL);
    }

    WHEN( "traverseList is called with nodes" ) {
      THEN( "it does not crash" ) {
        list.traverseList();
      }
    }

    // clearList nulls head and tail but does not free nodes — ownership
    // stays with the QuadTree. The nodes remain valid in memory after clearing.
    WHEN( "clearList is called" ) {
      list.clearList();
      THEN( "list is empty" ) {
        REQUIRE(list.head == NULL);
        REQUIRE(list.tail == NULL);
      }

      // Mirrors the GameLevel frame cycle: clearList at the start of each
      // frame, then buildDisplayList re-populates via insertLast.
      AND_WHEN( "nodes are inserted after clearing" ) {
        QuadTreeNode c, d;
        list.insertLast(&c);
        list.insertLast(&d);
        THEN( "list is correctly rebuilt" ) {
          REQUIRE(list.head == &c);
          REQUIRE(list.tail == &d);
          REQUIRE(list.head->next == &d);
          REQUIRE(list.tail->next == NULL);
        }
      }
    }
  }
}
