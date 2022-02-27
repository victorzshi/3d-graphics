#include "matrix44.h"

#include <assert.h>

float& Matrix44::operator()(int row, int col) { return data[row + 4 * col]; }

Vector3 Matrix44::multiply(Vector3& v) const {
  Vector3 u;
  u.x = v.x * data[0] + v.y * data[1] + v.z * data[2] + data[3];
  u.y = v.x * data[4] + v.y * data[5] + v.z * data[6] + data[7];
  u.z = v.x * data[8] + v.y * data[9] + v.z * data[10] + data[11];
  float w = v.x * data[12] + v.y * data[13] + v.z * data[14] + data[15];

  if (w != 0.0f) {
    u.x /= w;
    u.y /= w;
    u.z /= w;
  }

  return u;
}
