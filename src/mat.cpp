#include "../include/engine.h"

mat4x4 matMul(mat4x4 &left, mat4x4 &right) {
  mat4x4 res;
  for (int c = 0; c < 4; c++) {
    for (int r = 0; r < 4; r++) {
      for (int i = 0; i < 4; i++) {
        res.m[r][c] += left.m[r][i] * right.m[i][c];
      }
    }
  }
  return res;
}

mat4x4 newRotMatX(float theta) {
  mat4x4 matRotX;
  matRotX.m[0][0] = 1;
  matRotX.m[1][1] = cosf(theta * 0.5f);
  matRotX.m[1][2] = sinf(theta * 0.5f);
  matRotX.m[2][1] = -sinf(theta * 0.5f);
  matRotX.m[2][2] = cosf(theta * 0.5f);
  matRotX.m[3][3] = 1;
  return matRotX;
}
mat4x4 Matrix_MakeRotationY(float theta) {
  mat4x4 matrix;
  matrix.m[0][0] = cosf(theta);
  matrix.m[0][2] = sinf(theta);
  matrix.m[2][0] = -sinf(theta);
  matrix.m[1][1] = 1.0f;
  matrix.m[2][2] = cosf(theta);
  matrix.m[3][3] = 1.0f;
  return matrix;
}

mat4x4 newRotMatZ(float theta) {
  mat4x4 matRotZ;
  matRotZ.m[0][0] = cosf(theta);
  matRotZ.m[0][1] = sinf(theta);
  matRotZ.m[1][0] = -sinf(theta);
  matRotZ.m[1][1] = cosf(theta);
  matRotZ.m[2][2] = 1;
  matRotZ.m[3][3] = 1;
  return matRotZ;
}

mat4x4 newProjMat() {
  mat4x4 matProj;
  float near = 0.1f;
  float far = 1000.0f;
  float fov = 90.0f;
  float aspectRatio = (float)GetScreenHeight() / (float)GetScreenWidth();
  float fovRad = 1.0f / tanf(fov * 0.5f / 180.0f * 3.14159f);

  matProj.m[0][0] = aspectRatio * fovRad;
  matProj.m[1][1] = fovRad;
  matProj.m[2][2] = far / (far - near);
  matProj.m[3][2] = (-far * near) / (far - near);
  matProj.m[2][3] = 1.0f;
  matProj.m[3][3] = 0.0f;

  return matProj;
}

mat4x4 Matrix_PointAt(vec3d &pos, vec3d &target, vec3d &up) {
  // Calculate new forward direction
  vec3d newForward = target - pos;
  newForward.normalise();

  // Calculate new Up direction
  vec3d a = newForward * dotProduct(up, newForward);
  vec3d newUp = up - a;
  newUp = newUp.normalise();

  // New Right direction is easy, its just cross product
  vec3d newRight = crossProduct(newUp, newForward);

  // Construct Dimensioning and Translation Matrix
  mat4x4 matrix;
  matrix.m[0][0] = newRight.x;
  matrix.m[0][1] = newRight.y;
  matrix.m[0][2] = newRight.z;
  matrix.m[0][3] = 0.0f;
  matrix.m[1][0] = newUp.x;
  matrix.m[1][1] = newUp.y;
  matrix.m[1][2] = newUp.z;
  matrix.m[1][3] = 0.0f;
  matrix.m[2][0] = newForward.x;
  matrix.m[2][1] = newForward.y;
  matrix.m[2][2] = newForward.z;
  matrix.m[2][3] = 0.0f;
  matrix.m[3][0] = pos.x;
  matrix.m[3][1] = pos.y;
  matrix.m[3][2] = pos.z;
  matrix.m[3][3] = 1.0f;
  return matrix;
}

mat4x4 Matrix_QuickInverse(mat4x4 &m) // Only for Rotation/Translation Matrices
{
  mat4x4 matrix;
  matrix.m[0][0] = m.m[0][0];
  matrix.m[0][1] = m.m[1][0];
  matrix.m[0][2] = m.m[2][0];
  matrix.m[0][3] = 0.0f;
  matrix.m[1][0] = m.m[0][1];
  matrix.m[1][1] = m.m[1][1];
  matrix.m[1][2] = m.m[2][1];
  matrix.m[1][3] = 0.0f;
  matrix.m[2][0] = m.m[0][2];
  matrix.m[2][1] = m.m[1][2];
  matrix.m[2][2] = m.m[2][2];
  matrix.m[2][3] = 0.0f;
  matrix.m[3][0] = -(m.m[3][0] * matrix.m[0][0] + m.m[3][1] * matrix.m[1][0] +
                     m.m[3][2] * matrix.m[2][0]);
  matrix.m[3][1] = -(m.m[3][0] * matrix.m[0][1] + m.m[3][1] * matrix.m[1][1] +
                     m.m[3][2] * matrix.m[2][1]);
  matrix.m[3][2] = -(m.m[3][0] * matrix.m[0][2] + m.m[3][1] * matrix.m[1][2] +
                     m.m[3][2] * matrix.m[2][2]);
  matrix.m[3][3] = 1.0f;
  return matrix;
}
