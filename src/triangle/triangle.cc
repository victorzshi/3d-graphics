#include "triangle.h"

#include <SDL.h>

#include "matrix/matrix.h"

Triangle::Triangle() : color({255, 255, 255, 255}) {
  for (int i = 0; i < 3; i++) {
    point[i] = Vector3();
  }
}

Triangle::Triangle(Vector3 p1, Vector3 p2, Vector3 p3)
    : color({255, 255, 255, 255}) {
  point[0] = p1;
  point[1] = p2;
  point[2] = p3;
}

void Triangle::setColor(float dot) {
  if (dot > 0.9f) {
    color = {220, 220, 220, 255};
  } else if (dot > 0.8f) {
    color = {211, 211, 211, 255};
  } else if (dot > 0.7f) {
    color = {192, 192, 192, 255};
  } else if (dot > 0.6f) {
    color = {169, 169, 169, 255};
  } else if (dot > 0.5f) {
    color = {128, 128, 128, 255};
  } else {
    color = {105, 105, 105, 255};
  }
}

void Triangle::render(SDL_Renderer* renderer) {
  SDL_Vertex vertex[3];

  for (int i = 0; i < 3; i++) {
    vertex[i].position.x = point[i].x;
    vertex[i].position.y = point[i].y;
    vertex[i].color = color;
  }

  SDL_RenderGeometry(renderer, nullptr, vertex, 3, nullptr, 0);

#ifdef DEBUG
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
  for (int i = 0; i < 3; i++) {
    int a = i;
    int b = (i + 1) % 3;
    int x1 = static_cast<int>(roundf(point[a].x));
    int y1 = static_cast<int>(roundf(point[a].y));
    int x2 = static_cast<int>(roundf(point[b].x));
    int y2 = static_cast<int>(roundf(point[b].y));
    SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
  }
#endif
}
