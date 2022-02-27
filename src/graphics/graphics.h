#pragma once

struct SDL_Window;
struct SDL_Renderer;
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

  Vector3 multiply(Vector3& v, Matrix& m);
};
