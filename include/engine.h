#ifndef ENGINE_H
#define ENGINE_H

#include <vector>
#include "raylib.h"

struct vec3d {
  float x, y, z;
};

struct triangle {
  vec3d p[3];
  Color c;
};

struct mesh {
  std::vector<triangle> tris;
};

struct mat4x4 {
  float m[4][4] = {0};
};

mesh newCubeMesh();
bool readObjFile(mesh &o, std::string filePath);
void rotate(mesh &m, float theta, float dt);
void render(mesh &m, vec3d &camera);

#endif // !ENGINE_H
