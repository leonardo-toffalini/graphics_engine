#include "../include/engine.h"

float vec3d::norm() {
  return sqrtf(this->x * this->x + this->y * this->y + this->z * this->z);
}

vec3d vec3d::operator-(vec3d &right) {
  return (vec3d){this->x - right.x, this->y - right.y, this->z - right.z};
}

vec3d vec3d::operator/(float d) {
  vec3d res;
  res.x = this->x / d;
  res.y = this->y / d;
  res.z = this->z / d;
  return res;
}

vec3d vec3d::operator*(float c) {
  vec3d res;
  res.x = this->x * c;
  res.y = this->y * c;
  res.z = this->z * c;
  return res;
}

vec3d vec3d::normalise() {
  vec3d temp;
  float len = this->norm();
  temp = temp / len;
  return temp;
}

float dotProduct(vec3d &left, vec3d &right) {
  return left.x * right.x + left.y * right.y + left.z * right.z;
}

vec3d crossProduct(vec3d &left, vec3d &right) {
  vec3d res;
  res.x = left.y * right.z - left.z * right.y;
  res.y = left.z * right.x - left.x * right.z;
  res.z = left.x * right.y - left.y * right.x;
  return res;
}

vec3d MultiplyMatrixVector(mat4x4 &m, vec3d &v) {
  vec3d o;
  o.x = v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0] + m.m[3][0];
  o.y = v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1] + m.m[3][1];
  o.z = v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2] + m.m[3][2];
  float w = v.x * m.m[0][3] + v.y * m.m[1][3] + v.z * m.m[2][3] + m.m[3][3];

  if (w != 0.0f) {
    o.x = o.x / w;
    o.y = o.y / w;
    o.z = o.z / w;
  }
  return o;
}
