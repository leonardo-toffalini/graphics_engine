#ifndef ENGINE_H
#define ENGINE_H

#include "raylib.h"
#include <iostream>
#include <string>
#include <vector>

struct vec3d {
  float x = 0;
  float y = 0;
  float z = 0;
  float w = 1;

  float norm();
  vec3d normalise();
  vec3d operator-(vec3d &right);
  vec3d operator+(vec3d &right);
  vec3d operator/(float d);
  vec3d operator*(float c);
};

float dotProduct(vec3d &left, vec3d &right);
vec3d crossProduct(vec3d &left, vec3d &right);

struct triangle {
  vec3d p[3];
  Color c;

  vec3d getSurfaceNormal();
  triangle project();
  triangle translate(vec3d offset);
  void scaleToView(float screenWidth, float screenHeight);
};

std::ostream &operator<<(std::ostream &os, const triangle &tri);

struct mesh {
  std::vector<triangle> tris;

  bool readObjFile(std::string filePath);
  void rotate(float alpha, float beta, float gamma, float dt);
  void render(vec3d &camera);
};

struct mat4x4 {
  float m[4][4] = {0};
  mat4x4 operator*(mat4x4 &right);
  vec3d operator*(vec3d &v);
};

std::ostream &operator<<(std::ostream &os, const mat4x4 &mat);
mat4x4 newRotMatX(float theta);
mat4x4 newRotMatY(float theta);
mat4x4 newRotMatZ(float theta);
mat4x4 Matrix_PointAt(vec3d &pos, vec3d &target, vec3d &up);
mat4x4 Matrix_QuickInverse(mat4x4 &m);
mat4x4 newProjMat();

#endif // !ENGINE_H
