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

  GIVEN( "Copy and assignment" ) {
    Matrix a;
    populate_matrix(a);
    Matrix b(a);

    THEN( "copy constructor duplicates data" ) {
      for (int i = 0; i < NUM_CELLS; ++i) {
        REQUIRE(b.data[i] == a.data[i]);
      }
    }

    Matrix c;
    c = a;
    THEN( "assignment duplicates data" ) {
      for (int i = 0; i < NUM_CELLS; ++i) {
        REQUIRE(c.data[i] == a.data[i]);
      }
    }
  }

  GIVEN( "Identity times translation" ) {
    Matrix i;
    Matrix t;
    t.setTranslation(5.0f, -2.0f, 7.0f);
    Matrix r = i * t;

    THEN( "product is the translation matrix" ) {
      REQUIRE(r.data[12] == Approx(5.0f));
      REQUIRE(r.data[13] == Approx(-2.0f));
      REQUIRE(r.data[14] == Approx(7.0f));
      REQUIRE(r.data[0] == Approx(1.0f));
      REQUIRE(r.data[15] == Approx(1.0f));
    }
  }

  GIVEN( "Translation transforms a vertex with perspective divide" ) {
    Matrix t;
    t.setTranslation(1.0f, 2.0f, 3.0f);
    float v[3] = { 0.0f, 0.0f, 0.0f };
    float out[3];
    t.transformVertex(v, out);

    THEN( "origin maps to translation" ) {
      REQUIRE(out[0] == Approx(1.0f));
      REQUIRE(out[1] == Approx(2.0f));
      REQUIRE(out[2] == Approx(3.0f));
    }
  }

  GIVEN( "Explicit 16-element constructor (four columns: c1x,c2x,c3x,c4x each)" ) {
    // Matches Matrix(c11,c21,c31,c41, c12,..., c44) -> OpenGL column-major data[].
    Matrix m(
      1.0f, 0.0f, 0.0f, 0.0f,
      0.0f, 2.0f, 0.0f, 0.0f,
      0.0f, 0.0f, 3.0f, 0.0f,
      4.0f, 5.0f, 6.0f, 1.0f
    );

    THEN( "diagonal and last-column translation match constructor" ) {
      REQUIRE(m.data[0] == Approx(1.0f));
      REQUIRE(m.data[5] == Approx(2.0f));
      REQUIRE(m.data[10] == Approx(3.0f));
      REQUIRE(m.data[3] == Approx(4.0f));
      REQUIRE(m.data[7] == Approx(5.0f));
      REQUIRE(m.data[11] == Approx(6.0f));
      REQUIRE(m.data[15] == Approx(1.0f));
    }
  }

  GIVEN( "Uniform scale matrix" ) {
    Matrix s;
    s.setScale(2.0f, 3.0f, 4.0f);

    THEN( "diagonal holds scale factors" ) {
      REQUIRE(s.data[0] == Approx(2.0f));
      REQUIRE(s.data[5] == Approx(3.0f));
      REQUIRE(s.data[10] == Approx(4.0f));
      REQUIRE(s.data[15] == Approx(1.0f));
    }
  }
}
