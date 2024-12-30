#include "../include/engine.h"

void MultiplyMatrixVector(vec3d &i, vec3d &o, mat4x4 &m) {
  o.x = i.x * m.m[0][0] + i.y * m.m[1][0] + i.z * m.m[2][0] + m.m[3][0];
  o.y = i.x * m.m[0][1] + i.y * m.m[1][1] + i.z * m.m[2][1] + m.m[3][1];
  o.z = i.x * m.m[0][2] + i.y * m.m[1][2] + i.z * m.m[2][2] + m.m[3][2];
  float w = i.x * m.m[0][3] + i.y * m.m[1][3] + i.z * m.m[2][3] + m.m[3][3];

  if (w != 0.0f) {
    o.x = o.x / w;
    o.y = o.y / w;
    o.z = o.z / w;
  }
}

mesh newCubeMesh() {
  mesh meshCube;
  meshCube.tris = {
      // SOUTH
      {0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f},
      {0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f},

      // EAST
      {1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f},
      {1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f},

      // NORTH
      {1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f},
      {1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f},

      // WEST
      {0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f},
      {0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f},

      // TOP
      {0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f},
      {0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f},

      // BOTTOM
      {1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f},
      {1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f},
  };
  return meshCube;
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

void rotate(mesh &m, float theta, float dt) {
  theta = dt * theta;
  mat4x4 rotMatZ = newRotMatZ(theta);
  mat4x4 rotMatX = newRotMatX(theta);
  for (auto &tri : m.tris) {
    triangle temp;
    MultiplyMatrixVector(tri.p[0], temp.p[0], rotMatZ);
    MultiplyMatrixVector(tri.p[1], temp.p[1], rotMatZ);
    MultiplyMatrixVector(tri.p[2], temp.p[2], rotMatZ);
    tri = temp;
    MultiplyMatrixVector(tri.p[0], temp.p[0], rotMatX);
    MultiplyMatrixVector(tri.p[1], temp.p[1], rotMatX);
    MultiplyMatrixVector(tri.p[2], temp.p[2], rotMatX);
    tri = temp;
  }
}

void render(mesh m) {
  mat4x4 matProj = newProjMat();

  for (auto tri : m.tris) {
    triangle triProjected, triTranslated;
    // Offset into the screen
    triTranslated = tri;
    triTranslated.p[0].z = tri.p[0].z + 3.0f;
    triTranslated.p[1].z = tri.p[1].z + 3.0f;
    triTranslated.p[2].z = tri.p[2].z + 3.0f;

    MultiplyMatrixVector(triTranslated.p[0], triProjected.p[0], matProj);
    MultiplyMatrixVector(triTranslated.p[1], triProjected.p[1], matProj);
    MultiplyMatrixVector(triTranslated.p[2], triProjected.p[2], matProj);

    // Scale into view
    triProjected.p[0].x += 1.0f;
    triProjected.p[0].y += 1.0f;
    triProjected.p[1].x += 1.0f;
    triProjected.p[1].y += 1.0f;
    triProjected.p[2].x += 1.0f;
    triProjected.p[2].y += 1.0f;
    triProjected.p[0].x *= 0.5f * (float)GetScreenWidth();
    triProjected.p[0].y *= 0.5f * (float)GetScreenHeight();
    triProjected.p[1].x *= 0.5f * (float)GetScreenWidth();
    triProjected.p[1].y *= 0.5f * (float)GetScreenHeight();
    triProjected.p[2].x *= 0.5f * (float)GetScreenWidth();
    triProjected.p[2].y *= 0.5f * (float)GetScreenHeight();

    DrawTriangleLines({triProjected.p[0].x, triProjected.p[0].y},
                      {triProjected.p[1].x, triProjected.p[1].y},
                      {triProjected.p[2].x, triProjected.p[2].y}, WHITE);
  }
}
