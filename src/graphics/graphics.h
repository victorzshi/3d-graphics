#pragma once

#include <SDL.h>

#include "matrix/matrix.h"
#include "mesh/mesh.h"
#include "triangle/triangle.h"
#include "vector3/vector3.h"

struct Clipped {
  int total;
  Triangle triangle[2];
};

class Graphics {
 public:
  Graphics();
  ~Graphics();

  void run();

 private:
  static const int SCREEN_WIDTH = 640;
  static const int SCREEN_HEIGHT = 480;
  static const int RUN_DURATION = 1000;

  bool isRunning_;

  SDL_Window* window_;
  SDL_Renderer* renderer_;

  Matrix projection_;
  Vector3 position_;
  Vector3 lookDirection_;
  Mesh mesh_;
  float yaw;

  void handleInput();

  Vector3 intersectPlane(Vector3& planePoint, Vector3 planeNormal,
                         Vector3& lineStart, Vector3& lineEnd) const;
  Clipped clipAgainstPlane(Vector3 planePoint, Vector3 planeNormal,
                           Triangle& triangle) const;
};
