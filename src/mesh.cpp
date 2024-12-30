#include "../include/engine.h"
#include <algorithm>
#include <fstream>
#include <strstream>

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

void mesh::rotate(float theta, float dt) {
  theta = dt * theta;
  mat4x4 rotMatZ = newRotMatZ(theta);
  mat4x4 rotMatX = newRotMatX(theta);
  mat4x4 rotMat = matMul(rotMatZ, rotMatX);
  for (auto &tri : this->tris) {
    triangle temp;
    temp.p[0] = MultiplyMatrixVector(rotMatZ, tri.p[0]);
    temp.p[1] = MultiplyMatrixVector(rotMatZ, tri.p[1]);
    temp.p[2] = MultiplyMatrixVector(rotMatZ, tri.p[2]);
    tri = temp;
    temp.p[0] = MultiplyMatrixVector(rotMatX, tri.p[0]);
    temp.p[1] = MultiplyMatrixVector(rotMatX, tri.p[1]);
    temp.p[2] = MultiplyMatrixVector(rotMatX, tri.p[2]);
    tri = temp;
  }
}

void mesh::render(vec3d &camera) {
  vec3d lightDirection = {0.0f, 0.0f, -1.0f};
  std::vector<triangle> triBuffer;

  for (auto tri : this->tris) {
    triangle triProjected, triTranslated;
    translateTriZ(tri, triTranslated, 28.0f);
    vec3d surfaceNormal = triTranslated.getSurfaceNormal();
    vec3d cameraTriVec = triTranslated.p[0] - camera;
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

bool mesh::readObjFile(std::string filePath) {
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
      this->tris.push_back(
          {verts[face[0] - 1], verts[face[1] - 1], verts[face[2] - 1]});
    }
  }
  return true;
}
