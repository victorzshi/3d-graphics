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
  Mesh mesh = Mesh::loadFromObjectFile("sphere.obj");

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

    handleInput();

    Uint64 current = SDL_GetTicks64();
    Uint64 elapsed = current - previous;
    float theta = static_cast<float>(elapsed) / 1000.0f;

    // Set up transformations
    Matrix scaling = Matrix::scale(1.0f, 1.0f, 1.0f);
    Matrix rotation = Matrix::rotateY(theta) * Matrix::rotateX(theta);
    Matrix translation = Matrix::translate(Vector3(0.0f, 0.0f, 10.0f));
    Matrix world = Matrix::identity() * scaling * rotation * translation;

    // Set up camera
    Vector3 up = Vector3(0.0f, 1.0f, 0.0f);
    Vector3 target = Vector3(0.0f, 0.0f, 1.0f);
    lookDirection_ = Matrix::rotateY(yaw) * target;
    target = position_ + lookDirection_;
    Matrix camera = Matrix::pointAt(position_, target, up);
    Matrix view = Matrix::quickInverse(camera);

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
      Vector3 normal = a.cross(b).normalize();
      Vector3 temp = (transformed.point[0] - position_).normalize();

      if (normal.dot(temp) < 0.0f) {
        // Convert world space to view space
        Triangle viewed;
        for (int i = 0; i < 3; i++) {
          viewed.point[i] = view * transformed.point[i];
        }

        // Project from 3D to 2D
        Triangle projected;
        for (int i = 0; i < 3; i++) {
          projected.point[i] = projection * viewed.point[i];
        }

        // Normalize with reciprocal divide
        for (int i = 0; i < 3; i++) {
          float w = projected.point[i].w;
          if (w != 0.0f) {
            projected.point[i] /= w;
          }
        }

        // Offset into normalized space
        Vector3 offset = Vector3(1.0f, 1.0f, 0.0f);
        for (int i = 0; i < 3; i++) {
          projected.point[i] += offset;
          projected.point[i].x *= 0.5f * static_cast<float>(SCREEN_WIDTH);
          projected.point[i].y *= 0.5f * static_cast<float>(SCREEN_HEIGHT);
        }

        // Calculate lighting
        Vector3 light = Vector3(0.0f, 0.0f, -1.0f).normalize();
        projected.setColor(normal.dot(light));

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

void Graphics::handleInput() {
  const Uint8* currentKeyStates = SDL_GetKeyboardState(nullptr);
  if (currentKeyStates[SDL_SCANCODE_UP]) {
    position_.z += 0.1f;
  }
  if (currentKeyStates[SDL_SCANCODE_DOWN]) {
    position_.z -= 0.1f;
  }
  if (currentKeyStates[SDL_SCANCODE_LEFT]) {
    position_.x += 0.1f;
  }
  if (currentKeyStates[SDL_SCANCODE_RIGHT]) {
    position_.x -= 0.1f;
  }
  if (currentKeyStates[SDL_SCANCODE_SPACE]) {
    position_.y += 0.1f;
  }
  if (currentKeyStates[SDL_SCANCODE_LCTRL]) {
    position_.y -= 0.1f;
  }

  Vector3 forward = lookDirection_ * 0.1f;

  if (currentKeyStates[SDL_SCANCODE_W]) {
    position_ += forward;
  }
  if (currentKeyStates[SDL_SCANCODE_S]) {
    position_ -= forward;
  }
  if (currentKeyStates[SDL_SCANCODE_A]) {
    yaw += 0.01f;
  }
  if (currentKeyStates[SDL_SCANCODE_D]) {
    yaw -= 0.01f;
  }
}
