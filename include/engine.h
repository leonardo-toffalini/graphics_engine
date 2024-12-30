#ifndef ENGINE_H
#define ENGINE_H

#include <vector>
#include "raylib.h"

struct vec3d {
  float x, y, z;
};

struct triangle {
  vec3d p[3];
};

struct mesh {
  std::vector<triangle> tris;
};

struct mat4x4 {
  float m[4][4] = {0};
};

mesh newCubeMesh();
void rotate(mesh &m, float theta, float dt);
void render(mesh m);

#endif // !ENGINE_H
