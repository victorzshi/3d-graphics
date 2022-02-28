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
