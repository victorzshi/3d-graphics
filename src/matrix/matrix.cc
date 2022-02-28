#include "matrix.h"

Matrix::Matrix() {
  for (int i = 0; i < 16; i++) {
    m[i] = 0.0f;
  }
}

Matrix::Matrix(float n) {
  for (int i = 0; i < 16; i++) {
    m[i] = n;
  }
}

float& Matrix::operator()(int row, int col) { return m[row + 4 * col]; }

Matrix Matrix::operator*(const Matrix& other) const {
  Matrix matrix;

  matrix(0, 0) = this->m[0] * other.m[0] + this->m[4] * other.m[1] +
                 this->m[8] * other.m[2] + this->m[12] * other.m[3];
  matrix(0, 1) = this->m[0] * other.m[4] + this->m[4] * other.m[5] +
                 this->m[8] * other.m[6] + this->m[12] * other.m[7];
  matrix(0, 2) = this->m[0] * other.m[8] + this->m[4] * other.m[9] +
                 this->m[8] * other.m[10] + this->m[12] * other.m[11];
  matrix(0, 3) = this->m[0] * other.m[12] + this->m[4] * other.m[13] +
                 this->m[8] * other.m[14] + this->m[12] * other.m[15];

  matrix(1, 0) = this->m[1] * other.m[0] + this->m[5] * other.m[1] +
                 this->m[9] * other.m[2] + this->m[13] * other.m[3];
  matrix(1, 1) = this->m[1] * other.m[4] + this->m[5] * other.m[5] +
                 this->m[9] * other.m[6] + this->m[13] * other.m[7];
  matrix(1, 2) = this->m[1] * other.m[8] + this->m[5] * other.m[9] +
                 this->m[9] * other.m[10] + this->m[13] * other.m[11];
  matrix(1, 3) = this->m[1] * other.m[12] + this->m[5] * other.m[13] +
                 this->m[9] * other.m[14] + this->m[13] * other.m[15];

  matrix(2, 0) = this->m[2] * other.m[0] + this->m[6] * other.m[1] +
                 this->m[10] * other.m[2] + this->m[14] * other.m[3];
  matrix(2, 1) = this->m[2] * other.m[4] + this->m[6] * other.m[5] +
                 this->m[10] * other.m[6] + this->m[14] * other.m[7];
  matrix(2, 2) = this->m[2] * other.m[8] + this->m[6] * other.m[9] +
                 this->m[10] * other.m[10] + this->m[14] * other.m[11];
  matrix(2, 3) = this->m[2] * other.m[12] + this->m[6] * other.m[13] +
                 this->m[10] * other.m[14] + this->m[14] * other.m[15];

  matrix(3, 0) = this->m[3] * other.m[0] + this->m[7] * other.m[1] +
                 this->m[11] * other.m[2] + this->m[15] * other.m[3];
  matrix(3, 1) = this->m[3] * other.m[4] + this->m[7] * other.m[5] +
                 this->m[11] * other.m[6] + this->m[15] * other.m[7];
  matrix(3, 2) = this->m[3] * other.m[8] + this->m[7] * other.m[9] +
                 this->m[11] * other.m[10] + this->m[15] * other.m[11];
  matrix(3, 3) = this->m[3] * other.m[12] + this->m[7] * other.m[13] +
                 this->m[11] * other.m[14] + this->m[15] * other.m[15];

  return matrix;
}
