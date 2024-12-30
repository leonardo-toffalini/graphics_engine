#include "../include/engine.h"
#include <algorithm>
#include <fstream>
#include <string>
#include <strstream>
#include <vector>

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

bool readObjFile(mesh &o, std::string filePath) {
  std::vector<vec3d> verts;
  std::ifstream f(filePath);
  if (!f.is_open())
    return false;

  while (!f.eof()) {
    char line[128];
    f.getline(line, 128);

    std::strstream s;
    s << line;

    char temp;

    if (line[0] == 'v') {
      vec3d v;
      s >> temp >> v.x >> v.y >> v.z;
      verts.push_back(v);
    }

    if (line[0] == 'f') {
      int face[3];
      s >> temp >> face[0] >> face[1] >> face[2];
      o.tris.push_back(
          {verts[face[0] - 1], verts[face[1] - 1], verts[face[2] - 1]});
    }
  }
  return true;
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

void projectTri(triangle &i, triangle &o) {
  mat4x4 matProj = newProjMat();
  MultiplyMatrixVector(i.p[0], o.p[0], matProj);
  MultiplyMatrixVector(i.p[1], o.p[1], matProj);
  MultiplyMatrixVector(i.p[2], o.p[2], matProj);
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

float vecNorm(vec3d v) { return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z); }

vec3d vecMinus(vec3d &left, vec3d &right) {
  return (vec3d){left.x - right.x, left.y - right.y, left.z - right.z};
}

vec3d getSurfaceNormal(triangle &tri) {
  vec3d surfaceNormal, line1, line2;
  line1 = vecMinus(tri.p[1], tri.p[0]);
  line2 = vecMinus(tri.p[2], tri.p[0]);

  surfaceNormal.x = line1.y * line2.z - line1.z * line2.y;
  surfaceNormal.y = line1.z * line2.x - line1.x * line2.z;
  surfaceNormal.z = line1.x * line2.y - line1.y * line2.x;

  float len = vecNorm(surfaceNormal);
  surfaceNormal.x /= len;
  surfaceNormal.y /= len;
  surfaceNormal.z /= len;
  return surfaceNormal;
}

float dotProduct(vec3d &v, vec3d &u) {
  return v.x * u.x + v.y * u.y + v.z * u.z;
}

Color getTriColor(vec3d &surfaceNormal, vec3d &lightDirection) {
  float dp = dotProduct(surfaceNormal, lightDirection);
  unsigned char g = 255 * dp;
  return (Color){g, g, g, 255};
}

void drawTriangle(triangle &tri) {
  DrawTriangle({tri.p[0].x, tri.p[0].y}, {tri.p[1].x, tri.p[1].y},
               {tri.p[2].x, tri.p[2].y}, tri.c);
}

void drawTriangleWireFrame(triangle &tri) {
  DrawTriangleLines({tri.p[0].x, tri.p[0].y}, {tri.p[1].x, tri.p[1].y},
                    {tri.p[2].x, tri.p[2].y}, BLACK);
}

void render(mesh &m, vec3d &camera) {
  vec3d lightDirection = {0.0f, 0.0f, -1.0f};
  std::vector<triangle> triBuffer;

  for (auto tri : m.tris) {
    triangle triProjected, triTranslated;
    translateTriZ(tri, triTranslated, 8.0f);
    vec3d surfaceNormal = getSurfaceNormal(triTranslated);
    vec3d cameraTriVec = vecMinus(triTranslated.p[0], camera);
    if (dotProduct(surfaceNormal, cameraTriVec) < 0) {
      projectTri(triTranslated, triProjected);
      scaleToViewTri(triProjected, GetScreenWidth(), GetScreenHeight());
      Color c = getTriColor(surfaceNormal, lightDirection);
      triProjected.c = c;
      triBuffer.push_back(triProjected);
    }
  }

  sort(triBuffer.begin(), triBuffer.end(), [](triangle &t1, triangle &t2) {
    float z1 = (t1.p[0].z + t1.p[1].z + t1.p[2].z) / 3.0f;
    float z2 = (t2.p[0].z + t2.p[1].z + t2.p[2].z) / 3.0f;
    return z1 > z2;
  });

  for (auto tri : triBuffer) {
    drawTriangle(tri);
  }
}
