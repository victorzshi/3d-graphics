#pragma once

struct Vector3;

// 4x4 matrix.
struct Matrix {
  float m[16];

  Matrix();

  explicit Matrix(float n);

  float& operator()(int row, int col);

  Matrix operator*(const Matrix& other) const;

  Vector3 operator*(const Vector3& v) const;

  static Matrix identity();
  static Matrix translate(Vector3 v);
  static Matrix rotateX(float theta);
  static Matrix rotateY(float theta);
  static Matrix rotateZ(float theta);
  static Matrix scale(float x, float y, float z);
  static Matrix projection(float distance, float aspect, float near, float far);
  static Matrix pointAt(Vector3& position, Vector3& target, Vector3& up);
  static Matrix quickInverse(Matrix& m);
};
