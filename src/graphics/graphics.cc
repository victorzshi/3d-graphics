#include "graphics.h"

#include <SDL.h>

#include <iostream>
#include <vector>

#include "math/matrix44/matrix44.h"
#include "math/vector3/vector3.h"

struct Triangle {
  Vector3 point[3];

  Triangle() {
    point[0] = Vector3();
    point[1] = Vector3();
    point[2] = Vector3();
  }

  Triangle(Vector3 p1, Vector3 p2, Vector3 p3) {
    point[0] = p1;
    point[1] = p2;
    point[2] = p3;
  }
};

struct Mesh {
  std::vector<Triangle> triangles;

  static Mesh cube() {
    Mesh cube;
    Vector3 p1;
    Vector3 p2;
    Vector3 p3;
    // South
    p1 = Vector3(0.0f, 0.0f, 0.0f);
    p2 = Vector3(0.0f, 1.0f, 0.0f);
    p3 = Vector3(1.0f, 1.0f, 0.0f);
    cube.triangles.push_back(Triangle(p1, p2, p3));
    p1 = Vector3(0.0f, 0.0f, 0.0f);
    p2 = Vector3(1.0f, 1.0f, 0.0f);
    p3 = Vector3(1.0f, 0.0f, 0.0f);
    cube.triangles.push_back(Triangle(p1, p2, p3));
    // East
    p1 = Vector3(1.0f, 0.0f, 0.0f);
    p2 = Vector3(1.0f, 1.0f, 0.0f);
    p3 = Vector3(1.0f, 1.0f, 1.0f);
    cube.triangles.push_back(Triangle(p1, p2, p3));
    p1 = Vector3(1.0f, 0.0f, 0.0f);
    p2 = Vector3(1.0f, 1.0f, 1.0f);
    p3 = Vector3(1.0f, 0.0f, 1.0f);
    cube.triangles.push_back(Triangle(p1, p2, p3));
    // North
    p1 = Vector3(1.0f, 0.0f, 1.0f);
    p2 = Vector3(1.0f, 1.0f, 1.0f);
    p3 = Vector3(0.0f, 1.0f, 1.0f);
    cube.triangles.push_back(Triangle(p1, p2, p3));
    p1 = Vector3(1.0f, 0.0f, 1.0f);
    p2 = Vector3(0.0f, 1.0f, 1.0f);
    p3 = Vector3(0.0f, 0.0f, 1.0f);
    cube.triangles.push_back(Triangle(p1, p2, p3));
    // West
    p1 = Vector3(0.0f, 0.0f, 1.0f);
    p2 = Vector3(0.0f, 1.0f, 1.0f);
    p3 = Vector3(0.0f, 1.0f, 0.0f);
    cube.triangles.push_back(Triangle(p1, p2, p3));
    p1 = Vector3(0.0f, 0.0f, 1.0f);
    p2 = Vector3(0.0f, 1.0f, 0.0f);
    p3 = Vector3(0.0f, 0.0f, 0.0f);
    cube.triangles.push_back(Triangle(p1, p2, p3));
    // Top
    p1 = Vector3(0.0f, 1.0f, 0.0f);
    p2 = Vector3(0.0f, 1.0f, 1.0f);
    p3 = Vector3(1.0f, 1.0f, 1.0f);
    cube.triangles.push_back(Triangle(p1, p2, p3));
    p1 = Vector3(0.0f, 1.0f, 0.0f);
    p2 = Vector3(1.0f, 1.0f, 1.0f);
    p3 = Vector3(1.0f, 1.0f, 0.0f);
    cube.triangles.push_back(Triangle(p1, p2, p3));
    // Bottom
    p1 = Vector3(1.0f, 0.0f, 1.0f);
    p2 = Vector3(0.0f, 0.0f, 1.0f);
    p3 = Vector3(0.0f, 0.0f, 0.0f);
    cube.triangles.push_back(Triangle(p1, p2, p3));
    p1 = Vector3(1.0f, 0.0f, 1.0f);
    p2 = Vector3(0.0f, 0.0f, 0.0f);
    p3 = Vector3(1.0f, 0.0f, 0.0f);
    cube.triangles.push_back(Triangle(p1, p2, p3));

    return cube;
  }
};

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
      rotated.point[0] = rotationZ.multiply(triangle.point[0]);
      rotated.point[1] = rotationZ.multiply(triangle.point[1]);
      rotated.point[2] = rotationZ.multiply(triangle.point[2]);

      rotated.point[0] = rotationX.multiply(rotated.point[0]);
      rotated.point[1] = rotationX.multiply(rotated.point[1]);
      rotated.point[2] = rotationX.multiply(rotated.point[2]);

      Triangle translated = rotated;
      translated.point[0].z += 3.0f;
      translated.point[1].z += 3.0f;
      translated.point[2].z += 3.0f;

      Triangle projected;
      projected.point[0] = projection.multiply(translated.point[0]);
      projected.point[1] = projection.multiply(translated.point[1]);
      projected.point[2] = projection.multiply(translated.point[2]);

      // Scale into view
      projected.point[0] += Vector3(1.0f, 1.0f, 0.0f);
      projected.point[1] += Vector3(1.0f, 1.0f, 0.0f);
      projected.point[2] += Vector3(1.0f, 1.0f, 0.0f);

      projected.point[0].x *= 0.5f * static_cast<float>(SCREEN_WIDTH);
      projected.point[0].y *= 0.5f * static_cast<float>(SCREEN_HEIGHT);
      projected.point[1].x *= 0.5f * static_cast<float>(SCREEN_WIDTH);
      projected.point[1].y *= 0.5f * static_cast<float>(SCREEN_HEIGHT);
      projected.point[2].x *= 0.5f * static_cast<float>(SCREEN_WIDTH);
      projected.point[2].y *= 0.5f * static_cast<float>(SCREEN_HEIGHT);

      int x1 = static_cast<int>(roundf(projected.point[0].x));
      int y1 = static_cast<int>(roundf(projected.point[0].y));
      int x2 = static_cast<int>(roundf(projected.point[1].x));
      int y2 = static_cast<int>(roundf(projected.point[1].y));
      SDL_RenderDrawLine(renderer_, x1, y1, x2, y2);
      x1 = static_cast<int>(roundf(projected.point[1].x));
      y1 = static_cast<int>(roundf(projected.point[1].y));
      x2 = static_cast<int>(roundf(projected.point[2].x));
      y2 = static_cast<int>(roundf(projected.point[2].y));
      SDL_RenderDrawLine(renderer_, x1, y1, x2, y2);
      x1 = static_cast<int>(roundf(projected.point[2].x));
      y1 = static_cast<int>(roundf(projected.point[2].y));
      x2 = static_cast<int>(roundf(projected.point[0].x));
      y2 = static_cast<int>(roundf(projected.point[0].y));
      SDL_RenderDrawLine(renderer_, x1, y1, x2, y2);
    }

    SDL_RenderPresent(renderer_);
  }
}
