#include "vector3.h"

#include <assert.h>

#include <iostream>

Vector3::Vector3() : x(0.0f), y(0.0f), z(0.0f) {}

Vector3::Vector3(float newX, float newY, float newZ)
    : x(newX), y(newY), z(newZ) {}

float Vector3::length() const { return sqrt(x * x + y * y + z * z); }

float Vector3::lengthSquared() const { return x * x + y * y + z * z; }

Vector3 Vector3::normalize() const {
  assert(this->lengthSquared() > 0);
  Vector3 v = *this / this->length();
  return v;
}

Vector3 Vector3::limit(float n) const {
  Vector3 v = *this;
  if (this->lengthSquared() > n * n) {
    v = this->normalize() * n;
  }
  return v;
}

Vector3 Vector3::operator+(const Vector3& v) const {
  Vector3 u;
  u.x = this->x + v.x;
  u.y = this->y + v.y;
  u.z = this->z + v.z;
  return u;
}

Vector3& Vector3::operator+=(const Vector3& v) {
  x += v.x;
  y += v.y;
  z += v.z;
  return *this;
}

Vector3 Vector3::operator-(const Vector3& v) const {
  Vector3 u;
  u.x = this->x - v.x;
  u.y = this->y - v.y;
  u.z = this->z - v.z;
  return u;
}

Vector3& Vector3::operator-=(const Vector3& v) {
  x -= v.x;
  y -= v.y;
  z -= v.z;
  return *this;
}

Vector3 Vector3::operator*(float n) const {
  Vector3 u;
  u.x = this->x * n;
  u.y = this->y * n;
  u.z = this->z * n;
  return u;
}

Vector3 Vector3::operator*=(float n) {
  x *= n;
  y *= n;
  z *= n;
  return *this;
}

Vector3 Vector3::operator/(float n) const {
  Vector3 u;
  u.x = this->x / n;
  u.y = this->y / n;
  u.z = this->z / n;
  return u;
}

Vector3 Vector3::operator/=(float n) {
  x /= n;
  y /= n;
  z /= n;
  return *this;
}

bool Vector3::operator==(const Vector3& v) const {
  return almostEquals(x, v.x) && almostEquals(y, v.y) && almostEquals(z, v.z);
}

bool Vector3::operator!=(const Vector3& v) const {
  return !almostEquals(x, v.x) || !almostEquals(y, v.y) ||
         !almostEquals(z, v.z);
}

bool Vector3::almostEquals(float a, float b, float epsilon) {
  return abs(a - b) <= epsilon * (abs(a) + abs(b) + 1.0f);
}

void Vector3::print() {
  std::cout << "(" << x << ", " << y << ", " << z << ")" << std::endl;
}
