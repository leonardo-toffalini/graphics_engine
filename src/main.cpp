#include "../include/engine.h"
#include "raylib.h"
#include "resource_dir.h" // utility header for SearchAndSetResourceDir
#include <iostream>
#include <string>

int main() {
  SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
  InitWindow(1280, 800, "Hello Raylib");
  SearchAndSetResourceDir("resources");

  mesh m;
  std::string objPath = "objects/teapot.obj";
  bool success = m.readObjFile(objPath);
  if (!success) {
    std::cout << "Unable to load object from " << objPath;
    exit(1);
  }
  vec3d camera = {0, 0, 0};

  while (!WindowShouldClose()) // run the loop untill the user presses ESCAPE or
                               // presses the Close button on the window
  {
    BeginDrawing();
    ClearBackground(BLACK);

    m.rotate(1, GetFrameTime());
    m.render(camera);

    EndDrawing();
  }

  CloseWindow();
  return 0;
}
