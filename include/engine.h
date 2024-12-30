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

void projectTri(triangle &i, triangle &o);
void translateTriZ(triangle &i, triangle &o, float offset);
void scaleToViewTri(triangle &tri, float screenWidth, float screenHeight);

struct mesh {
  std::vector<triangle> tris;

  bool readObjFile(std::string filePath);
  void rotate(float theta, float dt);
  void render(vec3d &camera);
};

struct mat4x4 {
  float m[4][4] = {0};
};

mat4x4 matMul(mat4x4 &left, mat4x4 &right);
mat4x4 newRotMatX(float theta);
mat4x4 Matrix_MakeRotationY(float theta);
mat4x4 newRotMatZ(float theta);
mat4x4 newProjMat();
vec3d MultiplyMatrixVector(mat4x4 &m, vec3d &v);

#endif // !ENGINE_H
