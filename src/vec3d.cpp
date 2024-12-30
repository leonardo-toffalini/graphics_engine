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
