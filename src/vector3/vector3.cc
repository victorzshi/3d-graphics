#include "vector3.h"

#include <assert.h>

Vector3::Vector3() : x(0.0f), y(0.0f), z(0.0f), w(1.0f) {}

Vector3::Vector3(float newX, float newY, float newZ)
    : x(newX), y(newY), z(newZ), w(1.0f) {}

float Vector3::length() const { return sqrtf(x * x + y * y + z * z); }

float Vector3::lengthSquared() const { return x * x + y * y + z * z; }

Vector3 Vector3::normalize() const {
  assert(this->lengthSquared() > 0);
  return *this / this->length();
}

float Vector3::dot(const Vector3& other) const {
  return x * other.x + y * other.y + z * other.z;
}

Vector3 Vector3::cross(const Vector3& other) const {
  return Vector3(y * other.z - other.y * z, z * other.x - other.z * x,
                 x * other.y - other.x * y);
}

Vector3 Vector3::operator+(const Vector3& other) const {
  return Vector3(this->x + other.x, this->y + other.y, this->z + other.z);
}

Vector3& Vector3::operator+=(const Vector3& other) {
  x += other.x;
  y += other.y;
  z += other.z;
  return *this;
}

Vector3 Vector3::operator-(const Vector3& other) const {
  return Vector3(this->x - other.x, this->y - other.y, this->z - other.z);
}

Vector3& Vector3::operator-=(const Vector3& other) {
  x -= other.x;
  y -= other.y;
  z -= other.z;
  return *this;
}

Vector3 Vector3::operator*(float scalar) const {
  return Vector3(this->x * scalar, this->y * scalar, this->z * scalar);
}

Vector3 Vector3::operator*=(float scalar) {
  x *= scalar;
  y *= scalar;
  z *= scalar;
  return *this;
}

Vector3 Vector3::operator/(float scalar) const {
  return Vector3(this->x / scalar, this->y / scalar, this->z / scalar);
}

Vector3 Vector3::operator/=(float scalar) {
  x /= scalar;
  y /= scalar;
  z /= scalar;
  return *this;
}

bool Vector3::operator==(const Vector3& other) const {
  return equals(x, other.x) && equals(y, other.y) && equals(z, other.z);
}

bool Vector3::operator!=(const Vector3& other) const {
  return !equals(x, other.x) || !equals(y, other.y) || !equals(z, other.z);
}

bool Vector3::equals(float a, float b, float epsilon) {
  return abs(a - b) <= epsilon * (abs(a) + abs(b) + 1.0f);
}

std::string Vector3::str() const {
  std::string text;
  text += "(";
  text += std::to_string(x);
  text += ", ";
  text += std::to_string(y);
  text += ", ";
  text += std::to_string(z);
  text += ")";
  return text;
}
