#pragma once

#include "v3.h"
class FrameBuffer;

class PPC {
public:
  V3 a, b, c, C;
  int w, h;
  PPC(float hfov,  int w, int h);
  bool Project(V3 p, V3 &pp);
  V3 GetVD();
  void TranslateFB(float step);
  void Draw(PPC *ppc, FrameBuffer *fb, float imagePlanez);
  float Getf();
  void RotateAboutAxis(V3 aO, V3 aD, float theta);
  V3 GetPoint(int u, int v, float z);
  V3 Unproject(V3 projP);
  void SetViewGL(float zNear, float zFar);
};