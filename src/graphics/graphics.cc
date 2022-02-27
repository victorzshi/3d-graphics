#include "graphics.h"

#include <SDL.h>

#include <iostream>
#include <vector>

#include "matrix44/matrix44.h"
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

  // Projection matrix
  float nearPlane = 0.1f;
  float farPlane = 1000.0f;
  float fieldOfView = 90.0f;
  float aspectRatio = static_cast<float>(SCREEN_HEIGHT) / SCREEN_WIDTH;
  float fieldOfViewRadians =
      1.0f / tanf(fieldOfView * 0.5f / 180.0f * 3.14159f);

  Matrix44 projection;
  projection(0, 0) = aspectRatio * fieldOfViewRadians;
  projection(1, 1) = fieldOfViewRadians;
  projection(2, 2) = farPlane / (farPlane - nearPlane);
  projection(3, 2) = (-farPlane * nearPlane) / (farPlane - nearPlane);
  projection(2, 3) = 1.0f;
  projection(3, 3) = 0.0f;

  Uint64 previous = SDL_GetTicks64();

  SDL_Event event;

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

    Matrix44 rotationZ;
    rotationZ(0, 0) = cosf(theta);
    rotationZ(0, 1) = sinf(theta);
    rotationZ(1, 0) = -sinf(theta);
    rotationZ(1, 1) = cosf(theta);
    rotationZ(2, 2) = 1;
    rotationZ(3, 3) = 1;

    Matrix44 rotationX;
    rotationX(0, 0) = 1;
    rotationX(1, 1) = cosf(theta * 0.5f);
    rotationX(1, 2) = sinf(theta * 0.5f);
    rotationX(2, 1) = -sinf(theta * 0.5f);
    rotationX(2, 2) = cosf(theta * 0.5f);
    rotationX(3, 3) = 1;

    SDL_SetRenderDrawColor(renderer_, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer_);

    SDL_SetRenderDrawColor(renderer_, 255, 255, 255, SDL_ALPHA_OPAQUE);
    for (auto triangle : cube.triangles) {
      Triangle rotated;
      rotated.v[0] = multiply(triangle.v[0], rotationZ);
      rotated.v[1] = multiply(triangle.v[1], rotationZ);
      rotated.v[2] = multiply(triangle.v[2], rotationZ);

      rotated.v[0] = multiply(rotated.v[0], rotationX);
      rotated.v[1] = multiply(rotated.v[1], rotationX);
      rotated.v[2] = multiply(rotated.v[2], rotationX);

      Triangle translated = rotated;
      translated.v[0].z += 3.0f;
      translated.v[1].z += 3.0f;
      translated.v[2].z += 3.0f;

      Triangle projected;
      projected.v[0] = multiply(translated.v[0], projection);
      projected.v[1] = multiply(translated.v[1], projection);
      projected.v[2] = multiply(translated.v[2], projection);

      // Scale into view
      projected.v[0] += Vector3(1.0f, 1.0f, 0.0f);
      projected.v[1] += Vector3(1.0f, 1.0f, 0.0f);
      projected.v[2] += Vector3(1.0f, 1.0f, 0.0f);

      projected.v[0].x *= 0.5f * static_cast<float>(SCREEN_WIDTH);
      projected.v[0].y *= 0.5f * static_cast<float>(SCREEN_HEIGHT);
      projected.v[1].x *= 0.5f * static_cast<float>(SCREEN_WIDTH);
      projected.v[1].y *= 0.5f * static_cast<float>(SCREEN_HEIGHT);
      projected.v[2].x *= 0.5f * static_cast<float>(SCREEN_WIDTH);
      projected.v[2].y *= 0.5f * static_cast<float>(SCREEN_HEIGHT);

      projected.render(renderer_);
    }

    SDL_RenderPresent(renderer_);
  }
}

Vector3 Graphics::multiply(Vector3& v, Matrix44& m) {
  Vector3 u;
  u.x = v.x * m(0, 0) + v.y * m(1, 0) + v.z * m(2, 0) + m(3, 0);
  u.y = v.x * m(0, 1) + v.y * m(1, 1) + v.z * m(2, 1) + m(3, 1);
  u.z = v.x * m(0, 2) + v.y * m(1, 2) + v.z * m(2, 2) + m(3, 2);
  float w = v.x * m(0, 3) + v.y * m(1, 3) + v.z * m(2, 3) + m(3, 3);

  if (w != 0.0f) {
    u.x /= w;
    u.y /= w;
    u.z /= w;
  }

  return u;
}
