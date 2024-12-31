#include "../include/engine.h"
#include <algorithm>
#include <fstream>
#include <iostream>
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

void mesh::rotate(float alpha, float beta, float gamma, float dt) {
  alpha = dt * alpha;
  beta = dt * beta;
  gamma = dt * gamma;
  mat4x4 rotMatX = newRotMatX(alpha);
  mat4x4 rotMatY = newRotMatX(beta);
  mat4x4 rotMatZ = newRotMatZ(gamma);
  mat4x4 rotMat = rotMatX * rotMatZ;
  for (auto &tri : this->tris) {
    triangle temp;
    temp.p[0] = rotMat * tri.p[0];
    temp.p[1] = rotMat * tri.p[1];
    temp.p[2] = rotMat * tri.p[2];
    tri = temp;
  }
}

void mesh::render(vec3d &camera) {
  vec3d lightDirection = {0.0f, 0.0f, -1.0f};

  vec3d upVec = {0, 1, 0};
  vec3d lookDir = {0, 0, 1};
  vec3d targetVec = camera + lookDir;

  mat4x4 matCamera = Matrix_PointAt(camera, targetVec, upVec);
  mat4x4 matView = Matrix_QuickInverse(matCamera);
  std::cout << matView << "\n";

  std::vector<triangle> triBuffer;

  for (auto tri : this->tris) {
    triangle triProjected, triTransformed, triViewed;
    triTransformed = tri.translate((vec3d){0.0f, 0.0f, 20.0f});

    vec3d surfaceNormal = triTransformed.getSurfaceNormal();
    vec3d cameraRay = triTransformed.p[0] - camera;
    if (dotProduct(surfaceNormal, cameraRay) < 0) {
      triViewed.p[0] = matView * triTransformed.p[0];
      triViewed.p[1] = matView * triTransformed.p[1];
      triViewed.p[2] = matView * triTransformed.p[2];
      // std::cout << triTransformed << "\n";
      // std::cout << triViewed << "\n";

      triProjected = triViewed.project();
      // triProjected = triTransformed.project();
      triProjected.scaleToView(GetScreenWidth(), GetScreenHeight());
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

  this->rotate(3.141592f / 4.0f, 0, 3.141592f,
               1); // rotate axis to be in correct orientation
  return true;
}
