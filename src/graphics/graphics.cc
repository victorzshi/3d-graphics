#include "graphics.h"

#include <SDL.h>

Graphics::Graphics() : isRunning_(true) {
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    fprintf(stderr, "SDL could not initialize! SDL error: %s\n",
            SDL_GetError());
  }

  window_ = SDL_CreateWindow("Graphics 3D", SDL_WINDOWPOS_UNDEFINED,
                             SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
                             SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
  if (window_ == nullptr) {
    fprintf(stderr, "Window was not created! SDL error: %s\n", SDL_GetError());
  }

  renderer_ = SDL_CreateRenderer(
      window_, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (renderer_ == nullptr) {
    fprintf(stderr, "Renderer was not create! SDL error: %s\n", SDL_GetError());
  }
}

Graphics::~Graphics() {
  SDL_DestroyWindow(window_);
  SDL_Quit();
}

void Graphics::run() {
  while (isRunning_) {
    SDL_SetRenderDrawColor(renderer_, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer_);

    SDL_SetRenderDrawColor(renderer_, 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_Rect box = {SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4, 100, 100};
    SDL_RenderFillRect(renderer_, &box);

    SDL_RenderPresent(renderer_);

    SDL_Delay(RUN_DURATION);

    isRunning_ = false;
  }
}
