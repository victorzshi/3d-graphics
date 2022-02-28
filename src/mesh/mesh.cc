#include "mesh.h"

#include <fstream>
#include <iostream>
#include <strstream>

bool Mesh::loadFromObjectFile(std::string file) {
  std::ifstream ifs("../../data/" + file);
  if (!ifs.is_open()) {
    std::cerr << "Mesh could not load! File: " << file << std::endl;
    return false;
  }

  std::vector<Vector3> vertices;

  while (!ifs.eof()) {
    char line[32];
    ifs.getline(line, 32);

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

  return true;
}

Mesh Mesh::cube() {
  Mesh mesh;

  // South
  Vector3 p1 = Vector3(0.0f, 0.0f, 0.0f);
  Vector3 p2 = Vector3(0.0f, 1.0f, 0.0f);
  Vector3 p3 = Vector3(1.0f, 1.0f, 0.0f);
  mesh.triangles.push_back(Triangle(p1, p2, p3));
  p1 = Vector3(0.0f, 0.0f, 0.0f);
  p2 = Vector3(1.0f, 1.0f, 0.0f);
  p3 = Vector3(1.0f, 0.0f, 0.0f);
  mesh.triangles.push_back(Triangle(p1, p2, p3));

  // East
  p1 = Vector3(1.0f, 0.0f, 0.0f);
  p2 = Vector3(1.0f, 1.0f, 0.0f);
  p3 = Vector3(1.0f, 1.0f, 1.0f);
  mesh.triangles.push_back(Triangle(p1, p2, p3));
  p1 = Vector3(1.0f, 0.0f, 0.0f);
  p2 = Vector3(1.0f, 1.0f, 1.0f);
  p3 = Vector3(1.0f, 0.0f, 1.0f);
  mesh.triangles.push_back(Triangle(p1, p2, p3));

  // North
  p1 = Vector3(1.0f, 0.0f, 1.0f);
  p2 = Vector3(1.0f, 1.0f, 1.0f);
  p3 = Vector3(0.0f, 1.0f, 1.0f);
  mesh.triangles.push_back(Triangle(p1, p2, p3));
  p1 = Vector3(1.0f, 0.0f, 1.0f);
  p2 = Vector3(0.0f, 1.0f, 1.0f);
  p3 = Vector3(0.0f, 0.0f, 1.0f);
  mesh.triangles.push_back(Triangle(p1, p2, p3));

  // West
  p1 = Vector3(0.0f, 0.0f, 1.0f);
  p2 = Vector3(0.0f, 1.0f, 1.0f);
  p3 = Vector3(0.0f, 1.0f, 0.0f);
  mesh.triangles.push_back(Triangle(p1, p2, p3));
  p1 = Vector3(0.0f, 0.0f, 1.0f);
  p2 = Vector3(0.0f, 1.0f, 0.0f);
  p3 = Vector3(0.0f, 0.0f, 0.0f);
  mesh.triangles.push_back(Triangle(p1, p2, p3));

  // Top
  p1 = Vector3(0.0f, 1.0f, 0.0f);
  p2 = Vector3(0.0f, 1.0f, 1.0f);
  p3 = Vector3(1.0f, 1.0f, 1.0f);
  mesh.triangles.push_back(Triangle(p1, p2, p3));
  p1 = Vector3(0.0f, 1.0f, 0.0f);
  p2 = Vector3(1.0f, 1.0f, 1.0f);
  p3 = Vector3(1.0f, 1.0f, 0.0f);
  mesh.triangles.push_back(Triangle(p1, p2, p3));

  // Bottom
  p1 = Vector3(1.0f, 0.0f, 1.0f);
  p2 = Vector3(0.0f, 0.0f, 1.0f);
  p3 = Vector3(0.0f, 0.0f, 0.0f);
  mesh.triangles.push_back(Triangle(p1, p2, p3));
  p1 = Vector3(1.0f, 0.0f, 1.0f);
  p2 = Vector3(0.0f, 0.0f, 0.0f);
  p3 = Vector3(1.0f, 0.0f, 0.0f);
  mesh.triangles.push_back(Triangle(p1, p2, p3));

  return mesh;
}
