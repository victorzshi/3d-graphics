#include "mesh.h"

#include <fstream>
#include <iostream>
#include <strstream>

void Mesh::loadFromObjectFile(std::string file) {
  std::string path = "../../data/objects/" + file;

  std::ifstream ifs(path);
  if (!ifs.is_open()) {
    std::cerr << "Mesh could not load file! Path: " << path << std::endl;
  }

  std::vector<Vector3> vertices;

  while (ifs.good()) {
    char line[64];
    ifs.getline(line, 64);

    std::strstream s;
    s << line;

    char junk;
    if (line[0] == 'v') {
      Vector3 v;
      s >> junk >> v.x >> v.y >> v.z;
      vertices.push_back(v);
    } else if (line[0] == 'f') {
      size_t f[3];
      s >> junk >> f[0] >> f[1] >> f[2];
      Vector3 p1 = vertices[f[0] - 1];
      Vector3 p2 = vertices[f[1] - 1];
      Vector3 p3 = vertices[f[2] - 1];
      triangles.push_back(Triangle(p1, p2, p3));
    }
  }
}
