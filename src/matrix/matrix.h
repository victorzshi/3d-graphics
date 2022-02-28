#pragma once

// 4x4 matrix.
struct Matrix {
  float m[16];

  Matrix();

  Matrix(float n);

  float& operator()(int row, int col);

  Matrix operator*(const Matrix& other) const;
};
