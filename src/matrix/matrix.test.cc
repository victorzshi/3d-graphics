#include "matrix.h"

#include <catch2/catch.hpp>

#include "vector3/vector3.h"

Matrix a = Matrix(2.0f);
Matrix b = Matrix(3.0f);

TEST_CASE("Matrix multiply matrix") {
  Matrix c = a * b;

  REQUIRE(c(0, 0) == 24.0f);
  REQUIRE(c(0, 1) == 24.0f);
  REQUIRE(c(0, 2) == 24.0f);
  REQUIRE(c(0, 3) == 24.0f);

  REQUIRE(c(1, 0) == 24.0f);
  REQUIRE(c(1, 1) == 24.0f);
  REQUIRE(c(1, 2) == 24.0f);
  REQUIRE(c(1, 3) == 24.0f);

  REQUIRE(c(2, 0) == 24.0f);
  REQUIRE(c(2, 1) == 24.0f);
  REQUIRE(c(2, 2) == 24.0f);
  REQUIRE(c(2, 3) == 24.0f);

  REQUIRE(c(3, 0) == 24.0f);
  REQUIRE(c(3, 1) == 24.0f);
  REQUIRE(c(3, 2) == 24.0f);
  REQUIRE(c(3, 3) == 24.0f);
}

TEST_CASE("Matrix multiply vector") {
  Vector3 u = Vector3(3.0f, 3.0f, 3.0f);
  Vector3 v = a * u;

  REQUIRE(v == Vector3(18.0f, 18.0f, 18.0f));
  REQUIRE(v.w == 18.0f);
}
