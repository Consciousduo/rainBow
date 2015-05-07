#pragma once

#include "v3.h"
class PPC;
class FrameBuffer;

class TMesh {
public:
  V3 *verts;
  V3 *cols;
  V3 *normals;
  V3 *tcs;
  int vertsN;
  unsigned int *tris;
  int trisN;
  bool enabled;
  FrameBuffer *texture;
  TMesh() : texture(0), tcs(0), enabled(true), verts(0), vertsN(0), 
    tris(0), trisN(0), cols(0), normals(0){};
  void LoadBin(char *fname);
  void RotateAboutArbitraryAxis(V3 O, V3 a, float theta);
  V3 GetCenter();
  V3 GetCenterMass();
  void SetRectangle(V3 center, float a, float b);
  void RenderHW();
  void RenderDP(V3 eye);
  void SetDispersivePrism(float height, float side);
};