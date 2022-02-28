#include "triangle.h"

#include <SDL.h>

#include "matrix/matrix.h"

Triangle::Triangle() {
  for (int i = 0; i < 3; i++) {
    vertex[i] = Vector3();
  }
}

Triangle::Triangle(Vector3 v1, Vector3 v2, Vector3 v3) {
  vertex[0] = v1;
  vertex[1] = v2;
  vertex[2] = v3;
}

Triangle Triangle::multiply(Matrix& m) const {
  Triangle triangle;

  for (int i = 0; i < 3; i++) {
    Vector3 u;

    u.x = vertex[i].x * m(0, 0) + vertex[i].y * m(1, 0) + vertex[i].z * m(2, 0) + m(3, 0);
    u.y = vertex[i].x * m(0, 1) + vertex[i].y * m(1, 1) + vertex[i].z * m(2, 1) + m(3, 1);
    u.z = vertex[i].x * m(0, 2) + vertex[i].y * m(1, 2) + vertex[i].z * m(2, 2) + m(3, 2);
    float w = vertex[i].x * m(0, 3) + vertex[i].y * m(1, 3) + vertex[i].z * m(2, 3) + m(3, 3);

    if (w != 0.0f) {
      u.x /= w;
      u.y /= w;
      u.z /= w;
    }

    triangle.vertex[i] = u;
  }

  return triangle;
}

Triangle Triangle::translate(Vector3& v) const {
  Triangle triangle = *this;

  for (int i = 0; i < 3; i++) {
    triangle.vertex[i] += v;
  }

  return triangle;
}

void Triangle::render(SDL_Renderer* renderer) {
  for (int i = 0; i < 3; i++) {
    int a = i;
    int b = (i + 1) % 3;
    int x1 = static_cast<int>(roundf(vertex[a].x));
    int y1 = static_cast<int>(roundf(vertex[a].y));
    int x2 = static_cast<int>(roundf(vertex[b].x));
    int y2 = static_cast<int>(roundf(vertex[b].y));
    SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
  }
}
