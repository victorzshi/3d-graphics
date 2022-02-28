#pragma once

#include <SDL.h>

struct Vector3;
struct Matrix;

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

  Matrix projectionMatrix();
  Matrix rotationX(float theta);
  Matrix rotationY(float theta);
  Matrix rotationZ(float theta);
  SDL_Color getColor(Vector3& normal);
};
