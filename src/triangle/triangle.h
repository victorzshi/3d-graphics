#pragma once

#include "vector3/vector3.h"

struct SDL_Renderer;
struct Matrix;

struct Triangle {
  Vector3 vertex[3];

  Triangle();

  Triangle(Vector3 v1, Vector3 v2, Vector3 v3);

  Triangle multiply(Matrix& m) const;

  Triangle translate(Vector3& v) const;

  void render(SDL_Renderer* renderer);
};
