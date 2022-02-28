#pragma once

struct Vector3;

// 4x4 matrix.
struct Matrix {
  float m[16];

  Matrix();

  explicit Matrix(float n);

  static Matrix rotationX(float theta);
  static Matrix rotationY(float theta);
  static Matrix rotationZ(float theta);

  float& operator()(int row, int col);

  Matrix operator*(const Matrix& other) const;

  Vector3 operator*(const Vector3& v) const;
};
