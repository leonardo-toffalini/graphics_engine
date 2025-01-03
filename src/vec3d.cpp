#include "../include/engine.h"
#include <iostream>

std::ostream &operator<<(std::ostream &os, const vec3d &vec) {
  os << "{" << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w << "}";
  return os;
}

float vec3d::norm() {
  return sqrtf(this->x * this->x + this->y * this->y + this->z * this->z);
}

vec3d vec3d::operator-(vec3d &right) {
  return (vec3d){this->x - right.x, this->y - right.y, this->z - right.z};
}

vec3d vec3d::operator+(vec3d &right) {
  return (vec3d){this->x + right.x, this->y + right.y, this->z + right.z};
}

vec3d vec3d::operator/(float d) {
  return (vec3d){this->x / d, this->y / d, this->z / d};
}

vec3d vec3d::operator*(float c) {
  return (vec3d){this->x * c, this->y * c, this->z * c};
}

vec3d vec3d::normalise() { return (vec3d) * this / this->norm(); }

float dotProduct(vec3d &left, vec3d &right) {
  return left.x * right.x + left.y * right.y + left.z * right.z;
}

vec3d crossProduct(vec3d &left, vec3d &right) {
  return (vec3d){left.y * right.z - left.z * right.y,
                 left.z * right.x - left.x * right.z,
                 left.x * right.y - left.y * right.x};
}
