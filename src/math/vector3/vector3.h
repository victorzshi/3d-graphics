#pragma once

struct Vector3 {
  float x, y, z;

  Vector3();

  Vector3(float x, float y, float z);

  float length() const;

  float lengthSquared() const;

  Vector3 normalize() const;

  Vector3 limit(float n) const;

  // TODO(Victor): Dot product.

  // TODO(Victor): Cross product.

  Vector3 operator+(const Vector3& v) const;

  Vector3& operator+=(const Vector3& v);

  Vector3 operator-(const Vector3& v) const;

  Vector3& operator-=(const Vector3& v);

  Vector3 operator*(float n) const;

  Vector3 operator*=(float n);

  Vector3 operator/(float n) const;

  Vector3 operator/=(float n);

  bool operator==(const Vector3& v) const;

  bool operator!=(const Vector3& v) const;

  static bool equals(float a, float b, float epsilon = 1.0f / 1000);

  void print();
};
