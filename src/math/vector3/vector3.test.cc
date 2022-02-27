#define CATCH_CONFIG_MAIN

#include "vector3.h"

#include <catch2/catch.hpp>

Vector3 v = Vector3(1.0f, 2.0f, 3.0f);
Vector3 w = Vector3(4.0f, 5.0f, 6.0f);

TEST_CASE("Vector3 add") {
  Vector3 u = v + w;
  REQUIRE(u == Vector3(5.0f, 7.0f, 9.0f));
  REQUIRE(u != Vector3(5.1f, 7.0f, 9.0f));
  REQUIRE(u != Vector3(5.0f, 7.1f, 9.0f));
  REQUIRE(u != Vector3(5.0f, 7.0f, 9.1f));
}

TEST_CASE("Vector3 compound add") {
  Vector3 u = v + w;
  u += u;
  REQUIRE(u == Vector3(10.0f, 14.0f, 18.0f));
  REQUIRE(u != Vector3(10.1f, 14.0f, 18.0f));
  REQUIRE(u != Vector3(10.0f, 14.1f, 18.0f));
  REQUIRE(u != Vector3(10.0f, 14.0f, 18.1f));
}

TEST_CASE("Vector3 subtract") {
  Vector3 u = v - w;
  REQUIRE(u == Vector3(-3.0f, -3.0f, -3.0f));
  REQUIRE(u != Vector3(-3.1f, -3.0f, -3.0f));
  REQUIRE(u != Vector3(-3.0f, -3.1f, -3.0f));
  REQUIRE(u != Vector3(-3.0f, -3.0f, -3.1f));
}

TEST_CASE("Vector3 compound subtract") {
  Vector3 u = v - w;
  u -= u;
  REQUIRE(u == Vector3(0.0f, 0.0f, 0.0f));
  REQUIRE(u != Vector3(0.1f, 0.0f, 0.0f));
  REQUIRE(u != Vector3(0.0f, 0.1f, 0.0f));
  REQUIRE(u != Vector3(0.0f, 0.0f, 0.1f));
}

TEST_CASE("Vector3 multiply") {
  Vector3 u = v * 2.0f;
  REQUIRE(u == Vector3(2.0f, 4.0f, 6.0f));
  REQUIRE(u != Vector3(2.1f, 4.0f, 6.0f));
  REQUIRE(u != Vector3(2.0f, 4.1f, 6.0f));
  REQUIRE(u != Vector3(2.0f, 4.0f, 6.1f));
}

TEST_CASE("Vector3 compound multiply") {
  Vector3 u = v * 2.0f;
  u *= 2.0f;
  REQUIRE(u == Vector3(4.0f, 8.0f, 12.0f));
  REQUIRE(u != Vector3(4.1f, 8.0f, 12.0f));
  REQUIRE(u != Vector3(4.0f, 8.1f, 12.0f));
  REQUIRE(u != Vector3(4.0f, 8.0f, 12.1f));
}

TEST_CASE("Vector3 divide") {
  Vector3 u = v / 2.0f;
  REQUIRE(u == Vector3(0.5f, 1.0f, 1.5f));
  REQUIRE(u != Vector3(0.6f, 1.0f, 1.5f));
  REQUIRE(u != Vector3(0.5f, 1.1f, 1.5f));
  REQUIRE(u != Vector3(0.5f, 1.0f, 1.6f));
}

TEST_CASE("Vector3 compound divide") {
  Vector3 u = v / 2.0f;
  u /= 2.0f;
  REQUIRE(u == Vector3(0.25f, 0.5f, 0.75f));
  REQUIRE(u != Vector3(0.26f, 0.5f, 0.75f));
  REQUIRE(u != Vector3(0.25f, 0.6f, 0.75f));
  REQUIRE(u != Vector3(0.25f, 0.5f, 0.76f));
}

TEST_CASE("Vector3 length") {
  float length = v.length();
  REQUIRE(Vector3::almostEquals(length, sqrt(14)));
  REQUIRE(!Vector3::almostEquals(length, sqrt(13)));
  REQUIRE(!Vector3::almostEquals(length, sqrt(15)));
}

TEST_CASE("Vector3 normalize") {
  float length = v.length();
  Vector3 u = v.normalize();
  REQUIRE(u == Vector3(v.x / length, v.y / length, v.z / length));
}

TEST_CASE("Vector3 limit") {
  float length = v.length();
  float n = 0.5f;
  Vector3 u = v.limit(n);
  REQUIRE(u == Vector3(v.x / length * n, v.y / length * n, v.z / length * n));
}
