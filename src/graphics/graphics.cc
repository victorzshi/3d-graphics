#include "graphics.h"

#include <SDL.h>

#include <algorithm>
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
  Mesh mesh;
  mesh.loadFromObjectFile("bunny.obj");

  Vector3 camera = Vector3();
  Matrix projection = projectionMatrix();

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

    float theta = static_cast<float>(elapsed) / 1000.0f;

    Matrix rotation = rotationZ(theta) * rotationX(theta);

    // Cull triangles
    std::vector<Triangle> raster;

    for (auto triangle : mesh.triangles) {
      // Apply rotation
      Triangle rotated = triangle.multiply(rotation);

      // Apply translation
      Vector3 translation = Vector3(0.0f, 0.0f, 2.0f);
      Triangle translated = rotated.translate(translation);

      // Calculate normal
      Vector3 a = translated.point[1] - translated.point[0];
      Vector3 b = translated.point[2] - translated.point[0];
      Vector3 normal = a.cross(b);
      normal = normal.normalize();

      if (normal.dot(translated.point[0] - camera) < 0) {
        // Project from 3D to 2D
        Triangle projected = translated.multiply(projection);

        // Scale into view
        translation = Vector3(1.0f, 1.0f, 0.0f);
        projected = projected.translate(translation);

        projected.point[0].x *= 0.5f * static_cast<float>(SCREEN_WIDTH);
        projected.point[0].y *= 0.5f * static_cast<float>(SCREEN_HEIGHT);
        projected.point[1].x *= 0.5f * static_cast<float>(SCREEN_WIDTH);
        projected.point[1].y *= 0.5f * static_cast<float>(SCREEN_HEIGHT);
        projected.point[2].x *= 0.5f * static_cast<float>(SCREEN_WIDTH);
        projected.point[2].y *= 0.5f * static_cast<float>(SCREEN_HEIGHT);

        // Lighting
        projected.color = getColor(normal);

        raster.push_back(projected);
      }
    }

    // Sort triangles
    std::sort(raster.begin(), raster.end(), [](Triangle& a, Triangle& b) {
      float z1 = (a.point[0].z + a.point[1].z + a.point[2].z) / 3.0f;
      float z2 = (b.point[0].z + b.point[1].z + b.point[2].z) / 3.0f;
      return z1 > z2;
    });

    SDL_SetRenderDrawColor(renderer_, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer_);

    for (auto triangle : raster) {
      triangle.render(renderer_);
    }

    SDL_RenderPresent(renderer_);
  }
}

Matrix Graphics::projectionMatrix() {
  float near = 0.1f;
  float far = 100.0f;
  float fov = 90.0f;
  float d = 1.0f / tanf(fov * 0.5f * 3.141592f / 180.f);  // Distance to plane
  float a = static_cast<float>(SCREEN_WIDTH) / SCREEN_HEIGHT;  // Aspect ratio

  Matrix matrix;
  matrix(0, 0) = d / a;
  matrix(1, 1) = d;
  matrix(2, 2) = far / (far - near);
  matrix(2, 3) = -(far * near) / (far - near);
  matrix(3, 2) = 1.0f;
  return matrix;
}

Matrix Graphics::rotationX(float theta) {
  Matrix rotation;
  rotation(0, 0) = 1;
  rotation(1, 1) = cosf(theta);
  rotation(1, 2) = sinf(theta);
  rotation(2, 1) = -sinf(theta);
  rotation(2, 2) = cosf(theta);
  return rotation;
}

Matrix Graphics::rotationY(float theta) {
  Matrix rotation;
  rotation(0, 0) = cosf(theta);
  rotation(0, 2) = sinf(theta);
  rotation(1, 1) = 1.0f;
  rotation(2, 0) = -sinf(theta);
  rotation(2, 2) = cosf(theta);
  return rotation;
}

Matrix Graphics::rotationZ(float theta) {
  Matrix rotation;
  rotation(0, 0) = cosf(theta);
  rotation(0, 1) = sinf(theta);
  rotation(1, 0) = -sinf(theta);
  rotation(1, 1) = cosf(theta);
  rotation(2, 2) = 1;
  return rotation;
}

SDL_Color Graphics::getColor(Vector3& normal) {
  Vector3 light = Vector3(0.0f, 0.0f, -1.0f);
  light = light.normalize();

  float dot = normal.dot(light);

  SDL_Color color;
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
  return color;
}
