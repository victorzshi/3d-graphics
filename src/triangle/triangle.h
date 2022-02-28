#pragma once

#include <SDL.h>

#include "vector3/vector3.h"

struct Matrix;

struct Triangle {
  Vector3 point[3];
  SDL_Color color;

  Triangle();

  Triangle(Vector3 v1, Vector3 v2, Vector3 v3);

  Triangle multiply(Matrix& m) const;

  Triangle translate(Vector3& v) const;

  void render(SDL_Renderer* renderer);
};
