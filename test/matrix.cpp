#include "catch.hpp"
#include "Matrix.h"

void validate_identity_matrix(Matrix &m) {
  for (int i = 0; i < NUM_CELLS; ++i) {
    int expected = i % 5 == 0;
    REQUIRE(m.data[i] == expected);
  }
}

void populate_matrix(Matrix &m) {
  for (int i = 0; i < NUM_CELLS; ++i) {
    m.data[i] = i;
  }
}

SCENARIO( "Manipulating a matrix", "[matrix]" ) {
  GIVEN( "A populated matrix" ) {
    Matrix m;

    // Matrix is defaulted to the identity matrix
    validate_identity_matrix(m);

    populate_matrix(m);

    WHEN( "the matrix is zeroed" ) {
      m.loadZero();

      THEN( "all cells contain 0" ) {
        for (int i = 0; i < NUM_CELLS; ++i) {
          REQUIRE(m.data[i] == 0);
        }
      }
    }

    WHEN( "the identity matrix is loaded" ) {
      m.loadIdentity();

      THEN( "the matrix contains the identify matrix" ) {
        validate_identity_matrix(m);
      }
    }
  }
}
