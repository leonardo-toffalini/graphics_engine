#include "../include/engine.h"
#include "raylib.h"
#include "resource_dir.h" // utility header for SearchAndSetResourceDir

int main() {
  SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
  InitWindow(1280, 800, "Hello Raylib");
  SearchAndSetResourceDir("resources");

  mesh cube = newCubeMesh();

  while (!WindowShouldClose()) // run the loop untill the user presses ESCAPE or
                               // presses the Close button on the window
  {
    BeginDrawing();
    ClearBackground(BLACK);

    render(cube);

    EndDrawing();
  }

  CloseWindow();
  return 0;
}
