#include "matrix.h"

float& Matrix::operator()(int row, int col) { return m[row + 4 * col]; }
