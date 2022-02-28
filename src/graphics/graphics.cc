#include "graphics.h"

#include <SDL.h>

#include <iostream>
#include <vector>

#include "matrix/matrix.h"
#include "mesh/mesh.h"
#include "triangle/triangle.h"
#include "vector3/vector3.h"

Graphics::Graphics() : isRunning_(true) {
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    std::cerr << "SDL could not initialize! SDL error: " << SDL_GetError()
              << std::endl;
  }

  window_ = SDL_CreateWindow("Graphics 3D", SDL_WINDOWPOS_UNDEFINED,
                             SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
                             SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
  if (window_ == nullptr) {
    std::cerr << "Window was not created! SDL error: " << SDL_GetError()
              << std::endl;
  }

  renderer_ = SDL_CreateRenderer(
      window_, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (renderer_ == nullptr) {
    std::cerr << "Renderer was not created! SDL error: " << SDL_GetError()
              << std::endl;
  }
}

Graphics::~Graphics() {
  SDL_DestroyWindow(window_);
  SDL_Quit();
}

void Graphics::run() {
  Mesh cube = Mesh::cube();

  // Set up projection matrix
  float near = 0.1f;
  float far = 100.0f;
  float fov = 90.0f;
  float d = 1.0f / tanf(fov * 0.5f * 3.14159f / 180.f);  // Distance to plane
  float a = static_cast<float>(SCREEN_WIDTH) / SCREEN_HEIGHT;  // Aspect ratio

  Matrix projection;
  projection(0, 0) = d / a;
  projection(1, 1) = d;
  projection(2, 2) = far / (far - near);
  projection(2, 3) = -(far * near) / (far - near);
  projection(3, 2) = 1.0f;

  SDL_Event event;
  Uint64 previous = SDL_GetTicks64();

  while (isRunning_) {
    while (SDL_PollEvent(&event) != 0) {
      if (event.type == SDL_QUIT) {
        isRunning_ = false;
        return;
      }
    }

    Uint64 current = SDL_GetTicks64();
    Uint64 elapsed = current - previous;

    // Build rotation matrix
    float theta = static_cast<float>(elapsed) / 1000.0f;

    Matrix rotationZ;
    rotationZ(0, 0) = cosf(theta);
    rotationZ(0, 1) = sinf(theta);
    rotationZ(1, 0) = -sinf(theta);
    rotationZ(1, 1) = cosf(theta);
    rotationZ(2, 2) = 1;
    rotationZ(3, 3) = 1;

    Matrix rotationX;
    rotationX(0, 0) = 1;
    rotationX(1, 1) = cosf(theta * 0.5f);
    rotationX(1, 2) = sinf(theta * 0.5f);
    rotationX(2, 1) = -sinf(theta * 0.5f);
    rotationX(2, 2) = cosf(theta * 0.5f);
    rotationX(3, 3) = 1;

    Matrix rotation = rotationX * rotationZ;

    SDL_SetRenderDrawColor(renderer_, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer_);

    SDL_SetRenderDrawColor(renderer_, 255, 255, 255, SDL_ALPHA_OPAQUE);
    for (auto triangle : cube.triangles) {
      Triangle rotated = triangle.multiply(rotation);

      Vector3 translation = Vector3(0.0f, 0.0f, 20.0f);
      Triangle translated = rotated.translate(translation);

      Triangle projected = translated.multiply(projection);

      // Scale into view
      translation = Vector3(1.0f, 1.0f, 0.0f);
      projected = projected.translate(translation);

      projected.vertex[0].x *= 0.5f * static_cast<float>(SCREEN_WIDTH);
      projected.vertex[0].y *= 0.5f * static_cast<float>(SCREEN_HEIGHT);
      projected.vertex[1].x *= 0.5f * static_cast<float>(SCREEN_WIDTH);
      projected.vertex[1].y *= 0.5f * static_cast<float>(SCREEN_HEIGHT);
      projected.vertex[2].x *= 0.5f * static_cast<float>(SCREEN_WIDTH);
      projected.vertex[2].y *= 0.5f * static_cast<float>(SCREEN_HEIGHT);

      projected.render(renderer_);
    }

    SDL_RenderPresent(renderer_);
  }
}
