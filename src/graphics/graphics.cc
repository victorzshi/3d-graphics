#include "graphics.h"

#include <SDL.h>
#include <assert.h>

#include <algorithm>
#include <iostream>
#include <list>
#include <vector>

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
  mesh_ = Mesh::loadFromObjectFile("teapot.obj");

  float fov = 90.0f;
  float pi = static_cast<float>(atan(1)) * 4;
  float distance = 1.0f / tanf(fov / 2 * pi / 180);
  float aspect = static_cast<float>(SCREEN_WIDTH) / SCREEN_HEIGHT;
  float near = 0.1f;
  float far = 100.0f;
  projection_ = Matrix::projection(distance, aspect, near, far);

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
    Matrix scaling = Matrix::scale(0.2f, 0.2f, 0.2f);
    Matrix rotation = Matrix::rotateY(theta) * Matrix::rotateZ(theta);
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

    for (auto triangle : mesh_.triangles) {
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
        Triangle viewed;

        // Calculate lighting
        Vector3 light = Vector3(0.0f, 0.0f, -1.0f).normalize();
        viewed.setColor(normal.dot(light));

        // Convert world space to view space
        for (int i = 0; i < 3; i++) {
          viewed.point[i] = view * transformed.point[i];
        }

        // Clip viewed triangles against near plane
        Vector3 planePoint = Vector3(0.0f, 0.0f, near);
        Vector3 planeNormal = Vector3(0.0f, 0.0f, 1.0f);
        Clipped clipped = clipAgainstPlane(planePoint, planeNormal, viewed);

        for (int i = 0; i < clipped.total; i++) {
          Triangle projected;

          // Project from 3D to 2D
          for (int j = 0; j < 3; j++) {
            projected.point[j] = projection_ * clipped.triangle[i].point[j];
            projected.color = clipped.triangle[i].color;
          }

          // Normalize with reciprocal divide
          for (int j = 0; j < 3; j++) {
            float w = projected.point[j].w;
            assert(w != 0.0f);
            projected.point[j] /= w;
          }

          // Offset into normalized space
          Vector3 offset = Vector3(1.0f, 1.0f, 0.0f);
          for (int j = 0; j < 3; j++) {
            projected.point[j] += offset;
            projected.point[j].x *= 0.5f * static_cast<float>(SCREEN_WIDTH);
            projected.point[j].y *= 0.5f * static_cast<float>(SCREEN_HEIGHT);
          }

          raster.push_back(projected);
        }
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

    for (auto triangleRaster : raster) {
      // Clip triangles against all four screen edges
      Clipped clipped;
      std::list<Triangle> triangles;

      triangles.push_back(triangleRaster);
      size_t newTriangles = 1;

      for (int plane = 0; plane < 4; plane++) {
        // int trianglesToAdd = 0;

        while (newTriangles > 0) {
          Triangle test = triangles.front();
          triangles.pop_front();
          newTriangles--;

          Vector3 planePoint;
          Vector3 planeNormal;

          switch (plane) {
            case 0:
              planePoint = Vector3(0.0f, 0.0f, 0.0f);
              planeNormal = Vector3(0.0f, 1.0f, 0.0f);
              break;
            case 1:
              planePoint = Vector3(0.0f, SCREEN_HEIGHT - 1, 0.0f);
              planeNormal = Vector3(0.0f, -1.0f, 0.0f);
              break;
            case 2:
              planePoint = Vector3(0.0f, 0.0f, 0.0f);
              planeNormal = Vector3(1.0f, 0.0f, 0.0f);
              break;
            case 3:
              planePoint = Vector3(SCREEN_WIDTH - 1, 0.0f, 0.0f);
              planeNormal = Vector3(-1.0f, 0.0f, 0.0f);
              break;
          }

          clipped = clipAgainstPlane(planePoint, planeNormal, test);

          for (int i = 0; i < clipped.total; i++) {
            triangles.push_back(clipped.triangle[i]);
          }
        }
        newTriangles = triangles.size();
      }

      for (auto& triangle : triangles) {
        triangle.render(renderer_);
      }
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

Vector3 Graphics::intersectPlane(Vector3& planePoint, Vector3 planeNormal,
                                 Vector3& lineStart, Vector3& lineEnd) const {
  float planeDot = -1 * planeNormal.dot(planePoint);
  float aDot = lineStart.dot(planeNormal);
  float bDot = lineEnd.dot(planeNormal);
  float t = (-1 * planeDot - aDot) / (bDot - aDot);
  Vector3 lineStartToEnd = lineEnd - lineStart;
  Vector3 lineToIntersect = lineStartToEnd * t;
  return lineStart + lineToIntersect;
}

Clipped Graphics::clipAgainstPlane(Vector3 planePoint, Vector3 planeNormal,
                                   Triangle& triangle) const {
  Clipped clipped;

  // Return signed shortest distance from point to plane, plane normal must be
  // normalised
  auto dist = [&](Vector3& p) {
    // Vector3 n = p.normalize();
    return (planeNormal.x * p.x + planeNormal.y * p.y + planeNormal.z * p.z -
            planeNormal.dot(planePoint));
  };

  // Create two temporary storage arrays to classify points either side of plane
  // If distance sign is positive, point lies on "inside" of plane
  Vector3* insidePoint[3];
  int insidePointTotal = 0;
  Vector3* outsidePoint[3];
  int outsidePointTotal = 0;

  // Get signed distance of each point in triangle to plane
  for (int i = 0; i < 3; i++) {
    float distance = dist(triangle.point[i]);
    if (distance >= 0) {
      insidePoint[insidePointTotal++] = &triangle.point[i];
    } else {
      outsidePoint[outsidePointTotal++] = &triangle.point[i];
    }
  }

  // Now classify triangle points, and break the input triangle into
  // smaller output triangles if required. There are four possible
  // outcomes...
  if (insidePointTotal == 0) {
    clipped.total = 0;
  } else if (insidePointTotal == 3) {
    clipped.total = 1;
    clipped.triangle[0] = triangle;
  } else if (insidePointTotal == 1 && outsidePointTotal == 2) {
    clipped.total = 1;

#ifdef DEBUG
    clipped.triangle[0].color = SDL_Color{255, 0, 0, 255};
#else
    clipped.triangle[0].color = triangle.color;
#endif

    clipped.triangle[0].point[0] = *insidePoint[0];

    clipped.triangle[0].point[1] = intersectPlane(
        planePoint, planeNormal, *insidePoint[0], *outsidePoint[0]);
    clipped.triangle[0].point[2] = intersectPlane(
        planePoint, planeNormal, *insidePoint[0], *outsidePoint[1]);
  } else if (insidePointTotal == 2 && outsidePointTotal == 1) {
    clipped.total = 2;

#ifdef DEBUG
    clipped.triangle[0].color = SDL_Color{0, 255, 0, 255};
    clipped.triangle[1].color = SDL_Color{0, 0, 255, 255};
#else
    clipped.triangle[0].color = triangle.color;
    clipped.triangle[1].color = triangle.color;
#endif

    clipped.triangle[0].point[0] = *insidePoint[0];
    clipped.triangle[0].point[1] = *insidePoint[1];
    clipped.triangle[0].point[2] = intersectPlane(
        planePoint, planeNormal, *insidePoint[0], *outsidePoint[0]);

    clipped.triangle[1].point[0] = *insidePoint[1];
    clipped.triangle[1].point[1] = clipped.triangle[0].point[2];
    clipped.triangle[1].point[2] = intersectPlane(
        planePoint, planeNormal, *insidePoint[1], *outsidePoint[0]);
  }

  return clipped;
}
