#include "catch.hpp"
#include <cmath>
#include "QuadTree.h"
#include "QuadTreeNode.h"
#include "DisplayList.h"

namespace {

// QuadTree's destructor does not delete the tree; callers must free nodes.
void freeQuadTreeNode(QuadTreeNode *n) {
  if (n == NULL)
    return;
  for (int i = 0; i < 4; ++i) {
    freeQuadTreeNode(n->child[i]);
  }
  delete n;
}

int countDisplayList(DisplayList *list) {
  int n = 0;
  for (QuadTreeNode *p = list->head; p != NULL; p = p->next) {
    ++n;
  }
  return n;
}

bool isLeaf(QuadTreeNode *n) {
  if (n == NULL)
    return false;
  return n->child[0] == NULL && n->child[1] == NULL && n->child[2] == NULL &&
         n->child[3] == NULL;
}

}  // namespace

SCENARIO( "QuadTree spatial partition", "[QuadTree]" ) {
  GIVEN( "A tree that is already a leaf at root" ) {
    QuadTree qt;
    REQUIRE(qt.root == NULL);

    qt.buildTree(qt.root, 0.0f, 10.0f, 0.0f, 10.0f, 10.0f);
    REQUIRE(qt.root != NULL);

    THEN( "bounds and bounding sphere match implementation formulas" ) {
      REQUIRE(qt.root->min[0] == Approx(0.0f));
      REQUIRE(qt.root->max[0] == Approx(10.0f));
      REQUIRE(qt.root->min[1] == Approx(0.0f));
      REQUIRE(qt.root->max[1] == Approx(10.0f));

      Vector o;
      qt.root->boundingSphere.getOriginVector(o);
      REQUIRE(o.x == Approx(5.0f));
      REQUIRE(o.y == Approx(0.0f));
      REQUIRE(o.z == Approx(-5.0f));
      // NOTE: the radius formula uses the full diagonal (sqrt(w²+h²)) rather
      // than the circumradius (half-diagonal). The sphere is 2x oversized,
      // causing conservative over-rendering. See known bugs for the fix.
      const float expectedR = std::sqrt(200.0f);
      REQUIRE(qt.root->boundingSphere.getRadius() == Approx(expectedR));
    }

    THEN( "root has no children" ) {
      REQUIRE(isLeaf(qt.root));
    }

    freeQuadTreeNode(qt.root);
    qt.root = NULL;
  }

  GIVEN( "A 20x20 region with scale 10 (splits into four leaves)" ) {
    QuadTree qt;
    qt.buildTree(qt.root, 0.0f, 20.0f, 0.0f, 20.0f, 10.0f);

    THEN( "root has four children and each is a leaf" ) {
      REQUIRE(qt.root->child[0] != NULL);
      REQUIRE(qt.root->child[1] != NULL);
      REQUIRE(qt.root->child[2] != NULL);
      REQUIRE(qt.root->child[3] != NULL);
      for (int i = 0; i < 4; ++i) {
        REQUIRE(isLeaf(qt.root->child[i]));
      }
    }

    THEN( "child[0] covers xMin-midX, zMin-midZ" ) {
      QuadTreeNode *c = qt.root->child[0];
      REQUIRE(c->min[0] == Approx(0.0f));
      REQUIRE(c->max[0] == Approx(10.0f));
      REQUIRE(c->min[1] == Approx(0.0f));
      REQUIRE(c->max[1] == Approx(10.0f));
    }

    THEN( "child[1] covers xMin-midX, midZ-zMax" ) {
      QuadTreeNode *c = qt.root->child[1];
      REQUIRE(c->min[0] == Approx(0.0f));
      REQUIRE(c->max[0] == Approx(10.0f));
      REQUIRE(c->min[1] == Approx(10.0f));
      REQUIRE(c->max[1] == Approx(20.0f));
    }

    THEN( "child[2] covers midX-xMax, zMin-midZ" ) {
      QuadTreeNode *c = qt.root->child[2];
      REQUIRE(c->min[0] == Approx(10.0f));
      REQUIRE(c->max[0] == Approx(20.0f));
      REQUIRE(c->min[1] == Approx(0.0f));
      REQUIRE(c->max[1] == Approx(10.0f));
    }

    THEN( "child[3] covers midX-xMax, midZ-zMax" ) {
      QuadTreeNode *c = qt.root->child[3];
      REQUIRE(c->min[0] == Approx(10.0f));
      REQUIRE(c->max[0] == Approx(20.0f));
      REQUIRE(c->min[1] == Approx(10.0f));
      REQUIRE(c->max[1] == Approx(20.0f));
    }

    THEN( "buildLeafList collects four nodes" ) {
      DisplayList list;
      qt.buildLeafList(&list);
      REQUIRE(countDisplayList(&list) == 4);
    }

    freeQuadTreeNode(qt.root);
    qt.root = NULL;
  }

  GIVEN( "A 20x10 strip (split along X only)" ) {
    QuadTree qt;
    qt.buildTree(qt.root, 0.0f, 20.0f, 0.0f, 10.0f, 10.0f);

    THEN( "two children along X, both leaves" ) {
      REQUIRE(qt.root->child[0] != NULL);
      REQUIRE(qt.root->child[1] != NULL);
      REQUIRE(qt.root->child[2] == NULL);
      REQUIRE(qt.root->child[3] == NULL);
      REQUIRE(isLeaf(qt.root->child[0]));
      REQUIRE(isLeaf(qt.root->child[1]));
      REQUIRE(qt.root->child[0]->min[0] == Approx(10.0f));
      REQUIRE(qt.root->child[0]->max[0] == Approx(20.0f));
      REQUIRE(qt.root->child[1]->min[0] == Approx(0.0f));
      REQUIRE(qt.root->child[1]->max[0] == Approx(10.0f));
    }

    THEN( "buildLeafList collects two nodes" ) {
      DisplayList list;
      qt.buildLeafList(&list);
      REQUIRE(countDisplayList(&list) == 2);
    }

    freeQuadTreeNode(qt.root);
    qt.root = NULL;
  }

  GIVEN( "A 10x20 strip (split along Z only)" ) {
    QuadTree qt;
    qt.buildTree(qt.root, 0.0f, 10.0f, 0.0f, 20.0f, 10.0f);

    THEN( "two children along Z, both leaves" ) {
      REQUIRE(qt.root->child[0] == NULL);
      REQUIRE(qt.root->child[1] == NULL);
      REQUIRE(qt.root->child[2] != NULL);
      REQUIRE(qt.root->child[3] != NULL);
      REQUIRE(isLeaf(qt.root->child[2]));
      REQUIRE(isLeaf(qt.root->child[3]));
      REQUIRE(qt.root->child[2]->min[1] == Approx(0.0f));
      REQUIRE(qt.root->child[2]->max[1] == Approx(10.0f));
      REQUIRE(qt.root->child[3]->min[1] == Approx(10.0f));
      REQUIRE(qt.root->child[3]->max[1] == Approx(20.0f));
    }

    THEN( "buildLeafList collects two nodes" ) {
      DisplayList list;
      qt.buildLeafList(&list);
      REQUIRE(countDisplayList(&list) == 2);
    }

    freeQuadTreeNode(qt.root);
    qt.root = NULL;
  }
}

SCENARIO( "QuadTreeNode default construction", "[QuadTreeNode]" ) {
  GIVEN( "A default node" ) {
    QuadTreeNode n;

    THEN( "children are null and bounds are zero" ) {
      for (int i = 0; i < 4; ++i) {
        REQUIRE(n.child[i] == NULL);
      }
      REQUIRE(n.min[0] == Approx(0.0f));
      REQUIRE(n.max[0] == Approx(0.0f));
      REQUIRE(n.next == NULL);
    }
  }
}
