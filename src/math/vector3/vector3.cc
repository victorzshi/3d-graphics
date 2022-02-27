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
  return *this / this->length();
}

Vector3 Vector3::limit(float n) const {
  if (this->lengthSquared() > n * n) {
    return this->normalize() * n;
  }
  return *this;
}

Vector3 Vector3::operator+(const Vector3& v) const {
  return Vector3(this->x + v.x, this->y + v.y, this->z + v.z);
}

Vector3& Vector3::operator+=(const Vector3& v) {
  x += v.x;
  y += v.y;
  z += v.z;
  return *this;
}

Vector3 Vector3::operator-(const Vector3& v) const {
  return Vector3(this->x - v.x, this->y - v.y, this->z - v.z);
}

Vector3& Vector3::operator-=(const Vector3& v) {
  x -= v.x;
  y -= v.y;
  z -= v.z;
  return *this;
}

Vector3 Vector3::operator*(float n) const {
  return Vector3(this->x * n, this->y * n, this->z * n);
}

Vector3 Vector3::operator*=(float n) {
  x *= n;
  y *= n;
  z *= n;
  return *this;
}

Vector3 Vector3::operator/(float n) const {
  return Vector3(this->x / n, this->y / n, this->z / n);
}

Vector3 Vector3::operator/=(float n) {
  x /= n;
  y /= n;
  z /= n;
  return *this;
}

bool Vector3::operator==(const Vector3& v) const {
  return equals(x, v.x) && equals(y, v.y) && equals(z, v.z);
}

bool Vector3::operator!=(const Vector3& v) const {
  return !equals(x, v.x) || !equals(y, v.y) || !equals(z, v.z);
}

bool Vector3::equals(float a, float b, float epsilon) {
  return abs(a - b) <= epsilon * (abs(a) + abs(b) + 1.0f);
}

void Vector3::print() {
  std::cout << "(" << x << ", " << y << ", " << z << ")" << std::endl;
}
