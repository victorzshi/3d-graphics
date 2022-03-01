#pragma once

#include <string>

struct Vector3 {
  // 3D basis, plus extra info
  float x, y, z, w;

  Vector3();

  Vector3(float x, float y, float z);

  float length() const;

  float lengthSquared() const;

  Vector3 normalize() const;

  float dot(const Vector3& other) const;

  Vector3 cross(const Vector3& other) const;

  Vector3 operator+(const Vector3& other) const;

  Vector3& operator+=(const Vector3& other);

  Vector3 operator-(const Vector3& other) const;

  Vector3& operator-=(const Vector3& other);

  Vector3 operator*(float scalar) const;

  Vector3 operator*=(float scalar);

  Vector3 operator/(float scalar) const;

  Vector3 operator/=(float scalar);

  bool operator==(const Vector3& other) const;

  bool operator!=(const Vector3& other) const;

  static bool equals(float a, float b, float epsilon = 1.0f / 1000);

  std::string str() const;
};
