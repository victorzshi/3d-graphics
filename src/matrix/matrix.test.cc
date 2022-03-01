#include "matrix.h"

#include <catch2/catch.hpp>

#include "vector3/vector3.h"

Matrix a = Matrix(2.0f);
Matrix b = Matrix(3.0f);
float pi = atan(1) * 4.0f;

TEST_CASE("Matrix initializes to 0") {
  Matrix c = Matrix();

  REQUIRE(c(0, 0) == 0.0f);
  REQUIRE(c(0, 1) == 0.0f);
  REQUIRE(c(0, 2) == 0.0f);
  REQUIRE(c(0, 3) == 0.0f);
  REQUIRE(c(1, 0) == 0.0f);
  REQUIRE(c(1, 1) == 0.0f);
  REQUIRE(c(1, 2) == 0.0f);
  REQUIRE(c(1, 3) == 0.0f);
  REQUIRE(c(2, 0) == 0.0f);
  REQUIRE(c(2, 1) == 0.0f);
  REQUIRE(c(2, 2) == 0.0f);
  REQUIRE(c(2, 3) == 0.0f);
  REQUIRE(c(3, 0) == 0.0f);
  REQUIRE(c(3, 1) == 0.0f);
  REQUIRE(c(3, 2) == 0.0f);
  REQUIRE(c(3, 3) == 0.0f);
}

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

  REQUIRE(v == Vector3(20.0f, 20.0f, 20.0f));
  REQUIRE(v.w == 20.0f);
}

TEST_CASE("Matrix translate") {
  Vector3 v = Vector3(5.0f, 5.0f, 5.0f);
  Matrix m = Matrix::translate(v);

  REQUIRE(m(0, 0) == 1.0f);
  REQUIRE(m(1, 1) == 1.0f);
  REQUIRE(m(2, 2) == 1.0f);
  REQUIRE(m(3, 3) == 1.0f);
  REQUIRE(m(3, 0) == v.x);
  REQUIRE(m(3, 1) == v.y);
  REQUIRE(m(3, 2) == v.z);
}

TEST_CASE("Matrix rotate X") {
  Matrix m = Matrix::rotateX(pi);

  REQUIRE(m(0, 0) == 1.0f);
  REQUIRE(m(1, 1) == cosf(pi));
  REQUIRE(m(1, 2) == sinf(pi));
  REQUIRE(m(2, 1) == -sinf(pi));
  REQUIRE(m(2, 2) == cosf(pi));
}

TEST_CASE("Matrix rotate Y") {
  Matrix m = Matrix::rotateY(pi);

  REQUIRE(m(0, 0) == cosf(pi));
  REQUIRE(m(0, 2) == -sinf(pi));
  REQUIRE(m(1, 1) == 1.0f);
  REQUIRE(m(2, 0) == sinf(pi));
  REQUIRE(m(2, 2) == cosf(pi));
}

TEST_CASE("Matrix rotate Z") {
  Matrix m = Matrix::rotateZ(pi);

  REQUIRE(m(0, 0) == cosf(pi));
  REQUIRE(m(0, 1) == sinf(pi));
  REQUIRE(m(1, 0) == -sinf(pi));
  REQUIRE(m(1, 1) == cosf(pi));
  REQUIRE(m(2, 2) == 1.0f);
}

TEST_CASE("Matrix scale") {
  float x = 1.0f;
  float y = 2.0f;
  float z = 3.0f;
  Matrix m = Matrix::scale(x, y, z);

  REQUIRE(m(0, 0) == x);
  REQUIRE(m(1, 1) == y);
  REQUIRE(m(2, 2) == z);
  REQUIRE(m(3, 3) == 1.0f);
}
