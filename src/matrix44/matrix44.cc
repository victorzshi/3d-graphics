#include "matrix44.h"

float& Matrix44::operator()(int row, int col) { return data[row + 4 * col]; }
