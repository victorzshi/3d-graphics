#include "triangle.h"

#include <SDL.h>

Triangle::Triangle() {
  v[0] = Vector3();
  v[1] = Vector3();
  v[2] = Vector3();
}

Triangle::Triangle(Vector3 v1, Vector3 v2, Vector3 v3) {
  v[0] = v1;
  v[1] = v2;
  v[2] = v3;
}

void Triangle::render(SDL_Renderer* renderer) {
  for (int i = 0; i < 3; i++) {
    int a = i;
    int b = (i + 1) % 3;
    int x1 = static_cast<int>(roundf(v[a].x));
    int y1 = static_cast<int>(roundf(v[a].y));
    int x2 = static_cast<int>(roundf(v[b].x));
    int y2 = static_cast<int>(roundf(v[b].y));
    SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
  }
}
