#pragma once

#include <string>
#include <vector>

#include "triangle/triangle.h"
#include "vector3/vector3.h"

struct Mesh {
  std::vector<Triangle> triangles;

  void loadFromObjectFile(std::string file);
};
