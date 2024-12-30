#ifndef ENGINE_H
#define ENGINE_H

#include "raylib.h"
#include <vector>

struct vec3d {
  float x, y, z;

  float norm();
  vec3d normalise();
  vec3d operator-(vec3d &right);
  vec3d operator/(float d);
  vec3d operator*(float c);
};

float dotProduct(vec3d &left, vec3d &right);
vec3d crossProduct(vec3d &left, vec3d &right);

struct triangle {
  vec3d p[3];
  Color c;

  vec3d getSurfaceNormal();
};

struct mesh {
  std::vector<triangle> tris;

  bool readObjFile(std::string filePath);
  void rotate(float theta, float dt);
  void render(vec3d &camera);
};

struct mat4x4 {
  float m[4][4] = {0};
};

#endif // !ENGINE_H
