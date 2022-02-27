#include "SDL.h"

#include "graphics/graphics.h"

int main(int argc, char *argv[]) {
  (void)argc;
  (void)argv;

  Graphics graphics;

  graphics.run();

  return 0;
}
