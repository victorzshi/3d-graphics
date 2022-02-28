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

Triangle Triangle::multiply(Matrix& m) const {
  Triangle triangle;

  for (int i = 0; i < 3; i++) {
    Vector3 u;

    u.x = point[i].x * m(0, 0) + point[i].y * m(1, 0) + point[i].z * m(2, 0) +
          m(3, 0);
    u.y = point[i].x * m(0, 1) + point[i].y * m(1, 1) + point[i].z * m(2, 1) +
          m(3, 1);
    u.z = point[i].x * m(0, 2) + point[i].y * m(1, 2) + point[i].z * m(2, 2) +
          m(3, 2);
    float w = point[i].x * m(0, 3) + point[i].y * m(1, 3) +
              point[i].z * m(2, 3) + m(3, 3);

    if (w != 0.0f) {
      u.x /= w;
      u.y /= w;
      u.z /= w;
    }

    triangle.point[i] = u;
  }

  return triangle;
}

Triangle Triangle::translate(Vector3& v) const {
  Triangle triangle = *this;

  for (int i = 0; i < 3; i++) {
    triangle.point[i] += v;
  }

  return triangle;
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
