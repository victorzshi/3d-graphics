#pragma once

#include "vector3/vector3.h"

// 4x4 matrix.
struct Matrix {
  float m[16] = {0.0f};

  float& operator()(int row, int col);
};
