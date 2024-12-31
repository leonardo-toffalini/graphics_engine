#include "../include/engine.h"
#include "raylib.h"
#include "resource_dir.h" // utility header for SearchAndSetResourceDir
#include <iostream>
#include <string>

int main() {
  SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
  InitWindow(1280, 800, "Engine");
  SearchAndSetResourceDir("resources");

  mesh m;
  std::string objPath = "objects/axis.obj";
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

    // if (IsKeyDown('A')) camera.x -= 8.0f;
    // if (IsKeyDown('D')) camera.x += 8.0f;
    // if (IsKeyDown('W')) camera.y -= 8.0f;
    // if (IsKeyDown('S')) camera.y += 8.0f;

    // m.rotate(PI / 4.0f, 0.0f, PI / 8.0f, GetFrameTime());
    m.render(camera);

    EndDrawing();
  }

  CloseWindow();
  return 0;
}
