#pragma once

#include <SDL.h>

#include "vector3/vector3.h"

struct Matrix;

struct Triangle {
  Vector3 point[3];
  SDL_Color color;

  Triangle();

  Triangle(Vector3 p1, Vector3 p2, Vector3 p3);

  void setColor(float dot);

  void render(SDL_Renderer* renderer);
};
