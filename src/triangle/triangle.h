#pragma once

#include <SDL.h>

#include "vector3/vector3.h"

struct Matrix;

struct Triangle {
  Vector3 point[3];
  SDL_Color color;

  Triangle();

  Triangle(Vector3 v1, Vector3 v2, Vector3 v3);

  void render(SDL_Renderer* renderer);
};
