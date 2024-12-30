#include "../include/engine.h"

void projectTri(triangle &i, triangle &o) {
  mat4x4 matProj = newProjMat();
  o.p[0] = MultiplyMatrixVector(matProj, i.p[0]);
  o.p[1] = MultiplyMatrixVector(matProj, i.p[1]);
  o.p[2] = MultiplyMatrixVector(matProj, i.p[2]);
}

void translateTriZ(triangle &i, triangle &o, float offset) {
  o = i;
  o.p[0].z = i.p[0].z + offset;
  o.p[1].z = i.p[1].z + offset;
  o.p[2].z = i.p[2].z + offset;
}

void scaleToViewTri(triangle &tri, float screenWidth, float screenHeight) {
  tri.p[0].x += 1.0f;
  tri.p[0].y += 1.0f;
  tri.p[1].x += 1.0f;
  tri.p[1].y += 1.0f;
  tri.p[2].x += 1.0f;
  tri.p[2].y += 1.0f;
  tri.p[0].x *= 0.5f * screenWidth;
  tri.p[0].y *= 0.5f * screenHeight;
  tri.p[1].x *= 0.5f * screenWidth;
  tri.p[1].y *= 0.5f * screenHeight;
  tri.p[2].x *= 0.5f * screenWidth;
  tri.p[2].y *= 0.5f * screenHeight;
}

vec3d triangle::getSurfaceNormal() {
  vec3d surfaceNormal, line1, line2;
  line1 = this->p[1] - this->p[0];
  line2 = this->p[2] - this->p[0];

  surfaceNormal = crossProduct(line1, line2);

  float len = surfaceNormal.norm();
  surfaceNormal = surfaceNormal / len;
  return surfaceNormal;
}
