#include "../include/engine.h"
#include <iostream>

std::ostream &operator<<(std::ostream &os, const triangle &tri) {
  os << "{p1: {" << tri.p[0].x << ", " << tri.p[0].y << ", " << tri.p[0].z
     << "}, ";
  os << "p2: {" << tri.p[1].x << ", " << tri.p[1].y << ", " << tri.p[1].z
     << "}, ";
  os << "p3: {" << tri.p[2].x << ", " << tri.p[2].y << ", " << tri.p[2].z
     << "}}";
  return os;
}

triangle triangle::project() {
  triangle o;
  mat4x4 matProj = newProjMat();
  o.p[0] = matProj * this->p[0];
  o.p[1] = matProj * this->p[1];
  o.p[2] = matProj * this->p[2];
  return o;
}

triangle triangle::translate(vec3d offset) {
  triangle res;
  res.p[0] = this->p[0] + offset;
  res.p[1] = this->p[1] + offset;
  res.p[2] = this->p[2] + offset;
  return res;
}

void triangle::scaleToView(float screenWidth, float screenHeight) {
  this->p[0].x += 1.0f;
  this->p[0].y += 1.0f;
  this->p[1].x += 1.0f;
  this->p[1].y += 1.0f;
  this->p[2].x += 1.0f;
  this->p[2].y += 1.0f;
  this->p[0].x *= 0.5f * screenWidth;
  this->p[0].y *= 0.5f * screenHeight;
  this->p[1].x *= 0.5f * screenWidth;
  this->p[1].y *= 0.5f * screenHeight;
  this->p[2].x *= 0.5f * screenWidth;
  this->p[2].y *= 0.5f * screenHeight;
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
