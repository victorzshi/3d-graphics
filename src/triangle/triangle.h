#pragma once

#include "vector3/vector3.h"

struct SDL_Renderer;

struct Triangle {
  Vector3 v[3];

  Triangle();

  Triangle(Vector3 v1, Vector3 v2, Vector3 v3);

  void render(SDL_Renderer* renderer);
};
