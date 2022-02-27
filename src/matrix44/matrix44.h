#pragma once

#include "vector3/vector3.h"

struct Matrix44 {
  float data[16] = {0.0f};

  float& operator()(int row, int col);
};
