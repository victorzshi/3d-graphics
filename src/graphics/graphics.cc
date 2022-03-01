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
  mesh.loadFromObjectFile("teapot.obj");

  Vector3 camera = Vector3();

  float fov = 90.0f;
  float pi = static_cast<float>(atan(1)) * 4;
  float distance = 1.0f / tanf(fov / 2 * pi / 180);
  float aspect = static_cast<float>(SCREEN_WIDTH) / SCREEN_HEIGHT;
  float near = 0.1f;
  float far = 100.0f;
  Matrix projection = Matrix::projection(distance, aspect, near, far);

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

    // Set up transformations
    Matrix scaling = Matrix::scale(0.25f, 0.25f, 0.25f);

    float theta = static_cast<float>(elapsed) / 1000.0f;
    Matrix rotation = Matrix::rotateY(theta) * Matrix::rotateX(theta);

    Matrix translation = Matrix::translate(Vector3(0.0f, 0.0f, 10.0f));

    Matrix world = Matrix::identity() * scaling * rotation * translation;

    // Cull triangles
    std::vector<Triangle> raster;

    for (auto triangle : mesh.triangles) {
      Triangle transformed;
      for (int i = 0; i < 3; i++) {
        transformed.point[i] = world * triangle.point[i];
      }

      // Calculate normal
      Vector3 a = transformed.point[1] - transformed.point[0];
      Vector3 b = transformed.point[2] - transformed.point[0];
      Vector3 normal = a.cross(b);
      normal = normal.normalize();

      if (normal.dot(transformed.point[0] - camera) < 0) {
        // Project from 3D to 2D
        Triangle projected;
        for (int i = 0; i < 3; i++) {
          projected.point[i] = projection * transformed.point[i];
        }

        // Scale into view
        for (int i = 0; i < 3; i++) {
          projected.point[i] += Vector3(1.0f, 1.0f, 0.0f);
        }

        projected.point[0].x *= 0.5f * static_cast<float>(SCREEN_WIDTH);
        projected.point[0].y *= 0.5f * static_cast<float>(SCREEN_HEIGHT);
        projected.point[1].x *= 0.5f * static_cast<float>(SCREEN_WIDTH);
        projected.point[1].y *= 0.5f * static_cast<float>(SCREEN_HEIGHT);
        projected.point[2].x *= 0.5f * static_cast<float>(SCREEN_WIDTH);
        projected.point[2].y *= 0.5f * static_cast<float>(SCREEN_HEIGHT);

        // Lighting
        Vector3 light = Vector3(0.0f, 0.0f, -1.0f);
        light = light.normalize();

        float dot = normal.dot(light);
        projected.setColor(dot);

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
